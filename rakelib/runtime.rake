RUNTIME_CC_FLAGS = '-I.'

RUNTIME_SOURCES = FileList['runtime/**/*.c']
RUNTIME_HEADERS = FileList['runtime/closure.h', 'runtime/annotations.h', 'runtime/transactional_memory.h', 'runtime/atomic.h', 'runtime/types.h']
C11_HEADERS = FileList['runtime/c11/*.h']

RUNTIME_OBJECTS = BuildFunctions::objects_for_sources(RUNTIME_SOURCES)

RUNTIME_DIR = File.join(THREE_INCLUDE_DIR, 'runtime')
C11_DIR = File.join(THREE_INCLUDE_DIR, 'c11')

directory "#{BUILD_DIR}/runtime"
directory RUNTIME_DIR
directory C11_DIR

CLOBBER.include(RUNTIME_LIB)
file(RUNTIME_LIB => RUNTIME_OBJECTS) do
  BuildFunctions::library(RUNTIME_OBJECTS, RUNTIME_LIB)
end

namespace :runtime do
  desc "Build the runtime library"
  task :build => RUNTIME_LIB

  desc "Install the runtime library and C headers"
  task :install => [RUNTIME_LIB, THREE_LIB_DIR, RUNTIME_DIR, C11_DIR] do
    BuildFunctions::install(RUNTIME_LIB, File.join(THREE_LIB_DIR, 'libthree_runtime.a'))
    RUNTIME_HEADERS.each do |header|
      file = File.basename(header)
      BuildFunctions::install(header, File.join(RUNTIME_DIR, file))
    end

    C11_HEADERS.each do |header|
      file = File.basename(header)
      BuildFunctions::install(header, File.join(C11_DIR, file))
    end

    BuildFunctions::install('runtime/runtime.h', File.join(THREE_INCLUDE_DIR, 'runtime.h'))
  end

  desc "Uninstall the runtime library and C headers"
  task :uninstall do
    FileUtils.rm_rf(THREE_INCLUDE_DIR)
    FileUtils.rm_f(File.join(THREE_LIB_DIR, 'libthree_runtime.a'))
  end
end
