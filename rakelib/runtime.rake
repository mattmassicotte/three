RUNTIME_SOURCES = FileList['runtime/**/*.c']
RUNTIME_HEADERS = FileList['runtime/closure.h', 'runtime/annotations.h', 'runtime/transactional_memory.h', 'runtime/atomic.h', 'runtime/types.h', 'runtime/platform.h']
C11_HEADERS     = FileList['runtime/c11/*.h']
RUNTIME_LIB     = File.join(BUILD_DIR, 'libthree_runtime.a')
INSTALLED_RUNTIME_LIB = File.join(THREE_LIB_DIR, 'libthree_runtime.a')

RUNTIME_DIR = File.join(THREE_INCLUDE_DIR, 'runtime')
C11_DIR = File.join(THREE_INCLUDE_DIR, 'c11')

static_library RUNTIME_LIB do |target|
  target.add_objects_from_sources RUNTIME_SOURCES
end

directory RUNTIME_DIR
directory C11_DIR

namespace :runtime do
  desc "Build the runtime library"
  task :build => RUNTIME_LIB

  desc "Install the runtime library and C headers"
  task :install => [RUNTIME_LIB, THREE_INCLUDE_DIR, THREE_LIB_DIR, RUNTIME_DIR, C11_DIR] do
    RakeCompile.install(RUNTIME_LIB, INSTALLED_RUNTIME_LIB)

    RUNTIME_HEADERS.each do |header|
      file = File.basename(header)
      RakeCompile.install(header, File.join(RUNTIME_DIR, file))
    end

    C11_HEADERS.each do |header|
      file = File.basename(header)
      RakeCompile.install(header, File.join(C11_DIR, file))
    end

    RakeCompile.install('runtime/runtime.h', File.join(THREE_INCLUDE_DIR, 'runtime.h'))
  end

  desc "Uninstall the runtime library and C headers"
  task :uninstall do
    RakeCompile.uninstall(THREE_INCLUDE_DIR)
    RakeCompile.uninstall(INSTALLED_RUNTIME_LIB)
  end
end
