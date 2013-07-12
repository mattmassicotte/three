# Compiler rakefile

COMPILER_CC_FLAGS = '-I.'
COMPILER_GTEST_CC_FLAGS = '-I. -Igtest/include -DGTEST_HAS_TR1_TUPLE=0'

COMPILER_SOURCES = FileList['compiler/**/*.cpp']
COMPILER_SOURCES.exclude('compiler/compiler.cpp')

COMPILER_OBJECTS = BuildFunctions::objects_for_sources(COMPILER_SOURCES, COMPILER_CC_FLAGS)

directory "#{BUILD_DIR}/tests"

CLOBBER.include(COMPILER_LIB)
file COMPILER_LIB => COMPILER_OBJECTS do
  BuildFunctions::library(COMPILER_OBJECTS, COMPILER_LIB)
end

COMPILER_TEST_SOURCES = FileList['tests/**/*.cpp']
COMPILER_TEST_OBJECTS = BuildFunctions::objects_for_sources(COMPILER_TEST_SOURCES, COMPILER_GTEST_CC_FLAGS)

CLOBBER.include("#{BUILD_DIR}/compiler_test")
test_object_files = []
test_object_files << "#{BUILD_DIR}/gtest/gtest_main.o"
test_object_files << "#{BUILD_DIR}/gtest/gtest-all.o"
test_object_files << COMPILER_TEST_OBJECTS
file "#{BUILD_DIR}/compiler_test" => [test_object_files, COMPILER_LIB].flatten do
  BuildFunctions::executable(test_object_files, "#{BUILD_DIR}/compiler_test", "'-L#{BUILD_DIR}' -lthree_compiler")
end

namespace :compiler do
  desc "Run the compiler library tests"
  task :test => "#{BUILD_DIR}/compiler_test" do
    sh("#{BUILD_DIR}/compiler_test --gtest_catch_exceptions=0")
  end

  desc "Install the compiler and frontend"
  task :install => [FRONTEND_EXECUTABLE, COMPILER_LIB, THREE_LIB_DIR, THREE_BIN_DIR] do
    FileUtils.cp(COMPILER_LIB, File.join(THREE_LIB_DIR, 'libthree_compiler.a'))
    FileUtils.cp(FRONTEND_EXECUTABLE, File.join(THREE_BIN_DIR, 'three'))
  end

  desc "Uninstall the compiler and frontend"
  task :uninstall do
    FileUtils.rm_f(File.join(THREE_LIB_DIR, 'libthree_compiler.a'))
    FileUtils.rm_f(File.join(THREE_BIN_DIR, 'three'))
    FileUtils.rm_rf(THREE_INCLUDE_DIR)
  end
end
