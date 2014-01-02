# Rakefile
require 'rake/clean'
require 'rake-compile'

verbose(false) # keep the sh command quiet

# main artifact locations
BUILD_DIR         = File.absolute_path('build')
VENDOR_DIR        = File.absolute_path('vendor')
THREE_LIB_DIR     = '/usr/local/lib'
THREE_BIN_DIR     = '/usr/local/bin'
THREE_INCLUDE_DIR = '/usr/local/include/three'

#RAKE_FILES = FileList['Rakefile', 'rakelib/**/*.rake']

# tasks for all artifact locations
directory BUILD_DIR
directory VENDOR_DIR
directory THREE_LIB_DIR
directory THREE_BIN_DIR
directory THREE_INCLUDE_DIR

build_directory BUILD_DIR

# define the default cpp flags for rake-compile
base_cpp_flags("-std=c++11 -I#{File.dirname(__FILE__)}")
base_cc_flags("-std=c11 -I#{File.dirname(__FILE__)}")

task :default => 'compiler:test'
task :install => ['compiler:install', 'runtime:install']
task :uninstall => ['compiler:uninstall', 'runtime:uninstall']
