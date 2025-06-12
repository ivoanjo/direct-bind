#include "direct_bind.h"

#include <ruby.h>

bool direct_bind_self_test(bool raise_on_failure) {
  if (raise_on_failure) rb_raise(rb_eRuntimeError, "direct-bind self-test failed");
  else return false;
}

direct_bind_cfunc_result direct_bind_get_cfunc(VALUE klass, ID method_name, bool raise_on_failure) {
  if (raise_on_failure) rb_raise(rb_eRuntimeError, "direct-bind not implemented");
  return (direct_bind_cfunc_result) {.ok = false, .failure_reason = "not implemented"};
}
