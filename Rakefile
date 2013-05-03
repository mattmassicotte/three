# Rakefile
require 'rake/clean'

verbose(false)

BUILD_DIR = "build"

# build functions
def compile(input, output, opts=nil)
  puts("Compile: #{output}")
  sh("clang++ -std=c++0x #{opts} -o '#{output}' -c #{input}")
end

def executable(inputs, output)
  puts("   Link: #{output}")
  inputs = inputs.join(" ")
  sh("clang++ -std=c++0x -o '#{output}' #{inputs}")
end

def library(inputs, output)
  rm_f(output)
  
  puts("Library: #{output}")
  inputs = inputs.join(" ")
  sh("/usr/bin/ar rs '#{output}' #{inputs}")
end

CLOBBER.include(BUILD_DIR)
directory BUILD_DIR

file 'compiler/Token.cpp'
file 'compiler/Token.h'
CLEAN.include("#{BUILD_DIR}/Token.o")
file "#{BUILD_DIR}/Token.o" => ['compiler/Token.cpp', 'compiler/Token.h', BUILD_DIR] do
  compile('compiler/Token.cpp', "#{BUILD_DIR}/Token.o")
end

file 'compiler/Lexer.cpp'
file 'compiler/Lexer.h'
CLEAN.include("#{BUILD_DIR}/Lexer.o")
file "#{BUILD_DIR}/Lexer.o" => ['compiler/Lexer.cpp', 'compiler/Lexer.h', 'compiler/Token.h', BUILD_DIR] do
  compile('compiler/Lexer.cpp', "#{BUILD_DIR}/Lexer.o")
end

file 'compiler/compiler.cpp'
CLEAN.include("#{BUILD_DIR}/Compiler.o")
file "#{BUILD_DIR}/Compiler.o" => ['compiler/compiler.cpp', BUILD_DIR] do
  compile('compiler/compiler.cpp', "#{BUILD_DIR}/Compiler.o")
end

CLOBBER.include("#{BUILD_DIR}/lang")
task :default => "#{BUILD_DIR}/lang"
file "#{BUILD_DIR}/lang" => ["#{BUILD_DIR}/Compiler.o", "#{BUILD_DIR}/Token.o", "#{BUILD_DIR}/Lexer.o", BUILD_DIR] do
  inputs = ["#{BUILD_DIR}/Token.o", "#{BUILD_DIR}/Compiler.o", "#{BUILD_DIR}/Lexer.o"]
  executable(inputs, "#{BUILD_DIR}/lang")
end

# gtest stuff
namespace :gtest do
  desc 'Installs the C++ unit testing package gtest'
  task :install do
    FileUtils.rm_f('gtest')
    FileUtils.rm_f('gtest-1.6.0.zip')
    FileUtils.rm_f('gtest-1.6.0')
  
    sh('curl -o gtest-1.6.0.zip http://googletest.googlecode.com/files/gtest-1.6.0.zip')
    sh('unzip gtest-1.6.0.zip')
    FileUtils.rm_f('gtest-1.6.0.zip')
    FileUtils.mv('gtest-1.6.0', 'gtest')
  end
  
  desc 'Builds the gtest binaries'
  task :build => ["#{BUILD_DIR}/gtest/libgtest.a", "#{BUILD_DIR}/gtest/libgtest_main.a"]
end

file 'gtest/src/gtest_main.cc' do
  Rake::Task['gtest:install'].invoke
end

file 'gtest/src/gtest-all.cc' do
  Rake::Task['gtest:install'].invoke
end

directory "#{BUILD_DIR}/gtest"
CLEAN.include("#{BUILD_DIR}/gtest/gtest-all.o")
file "#{BUILD_DIR}/gtest/gtest-all.o" => ['gtest/src/gtest-all.cc', "#{BUILD_DIR}/gtest"] do
  compile('gtest/src/gtest-all.cc', "#{BUILD_DIR}/gtest/gtest-all.o", '-Igtest -Igtest/include')
end

CLEAN.include("#{BUILD_DIR}/gtest/gtest_main.o")
file "#{BUILD_DIR}/gtest/gtest_main.o" => ['gtest/src/gtest_main.cc', "#{BUILD_DIR}/gtest"] do
  compile('gtest/src/gtest_main.cc', "#{BUILD_DIR}/gtest/gtest_main.o", '-Igtest -Igtest/include')
end

CLOBBER.include("#{BUILD_DIR}/gtest/libgtest.a")
file "#{BUILD_DIR}/gtest/libgtest.a" => ["#{BUILD_DIR}/gtest/gtest-all.o"] do
  library(["#{BUILD_DIR}/gtest/gtest-all.o"], "#{BUILD_DIR}/gtest/libgtest.a")
end

CLOBBER.include("#{BUILD_DIR}/gtest/libgtest_main.a")
file "#{BUILD_DIR}/gtest/libgtest_main.a" => ["#{BUILD_DIR}/gtest/gtest_main.o", "#{BUILD_DIR}/gtest"] do
  library(["#{BUILD_DIR}/gtest/gtest_main.o"], "#{BUILD_DIR}/gtest/libgtest_main.a")
end
