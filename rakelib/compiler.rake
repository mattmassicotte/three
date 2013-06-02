# Compiler rakefile

COMPILER_SOURCES = FileList['compiler/**/*.cpp']
COMPILER_SOURCES.exclude('compiler/compiler.cpp')

COMPILER_GTEST_CC_FLAGS = '-I. -Igtest/include -DGTEST_HAS_TR1_TUPLE=0'

object_files = []
COMPILER_SOURCES.each do |source|
  # define the path
  object_dir  = BuildFunctions::absolute_path(File.dirname(source))
  object_path = BuildFunctions::absolute_path(File.basename(source, '.*') + '.o')

  # define a dependency on the object path
  directory(object_dir)

  object_files << object_path

  # make sure to clean this object
  CLEAN.include(object_path)

  # setup object file dependencies
  dependencies = BuildFunctions::get_dependencies(source)
  dependencies << __FILE__
  dependencies << object_dir

  # and finally, setup the rule to build this object
  file(object_path => dependencies) do
    BuildFunctions::compile(source, object_path)
  end
end

directory "#{BUILD_DIR}/tests"

file 'tests/LexerTests.cpp'
CLEAN.include("#{BUILD_DIR}/tests/LexerTests.o")
file "#{BUILD_DIR}/tests/LexerTests.o" => ['tests/LexerTests.cpp', 'compiler/Lexer.h', 'compiler/Token.h', "#{BUILD_DIR}/tests"] do
  BuildFunctions::compile('tests/LexerTests.cpp', "#{BUILD_DIR}/tests/LexerTests.o", COMPILER_GTEST_CC_FLAGS)
end

file 'tests/ParserTests.cpp' => ['compiler/Parser.h']
CLEAN.include("#{BUILD_DIR}/tests/ParserTests.o")
file "#{BUILD_DIR}/tests/ParserTests.o" => ['tests/ParserTests.cpp', "#{BUILD_DIR}/tests"] do
  BuildFunctions::compile('tests/ParserTests.cpp', "#{BUILD_DIR}/tests/ParserTests.o", COMPILER_GTEST_CC_FLAGS)
end

file 'tests/CSourceCodeGenTests.cpp' => ['compiler/CodeGen/CSource.h', 'compiler/Constructs/TypeReference.h', 'compiler/Constructs/DataType.h']
CLEAN.include("#{BUILD_DIR}/tests/CSourceCodeGenTests.o")
file "#{BUILD_DIR}/tests/CSourceCodeGenTests.o" => ['tests/CSourceCodeGenTests.cpp', "#{BUILD_DIR}/tests"] do
  BuildFunctions::compile('tests/CSourceCodeGenTests.cpp', "#{BUILD_DIR}/tests/CSourceCodeGenTests.o", COMPILER_GTEST_CC_FLAGS)
end

CLOBBER.include("#{BUILD_DIR}/libthree.a")
file "#{BUILD_DIR}/libthree.a" => object_files do
  BuildFunctions::library(object_files, "#{BUILD_DIR}/libthree.a")
end

CLOBBER.include("#{BUILD_DIR}/compiler_test")
test_object_files = []
test_object_files << "#{BUILD_DIR}/gtest/gtest_main.o"
test_object_files << "#{BUILD_DIR}/gtest/gtest-all.o"
test_object_files << "#{BUILD_DIR}/tests/LexerTests.o"
test_object_files << "#{BUILD_DIR}/tests/ParserTests.o"
test_object_files << "#{BUILD_DIR}/tests/CSourceCodeGenTests.o"
file "#{BUILD_DIR}/compiler_test" => [test_object_files, "#{BUILD_DIR}/libthree.a"].flatten do
  BuildFunctions::executable(test_object_files, "#{BUILD_DIR}/compiler_test", "'-L#{BUILD_DIR}' -lthree")
end

desc "Run the compiler library tests"
task 'compiler:test' => "#{BUILD_DIR}/compiler_test" do
  sh("#{BUILD_DIR}/compiler_test --gtest_catch_exceptions=0")
end
