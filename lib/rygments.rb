require File.expand_path(File.join(File.dirname(__FILE__), "..", "ext", "wrapper"))

module Rygments
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

  private

  # The full filename of the Python helper script
  HELPER_PATH = File.expand_path(File.join(File.dirname(__FILE__), "rygments"))

  # This function maintains a single instance of the Python wrapper 
  # extension.
  def self.wrapper
    @wrapper ||= Rygments::Wrapper.new(HELPER_PATH)
  end
end
