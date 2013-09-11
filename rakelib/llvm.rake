LLVM_VERSION = "3.3"
LLVM_PATH = "llvm-#{LLVM_VERSION}.src"
LLVM_COMPRESSED_PATH = "#{LLVM_PATH}.tar.gz"
LLVM_TOOLS_PATH = "#{LLVM_PATH}/tools"

CLANG_SRC = "cfe-#{LLVM_VERSION}.src"
CLANG_PATH = "#{LLVM_TOOLS_PATH}/clang"

def llvm_download_and_expand(path)
  compressed_path = "#{path}.tar.gz"
  BuildFunctions.log('download', path)
  sh("curl -o \"#{compressed_path}\" \"http://llvm.org/releases/#{LLVM_VERSION}/#{compressed_path}\"")
  sh("tar -xzf \"#{compressed_path}\"")
  FileUtils.rm_f(compressed_path)
end

namespace :llvm do
  file LLVM_PATH do
    llvm_download_and_expand(LLVM_PATH)
  end

  file CLANG_PATH => LLVM_PATH do
    Dir.chdir(LLVM_TOOLS_PATH) do
      llvm_download_and_expand(CLANG_SRC)
    end

    FileUtils.mv("#{LLVM_TOOLS_PATH}/#{CLANG_SRC}", CLANG_PATH)
  end

  desc "Build the LLVM and Clang sources"
  task :build => [LLVM_PATH, CLANG_PATH] do
    Dir.chdir(LLVM_PATH) do
      BuildFunctions.log('configure', 'llvm')
      sh("./configure --enable-optimized --enable-jit")
      BuildFunctions.log('make', 'llvm')
      sh("make -j#{BuildFunctions.optimal_parallelism}")
      sh('make install')
    end
  end
end

file "/usr/local/include/clang-c/Index.h" => 'llvm:build'
file "/usr/local/lib/libclang.a" => 'llvm:build'
