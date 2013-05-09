# Compiler rakefile

COMPILER_SOURCES = FileList['compiler/**/*.cpp']
COMPILER_SOURCES.exclude('compiler/compiler.cpp')

compiler_objects = []
COMPILER_SOURCES.each do |source|
  # define the path
  object_dir  = BuildFunctions::absolute_path(File.dirname(source))
  object_path = BuildFunctions::absolute_path(File.basename(source, '.*') + '.o')

  # define a dependency on the object path
  directory(object_dir)

  compiler_objects << object_path

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
  BuildFunctions::compile('tests/LexerTests.cpp', "#{BUILD_DIR}/tests/LexerTests.o", '-I. -Igtest/include')
end

file 'tests/ParserTests.cpp' => ['compiler/Parser.h']
CLEAN.include("#{BUILD_DIR}/tests/ParserTests.o")
file "#{BUILD_DIR}/tests/ParserTests.o" => ['tests/ParserTests.cpp', "#{BUILD_DIR}/tests"] do
  BuildFunctions::compile('tests/ParserTests.cpp', "#{BUILD_DIR}/tests/ParserTests.o", '-I. -Igtest/include')
end

CLOBBER.include("#{BUILD_DIR}/compiler_test")
object_files = compiler_objects.dup()
object_files << "#{BUILD_DIR}/tests/LexerTests.o"
object_files << "#{BUILD_DIR}/tests/ParserTests.o"
object_files << "#{BUILD_DIR}/gtest/gtest_main.o"
object_files << "#{BUILD_DIR}/gtest/gtest-all.o"
file "#{BUILD_DIR}/compiler_test" => object_files do
  BuildFunctions::executable(object_files, "#{BUILD_DIR}/compiler_test")
end

task 'compiler:test' => "#{BUILD_DIR}/compiler_test" do
  sh("#{BUILD_DIR}/compiler_test")
end
