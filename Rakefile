# Rakefile
require 'rake/clean'

verbose(false)

GTEST_PATH = 'gtest/gtest-1.6.0'

def compile(input, output, opts=nil)
  puts("Compile: #{output}")
  sh("clang++ -std=c++0x #{opts} -o #{output} -c #{input}")
end

def executable(inputs, output)
  puts("   Link: #{output}")
  inputs = inputs.join(" ")
  sh("clang++ -std=c++0x -o #{output} #{inputs}")
end

def library(inputs, output)
  rm_f(output)
  
  puts("Library: #{output}")
  inputs = inputs.join(" ")
  sh("/usr/bin/ar rs #{output} #{inputs}")
end

file 'compiler/Token.cpp'
file 'compiler/Token.h'
file 'Token.o' => ['compiler/Token.cpp', 'compiler/Token.h'] do
  compile('compiler/Token.cpp', 'Token.o')
end

file 'compiler/Lexer.cpp'
file 'compiler/Lexer.h'
file 'Lexer.o' => ['compiler/Lexer.cpp', 'compiler/Lexer.h', 'compiler/Token.h'] do
  compile('compiler/Lexer.cpp', 'Lexer.o')
end

file 'compiler/compiler.cpp'
file 'Compiler.o' => ['compiler/compiler.cpp'] do
  compile('compiler/compiler.cpp', 'Compiler.o')
end

file 'lang' => ['Compiler.o', 'Token.o', 'Lexer.o'] do
  inputs = ['Token.o', 'Compiler.o', 'Lexer.o']
  executable(inputs, 'lang')
end

directory GTEST_PATH do
  sh('curl -o gtest/gtest-1.6.0.zip http://googletest.googlecode.com/files/gtest-1.6.0.zip')
  sh('unzip gtest/gtest-1.6.0.zip -d gtest/')
  rm_f('gtest/gtest-1.6.0.zip')
end

file "libgtest.a" => GTEST_PATH do
  compile("#{GTEST_PATH}/src/gtest-all.cc", "gtest-all.o", "-I#{GTEST_PATH} -I#{GTEST_PATH}/include")
  
  library(['gtest-all.o'], "libgtest.a")
end

file "libgtest_main.a" => GTEST_PATH do
  compile("#{GTEST_PATH}/src/gtest_main.cc", "gtest_main.o", "-I#{GTEST_PATH} -I#{GTEST_PATH}/include")
  
  library(['gtest_main.o'], "libgtest_main.a")
end

task :default => 'lang' do
  puts("Build default")
end
