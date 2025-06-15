# direct-bind: Ruby gem for getting direct access to function pointers
# Copyright (c) 2025 Ivo Anjo <ivo@ivoanjo.me>
#
# This file is part of direct-bind.
#
# MIT License
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# frozen_string_literal: true

require "direct-bind"
require "direct_bind_testing_extension"

RSpec.describe DirectBindTesting do
  let(:cloned_thread_class) { Thread.dup }
  let(:test_thread) { Thread.new {}.tap { |it| it.name = "test_thread" } }

  describe "method binding" do
    it "can direct bind Thread#name" do
      expect(subject.call(cloned_thread_class, :name, test_thread)).to eq "test_thread"
    end

    context "when method is monkey patched" do
      before do
        cloned_thread_class.class_eval do
          def name
            "monkey_patched!"
          end
        end

        GC.start # Make sure any older definition is collected
      end

      it { expect { subject.call(cloned_thread_class, :name, test_thread) }.to raise_error(RuntimeError, /method_entry is not a cfunc/) }
    end

    context "when method is undefined" do
      before do
        cloned_thread_class.class_eval do
          undef_method :name
        end

        GC.start # Make sure any older definition is collected
      end

      it { expect { subject.call(cloned_thread_class, :name, test_thread) }.to raise_error(RuntimeError, /method_entry is not a cfunc/) }
    end

    context "when method does not exist" do
      it { expect { subject.call(cloned_thread_class, :does_not_exist, test_thread) }.to raise_error(RuntimeError, /not found/) }
    end
  end

  describe "arity validation" do
    let(:arity) { 0 }

    it "validates arity of retrieved function" do
      expect(subject.test_arity(cloned_thread_class, :name, arity)).to be true
    end

    context "when arity is unexpected" do
      let(:arity) { 1 }

      it { expect { subject.test_arity(cloned_thread_class, :name, arity) }.to raise_error(RuntimeError, /unexpected arity/) }
    end
  end
end
