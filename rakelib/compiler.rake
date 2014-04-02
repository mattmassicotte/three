COMPILER_SOURCES = FileList['compiler/**/*.cpp']
COMPILER_PCH = 'compiler/compiler_prefix.hpp'
COMPILER_LIB = "#{BUILD_DIR}/libthree_compiler.a"

COMPILER_TEST_SOURCES = FileList['tests/**/*.cpp']
COMPILER_TEST_SOURCES.exclude('tests/CheckerTests/*.cpp')
COMPILER_TEST_PCH = 'tests/test_prefix.hpp'
COMPILER_TEST_BIN = "#{BUILD_DIR}/three_compiler_test"

FRONTEND_SOURCES = FileList['frontend/**/*.cpp']
FRONTEND_BIN = "#{BUILD_DIR}/three"

# These dependencies are defined here, in this way, for two reasons:
# - these tasks cannot even be defined without first downloading llvm and gtest
# - the constants are in .rake files, and the load order of those isn't well-defined

namespace :compiler do
  task :define_tasks => 'gtest:define_tasks' do
    Rake::Task[GTEST_HEADER].invoke
    Rake::Task[LIBCLANG_HEADER].invoke

    # Its quite inefficient to invoke the llvm lib task here.  Especially since
    # it will be invoked implicitly by the tasks defined below.  The problem is
    # that task actually involves mutating process-local state (the current working directory).
    # Changing the working directory will cause all the tasks below to fail.
    #Rake::Task[LLVM_LIB].invoke

    pch COMPILER_PCH
    cpp_flags "-I#{LIBCLANG_INCLUDE_PATH}"
    static_library COMPILER_LIB do |target|
      target.add_objects_from_sources COMPILER_SOURCES
    end

    pch COMPILER_TEST_PCH
    cpp_flags "-I#{GTEST_INCLUDE_DIR} -DGTEST_HAS_TR1_TUPLE=0"
    link_library COMPILER_LIB
    link_library GTEST_LIB
    link_library LLVM_LIB
    ld_flags '-lpthread -ldl'
    executable COMPILER_TEST_BIN do |target|
      target.add_objects_from_sources COMPILER_TEST_SOURCES
    end

    link_library COMPILER_LIB
    link_library LLVM_LIB
    ld_flags '-lpthread -ldl'
    executable FRONTEND_BIN do |target|
      target.add_objects_from_sources FRONTEND_SOURCES
    end
  end

  desc "Build the compiler library"
  task :build => :define_tasks do
    Rake::Task[COMPILER_LIB].invoke
  end

  desc "Run the compiler tests"
  task :test => :define_tasks do
    Rake::Task[COMPILER_TEST_BIN].invoke
    Rake::sh("#{COMPILER_TEST_BIN} --gtest_catch_exceptions=0")
  end

  desc "Build the compiler frontend"
  task :frontend => :define_tasks do
    Rake::Task[FRONTEND_BIN].invoke
  end

  desc "Install the compiler"
  task :install => [:build, :frontend, THREE_LIB_DIR] do
    RakeCompile.install(FRONTEND_BIN, File.join(THREE_BIN_DIR, 'three'))
    RakeCompile.install(FRONTEND_BIN, File.join(THREE_LIB_DIR, 'libthree_compiler.a'))
  end

  desc "Uninstall the compiler"
  task :uninstall do
    RakeCompile.uninstall(File.join(THREE_BIN_DIR, 'three'))
    RakeCompile.uninstall(File.join(THREE_LIB_DIR, 'libthree_compiler.a'))
  end
end
