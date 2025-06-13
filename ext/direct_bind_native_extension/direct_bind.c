#include "direct_bind.h"

#include <ruby.h>

// # Self-test implementation

#define SELF_TEST_TARGET "direct_bind_self_test_target"
#define SELF_TEST_ARITY 3

static VALUE self_test_target_func(VALUE, VALUE, VALUE, VALUE) { return Qnil; }

bool direct_bind_self_test(bool raise_on_failure) {
  VALUE anonymous_module = rb_module_new();
  rb_define_method(anonymous_module, SELF_TEST_TARGET, self_test_target_func, SELF_TEST_ARITY);

  direct_bind_cfunc_result test_target =
    direct_bind_get_cfunc(anonymous_module, rb_intern(SELF_TEST_TARGET), raise_on_failure);

  if (!test_target.ok) return false;

  return test_target.arity == SELF_TEST_ARITY && test_target.func == self_test_target_func;
}

// # Structure layouts and exported symbol definitions from Ruby

// ## From internal/gc.h
void rb_objspace_each_objects(int (*callback)(void *start, void *end, size_t stride, void *data), void *data);
int rb_objspace_internal_object_p(VALUE obj);

// ## From method.h
typedef struct rb_method_entry_struct {
  VALUE flags;
  VALUE defined_class;
  struct rb_method_definition_struct * const def;
  ID called_id;
  VALUE owner;
} rb_method_entry_t;

// ### TODO: Cleanup a bit?
#define VM_METHOD_TYPE_MINIMUM_BITS 4

typedef VALUE (*rb_cfunc_t)(ANYARGS);
typedef struct rb_method_cfunc_struct {
  rb_cfunc_t func;
  VALUE (*invoker)(VALUE recv, int argc, const VALUE *argv, VALUE (*func)(ANYARGS));
  int argc;
} rb_method_cfunc_t;

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
# define BITFIELD(type, name, size) type name : size
#else
# define BITFIELD(type, name, size) unsigned int name : size
#endif

typedef enum {
  VM_METHOD_TYPE_ISEQ,      /*!< Ruby method */
  VM_METHOD_TYPE_CFUNC,     /*!< C method */
  VM_METHOD_TYPE_ATTRSET,   /*!< attr_writer or attr_accessor */
  VM_METHOD_TYPE_IVAR,      /*!< attr_reader or attr_accessor */
  VM_METHOD_TYPE_BMETHOD,
  VM_METHOD_TYPE_ZSUPER,
  VM_METHOD_TYPE_ALIAS,
  VM_METHOD_TYPE_UNDEF,
  VM_METHOD_TYPE_NOTIMPLEMENTED,
  VM_METHOD_TYPE_OPTIMIZED, /*!< Kernel#send, Proc#call, etc */
  VM_METHOD_TYPE_MISSING,   /*!< wrapper for method_missing(id) */
  VM_METHOD_TYPE_REFINED,   /*!< refinement */
} rb_method_type_t;

struct rb_method_definition_struct {
  BITFIELD(rb_method_type_t, type, VM_METHOD_TYPE_MINIMUM_BITS);
  unsigned int iseq_overload: 1;
  unsigned int no_redef_warning: 1;
  unsigned int aliased : 1;
  int reference_count : 28;

  union {
    rb_method_cfunc_t cfunc;
  } body;
};

// # This is where the magic happens: Using objectspace to find the method entry and retrieve the cfunc

typedef struct {
  VALUE target_klass;
  ID target_id;
  direct_bind_cfunc_result result;
} find_data_t;

static bool valid_method_entry(VALUE object);
static bool found_target_method_entry(rb_method_entry_t *method_entry, find_data_t *find_data);
static int find_cfunc(void *start, void *end, size_t stride, void *data);

direct_bind_cfunc_result direct_bind_get_cfunc(VALUE klass, ID method_name, bool raise_on_failure) {
  VALUE definition_not_found = rb_sprintf("method %"PRIsVALUE".%"PRIsVALUE" not found", klass, ID2SYM(method_name));

  find_data_t find_data = {.target_klass = klass, .target_id = method_name, .result = {.ok = false, .failure_reason = definition_not_found}};
  rb_objspace_each_objects(find_cfunc, &find_data);

  if (raise_on_failure && find_data.result.ok == false) {
    rb_raise(rb_eRuntimeError, "direct_bind_get_cfunc failed: %"PRIsVALUE, find_data.result.failure_reason);
  }

  return find_data.result;
}

// TODO: Change this to use safe memory reads that can never segv (e.g. if structure layouts are off?)
static int find_cfunc(void *start, void *end, size_t stride, void *data) {
  const int stop_iteration = 1;
  const int continue_iteration = 0;

  find_data_t *find_data = (find_data_t *) data;

  for (VALUE v = (VALUE) start; v != (VALUE) end; v += stride) {
    if (!valid_method_entry(v)) continue;

    rb_method_entry_t *method_entry = (rb_method_entry_t*) v;
    if (!found_target_method_entry(method_entry, find_data)) continue;

    if (method_entry->def == NULL) {
      find_data->result.failure_reason = rb_str_new_lit("method_entry->def is NULL");
    } else if (method_entry->def->type != VM_METHOD_TYPE_CFUNC) {
      find_data->result.failure_reason = rb_str_new_lit("method_entry is not a cfunc");
    } else {
      find_data->result = (direct_bind_cfunc_result) {
        .ok = true,
        .failure_reason = Qnil,
        .arity = method_entry->def->body.cfunc.argc,
        .func = method_entry->def->body.cfunc.func,
      };
    }
    return stop_iteration;
  }

  return continue_iteration;
}

static bool is_method_entry(VALUE imemo) {
  const unsigned long method_entry_id = 6;
  return ((RBASIC(imemo)->flags >> FL_USHIFT) & method_entry_id) == method_entry_id;
}

static bool valid_method_entry(VALUE object) {
  return rb_objspace_internal_object_p(object) && RB_BUILTIN_TYPE(object) == RUBY_T_IMEMO && RB_TYPE_P(object, RUBY_T_IMEMO) && is_method_entry(object);
}

static bool found_target_method_entry(rb_method_entry_t *method_entry, find_data_t *find_data) {
  VALUE method_klass = method_entry->defined_class ? method_entry->defined_class : method_entry->owner;
  return method_klass == find_data->target_klass && method_entry->called_id == find_data->target_id;
}
