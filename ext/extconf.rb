require "mkmf"

extension_name = "wrapper"

dir_config(extension_name)

$CFLAGS << " #{`python-config --cflags`.chomp} -Wno-strict-prototypes"
$LDFLAGS << " #{`python-config --ldflags`.chomp}"

# python-config adds arch options for ppc and x64 on Darwin. Ruby 1.9
# does not like this, so we'll have to remove them.
if RUBY_VERSION.start_with? "1.9" and RUBY_PLATFORM =~ /darwin/
  $CFLAGS.gsub! /\s+-arch\s+\w+/, " "
end

create_makefile(extension_name)
