require "rubygems"
require "rygments"

# Highlight a string with some HTML in it
puts Rygments.highlight_string("<div id=\"foo\">bar</div>", "html")

# Highlight this file
puts Rygments.highlight_file(__FILE__)
