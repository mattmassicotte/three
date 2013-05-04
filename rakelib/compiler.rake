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
file "#{BUILD_DIR}/lang" => ["#{BUILD_DIR}/Compiler.o", "#{BUILD_DIR}/Token.o", "#{BUILD_DIR}/Lexer.o", BUILD_DIR] do
  inputs = ["#{BUILD_DIR}/Token.o", "#{BUILD_DIR}/Compiler.o", "#{BUILD_DIR}/Lexer.o"]
  executable(inputs, "#{BUILD_DIR}/lang")
end

directory "#{BUILD_DIR}/tests"
file 'tests/LexerTests.cpp'
CLEAN.include("#{BUILD_DIR}/tests/LexerTests.o")
file "#{BUILD_DIR}/tests/LexerTests.o" => ['tests/LexerTests.cpp', 'compiler/Lexer.h', 'compiler/Token.h', "#{BUILD_DIR}/tests"] do
  compile('tests/LexerTests.cpp', "#{BUILD_DIR}/tests/LexerTests.o", '-I. -Igtest/include')
end

CLOBBER.include("#{BUILD_DIR}/compiler_test")
object_files = ["#{BUILD_DIR}/Token.o", "#{BUILD_DIR}/Lexer.o", "#{BUILD_DIR}/tests/LexerTests.o", "#{BUILD_DIR}/gtest/gtest_main.o", "#{BUILD_DIR}/gtest/gtest-all.o"]
file "#{BUILD_DIR}/compiler_test" => object_files do
  executable(object_files, "#{BUILD_DIR}/compiler_test")
end

task 'compiler:test' => "#{BUILD_DIR}/compiler_test" do
  sh("#{BUILD_DIR}/compiler_test")
end
