require 'bundler'
Bundler::GemHelper.install_tasks

require 'rake/extensiontask'
Rake::ExtensionTask.new do |ext|
  ext.name = 'wrapper'
  ext.ext_dir = 'ext'
end

require 'rspec/core/rake_task'
RSpec::Core::RakeTask.new(:spec) do |t|
  t.rspec_opts = ['--options', 'spec/spec.opts']
end

task :spec => :compile
task :default => :spec
