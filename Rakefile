# Rakefile
require 'rake/clean'
require_relative 'rakelib/build_functions'

verbose(false) # keep the sh command quiet

BUILD_DIR           = 'build'
FRONTEND_EXECUTABLE = "#{BUILD_DIR}/three"
RUNTIME_LIB         = "#{BUILD_DIR}/libthree_runtime.a"
COMPILER_LIB        = "#{BUILD_DIR}/libthree_compiler.a"

THREE_LIB_DIR     = '/usr/local/lib'
THREE_BIN_DIR     = '/usr/local/bin'
THREE_INCLUDE_DIR = '/usr/local/include/three'

RAKE_FILES = FileList['Rakefile', 'rakelib/**/*.rake']

CLOBBER.include(BUILD_DIR)
directory BUILD_DIR
directory THREE_INCLUDE_DIR
directory THREE_LIB_DIR

task :default => 'compiler:test'
task :build => ['compiler:test', 'compiler:frontend', 'runtime:build']
task :install => ['compiler:install', 'runtime:install']
task :uninstall => ['compiler:uninstall', 'runtime:uninstall']
task :reinstall => :uninstall do
  Rake::task['install']
end
