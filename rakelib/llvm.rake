LLVM_VERSION    = '3.3'
LLVM_DIR        = "#{VENDOR_DIR}/llvm"
LLVM_TOOLS_PATH = "#{LLVM_DIR}/tools"
LLVM_BUILD_DIR  = "#{BUILD_DIR}/llvm"
LLVM_LIB        = "#{BUILD_DIR}/libllvm.a"
LLVM_LIBS_PATH  = "#{LLVM_DIR}/Release+Asserts/lib"

LIBCLANG_HEADER = "#{LLVM_TOOLS_PATH}/clang/include/clang-c/Index.h"

namespace :llvm do
  desc 'Removes the llvm/clang sources'
  task :remove do
    FileUtils.rm_f(LLVM_DIR)
  end

  desc "Downloads the llvm/clang #{LLVM_VERSION} sources"
  task :download => VENDOR_DIR do
    Dir.chdir(VENDOR_DIR) do
      path = "llvm-#{LLVM_VERSION}.src.tar.gz"

      RakeCompile.log('DOWNLOAD'.red, "llvm-#{LLVM_VERSION}")
      sh("curl -o '#{path}' 'http://llvm.org/releases/#{LLVM_VERSION}/#{path}'")
      sh("tar -xzf '#{path}'")

      FileUtils.rm_f(path)
      FileUtils.mv("llvm-#{LLVM_VERSION}.src", 'llvm')
    end

    Dir.chdir(LLVM_TOOLS_PATH) do
      path = "cfe-#{LLVM_VERSION}.src.tar.gz"

      RakeCompile.log('DOWNLOAD'.red, "cfe-#{LLVM_VERSION}")
      sh("curl -o '#{path}' 'http://llvm.org/releases/#{LLVM_VERSION}/#{path}'")
      sh("tar -xzf '#{path}'")

      FileUtils.rm_f(path)
      FileUtils.mv("cfe-#{LLVM_VERSION}.src", 'clang')
    end
  end

  desc "Build the llvm/clang #{LLVM_VERSION} sources"
  task :build => [LLVM_DIR, LIBCLANG_HEADER] do
    Dir.chdir(LLVM_DIR) do
      RakeCompile.log('BUILD'.red, "llvm-#{LLVM_VERSION}")
      sh('./configure --enable-optimized --enable-jit')
      sh('make -j4') # this is totally bogus
    end
  end

  desc "Build the llvm/clang #{LLVM_VERSION} objects into a static lib"
  task :build_lib do
    FileUtils.rm_f(LLVM_LIB)
    Rake::Task[LLVM_LIB].invoke
  end
end

directory LLVM_BUILD_DIR
CLEAN.include(LLVM_LIB)

file LIBCLANG_HEADER => 'llvm:download'
file "#{LLVM_LIBS_PATH}/libclang.a" => 'llvm:build'

file LLVM_LIB => ["#{LLVM_LIBS_PATH}/libclang.a", LLVM_BUILD_DIR] do
  # - break up all the llvm static libs we need into objects
  # - recombine them into one, for easier linking

  RakeCompile.log('LIB'.light_blue, LLVM_LIB)
  libs = FileList["#{LLVM_LIBS_PATH}/libclang*.a", "#{LLVM_LIBS_PATH}/libLLVM*.a"]
  libs = libs.collect { |x| File.absolute_path(x) }

  FileUtils.rm_rf(Dir.glob("#{LLVM_BUILD_DIR}/*"))
  Dir.chdir(LLVM_BUILD_DIR) do
    libs.each do |lib|
      lib_dir = File.basename(lib, ".a")
      FileUtils.mkdir(lib_dir)
      Dir.chdir(lib_dir) do
        Rake::sh("ar -x #{lib}")
      end
    end
  end

  Rake::sh("ar -crs '#{LLVM_LIB}' #{LLVM_BUILD_DIR}/**/*.o")
end
