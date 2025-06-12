#include "direct_bind.h"

#include <ruby.h>

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

direct_bind_cfunc_result direct_bind_get_cfunc(VALUE klass, ID method_name, bool raise_on_failure) {
  if (raise_on_failure) rb_raise(rb_eRuntimeError, "direct-bind not implemented");
  return (direct_bind_cfunc_result) {.ok = false, .failure_reason = "not implemented"};
}
