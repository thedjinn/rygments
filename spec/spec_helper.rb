SPEC_DIR = File.dirname(__FILE__)
lib_path = File.expand_path(File.join(SPEC_DIR, "..", "lib"))
ext_path = File.expand_path(File.join(SPEC_DIR, "..", "ext"))
$LOAD_PATH.unshift lib_path unless $LOAD_PATH.include?(lib_path)
$LOAD_PATH.unshift ext_path unless $LOAD_PATH.include?(ext_path)

require "rygments"
require "wrapper"
