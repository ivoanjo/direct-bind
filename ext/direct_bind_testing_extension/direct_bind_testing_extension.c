// direct-bind: Ruby gem for getting direct access to function pointers
// Copyright (c) 2025 Ivo Anjo <ivo@ivoanjo.me>
//
// This file is part of direct-bind.
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "direct-bind.h"

VALUE direct_bind_call(VALUE _self, VALUE klass, VALUE method, VALUE instance);
VALUE direct_bind_test_arity(VALUE _self, VALUE klass, VALUE method, VALUE arity);

void Init_direct_bind_testing_extension(void) {
  VALUE direct_bind_module = rb_define_module("DirectBindTesting");

  direct_bind_initialize(direct_bind_module, true);

  rb_define_singleton_method(direct_bind_module, "call", direct_bind_call, 3);
  rb_define_singleton_method(direct_bind_module, "test_arity", direct_bind_test_arity, 3);
}

VALUE direct_bind_call(__attribute__((unused)) VALUE _self, VALUE klass, VALUE method, VALUE instance) {
  direct_bind_cfunc_result result = direct_bind_get_cfunc_with_arity(klass, SYM2ID(method), 0, true);
  return ((VALUE (*)(VALUE)) result.func)(instance);
}

VALUE direct_bind_test_arity(__attribute__((unused)) VALUE _self, VALUE klass, VALUE method, VALUE arity) {
  direct_bind_get_cfunc_with_arity(klass, SYM2ID(method), FIX2INT(arity), true);
  return Qtrue;
}
