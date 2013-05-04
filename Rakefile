# Rakefile
require 'rake/clean'
require_relative 'rakelib/build_functions'

verbose(false) # keep the sh command quiet

BUILD_DIR  = "build"
RAKE_FILES = FileList['Rakefile', 'rakelib/**/*.rake']

CLOBBER.include(BUILD_DIR)
directory BUILD_DIR

task :default => 'compiler:test'
