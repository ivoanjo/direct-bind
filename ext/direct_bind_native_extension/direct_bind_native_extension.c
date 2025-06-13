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

#include "direct_bind.h"

VALUE direct_bind_call(VALUE _self, VALUE klass, VALUE method, VALUE instance);

void Init_direct_bind_native_extension(void) {
  direct_bind_self_test(true);

  VALUE direct_bind_module = rb_define_module("DirectBind");
  rb_define_singleton_method(direct_bind_module, "call", direct_bind_call, 3);
}

VALUE direct_bind_call(__attribute__((unused)) VALUE _self, VALUE klass, VALUE method, VALUE instance) {
  direct_bind_cfunc_result result = direct_bind_get_cfunc(klass, SYM2ID(method), true);
  if (result.arity != 0) rb_raise(rb_eArgError, "Unexpected arity on cfunc: %d", result.arity);
  return ((VALUE (*)(VALUE)) result.func)(instance);
}
