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
require "direct_bind/rspec_helper"

RSpec.describe DirectBind::RSpecHelper do
  context "when native extension is using the latest direct-bind version" do
    it do
      expect {
        DirectBind::RSpecHelper.expect_direct_bind_version_to_be_up_to_date_in(DirectBind::ExtensionVersion)
      }.to_not raise_error
    end
  end

  context "when native extension is not using the latest direct-bind version" do
    before { stub_const("DirectBind::ExtensionVersion::DirectBind::VERSION", "incorrect_version") }

    it do
      expect {
        DirectBind::RSpecHelper.expect_direct_bind_version_to_be_up_to_date_in(DirectBind::ExtensionVersion)
      }.to raise_error(RSpec::Expectations::ExpectationNotMetError)
    end
  end
end
