SPEC_DIR = File.dirname(__FILE__)
lib_path = File.expand_path(File.join(SPEC_DIR, "..", "lib"))
$LOAD_PATH.unshift lib_path unless $LOAD_PATH.include?(lib_path)

require "rygments"
require "wrapper"
