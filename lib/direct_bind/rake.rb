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

require "rake"
require "rake/tasklib"

module DirectBind
  module Rake
    class InstallTask < ::Rake::TaskLib
      DIRECT_BIND_SOURCES = ["direct-bind.h", "direct-bind.c"]
      DIRECT_BIND_SOURCES_PATH = File.join(Gem.loaded_specs["direct-bind"].full_gem_path, "dist")

      def initialize(extension_name)
        target_extension_path = File.join(Dir.pwd, "ext", extension_name)

        desc "Install direct_bind files into extension"
        task(:"direct-bind:install") do
          DIRECT_BIND_SOURCES.each do |file_name|
            from_path = File.join(DIRECT_BIND_SOURCES_PATH, file_name)
            to_path = File.join(target_extension_path, file_name)

            FileUtils.cp(from_path, to_path) unless already_up_to_date?(from_path, to_path)
          end
        end
      end

      private

      def already_up_to_date?(file1, file2) # Order doesn't really matter here
        File.exist?(file1) && File.exist?(file2) && FileUtils.compare_file(file1, file2)
      end
    end
  end
end
