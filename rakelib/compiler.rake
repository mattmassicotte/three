# Compiler rakefile

COMPILER_CC_FLAGS = '-I.'
COMPILER_GTEST_CC_FLAGS = '-I. -Igtest/include -DGTEST_HAS_TR1_TUPLE=0'
GTEST_HEADER = 'gtest/include/gtest/gtest.h'

COMPILER_SOURCES = FileList['compiler/**/*.cpp']
COMPILER_SOURCES.exclude('compiler/compiler.cpp')

COMPILER_OBJECTS = BuildFunctions::objects_for_sources(COMPILER_SOURCES, :flags => COMPILER_CC_FLAGS)

directory "#{BUILD_DIR}/tests"

CLOBBER.include(COMPILER_LIB)
file COMPILER_LIB => COMPILER_OBJECTS do
  BuildFunctions::library(COMPILER_OBJECTS, COMPILER_LIB)
end

COMPILER_TEST_SOURCES = FileList['tests/**/*.cpp']
# Handle bootstrapping the first rake invocation, when gtest is install installed.  This
# is messy, because these rake task *definitions* depend on gtest.
find_deps = File.exist?(File.join(File.dirname(__FILE__), "../#{GTEST_HEADER}"))
opts = {:flags => COMPILER_GTEST_CC_FLAGS, :find_deps => find_deps}
COMPILER_TEST_OBJECTS = BuildFunctions::objects_for_sources(COMPILER_TEST_SOURCES, opts)

CLOBBER.include("#{BUILD_DIR}/compiler_test")
test_object_files = []
test_object_files << "#{BUILD_DIR}/gtest/gtest_main.o"
test_object_files << "#{BUILD_DIR}/gtest/gtest-all.o"
test_object_files << COMPILER_TEST_OBJECTS
file "#{BUILD_DIR}/compiler_test" => [test_object_files, COMPILER_LIB].flatten do
  BuildFunctions::executable(test_object_files, "#{BUILD_DIR}/compiler_test", "'-L#{BUILD_DIR}' -L/usr/local/lib -lthree_compiler -lclang")
end

namespace :compiler do
  desc "Run the compiler library tests"
  task :test => "#{BUILD_DIR}/compiler_test" do
    sh("#{BUILD_DIR}/compiler_test --gtest_catch_exceptions=0")
  end

  desc "Install the compiler and frontend"
  task :install => [FRONTEND_EXECUTABLE, COMPILER_LIB, THREE_LIB_DIR, THREE_BIN_DIR, :test] do
    BuildFunctions::install(COMPILER_LIB, File.join(THREE_LIB_DIR, 'libthree_compiler.a'))
    BuildFunctions::install(FRONTEND_EXECUTABLE, File.join(THREE_BIN_DIR, 'three'))
  end

  desc "Uninstall the compiler and frontend"
  task :uninstall do
    FileUtils.rm_f(File.join(THREE_LIB_DIR, 'libthree_compiler.a'))
    FileUtils.rm_f(File.join(THREE_BIN_DIR, 'three'))
    FileUtils.rm_rf(THREE_INCLUDE_DIR)
  end
end
