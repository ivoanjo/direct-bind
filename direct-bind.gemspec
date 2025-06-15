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

require_relative "lib/direct_bind/version"

Gem::Specification.new do |spec|
  spec.name = "direct-bind"
  spec.version = DirectBind::VERSION
  spec.authors = ["Ivo Anjo"]
  spec.email = ["ivo@ivoanjo.me"]

  spec.summary = "Ruby gem for getting direct access to function pointers"
  spec.homepage = "https://github.com/ivoanjo/direct-bind"
  spec.license = "MIT"
  spec.required_ruby_version = ">= 2.5.0"

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files = Dir.chdir(__dir__) do
    `git ls-files -z`.split("\x0").reject do |f|
      (f == __FILE__) || f.match(%r{\A(?:(?:bin|test|spec|features|examples)/|\.(?:git|travis|circleci)|appveyor)}) ||
        [".editorconfig", ".ruby-version", ".standard.yml", "gems.rb", "Rakefile"].include?(f)
    end
  end
  spec.require_paths = ["lib"]
end
