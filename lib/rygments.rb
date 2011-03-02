require "wrapper"

# = Introduction
#
# Rygments is a Ruby wrapper for the awesome Python based Pygments syntax
# highlighter.
#
# Rygments differs from other implementations in that it embeds a full
# Python interpreter for the Pygments Python files as a Ruby C extension.
# This enables Rygments to reach a level of performance that is equal to
# invoking Pygments from a native Python application without forking
# a new process. This difference in performance really pays off when
# you are highlighting lots of smaller code fragments, e.g. for a blog
# post.
#
# = Usage
#
# Usage is easy. Rygments comes with two class methods that enable you
# to highlight code from wherever you want.
#
# To highlight source code from a string use:
#
#   Rygments.highlight_string(source_code, lexer, formatter)
#
# and to highlight source code from a file use:
#
#   Rygments.highlight_file(filename, lexer, formatter)
#
# Both functions return a string containing the highlighted code.
#
# The _lexer_ is a string specifying the programming language you want
# to highlight for, e.g. +ruby+ or +sass+.
#
# The _formatter_ is a string specifying the kind of output you want, 
# e.g. +html+ or +latex+.
#
# A full list of lexers and formatters is available on the Pygments
# website.
#
# Author:: Emil Loer (http://emilloer.com)
# License:: Licensed under the terms of the MIT license
module Rygments
  # The full filename of the Python helper script
  HELPER_PATH = File.expand_path(File.join(File.dirname(__FILE__), "rygments"))

  # Perform syntax highlighting on a string.
  # Returns the highlighted string.
  def self.highlight_string(code, lexer = "ruby", formatter = "html")
    wrapper.highlight_string code, lexer, formatter
  end
  
  # Reads a file and performs syntax highlighting on the file's contents.
  # Returns the highlighted string.
  def self.highlight_file(filename, lexer = "ruby", formatter = "html")
    wrapper.highlight_file filename, lexer, formatter
  end

  # This function maintains a single instance of the Python wrapper 
  # extension.
  def self.wrapper
    @wrapper ||= Rygments::Wrapper.new(HELPER_PATH)
  end
end
