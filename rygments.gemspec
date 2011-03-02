# -*- encoding: utf-8 -*-
$:.push File.expand_path("../lib", __FILE__)
require "rygments/version"

Gem::Specification.new do |s|
  s.name        = "rygments"
  s.version     = Rygments::VERSION
  s.platform    = Gem::Platform::RUBY
  s.authors     = ["Emil Loer"]
  s.email       = ["emil@koffietijd.net"]
  s.homepage    = "https://github.com/thedjinn/rygments"
  s.summary     = %q{Rygments is a Ruby wrapper for Pygments}
  s.description = %q{Rygments is a Ruby wrapper for the Pygments syntax highlighter. It uses an embedded Python interpreter to get high processing throughput.}

  s.rubyforge_project = "rygments"

  s.files         = `git ls-files`.split("\n")
  s.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  s.executables   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  s.require_paths = ["lib"]

  s.add_development_dependency("rspec", ["~> 2.0"])
  s.add_development_dependency("rake-compiler", ["~> 0.7"])

  s.has_rdoc = true
  s.extensions = ["ext/extconf.rb"]
end
