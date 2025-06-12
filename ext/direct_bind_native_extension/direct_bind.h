#pragma once

#include <stdbool.h>
#include <ruby/internal/method.h>

typedef struct {
  bool ok;
  const char *failure_reason;
  int arity;
  VALUE (*func)(ANYARGS);
} direct_bind_cfunc_result;

// Recommended to call once during your gem's initialization, to validate that direct-bind's Ruby hacking is in good shape.
bool direct_bind_self_test(bool raise_on_failure);

// Provides the reverse of `rb_define_method`: Given a class and a method_name, retrieves the arity and func previously
// passed to `rb_define_method`.
//
// Performance note: As of this writing, this method scans objspace to find the definition of the method, so you
// most probably want to cache its result, rather than calling it very often.
direct_bind_cfunc_result direct_bind_get_cfunc(VALUE klass, ID method_name, bool raise_on_failure);
