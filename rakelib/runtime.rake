RUNTIME_CC_FLAGS = '-I.'

RUNTIME_SOURCES = FileList['runtime/**/*.c']
RUNTIME_HEADERS = FileList['runtime/**/*.h']

RUNTIME_OBJECTS = BuildFunctions::objects_for_sources(RUNTIME_SOURCES)

directory "#{BUILD_DIR}/runtime"
directory "#{THREE_INCLUDE_DIR}/runtime"

CLOBBER.include(RUNTIME_LIB)
file(RUNTIME_LIB => RUNTIME_OBJECTS) do
  BuildFunctions::library(RUNTIME_OBJECTS, RUNTIME_LIB)
end

namespace :runtime do
  desc "Build the runtime library"
  task :build => RUNTIME_LIB

  desc "Install the runtime library and C headers"
  task :install => [RUNTIME_LIB, THREE_LIB_DIR, "#{THREE_INCLUDE_DIR}/runtime"] do
    BuildFunctions::install(RUNTIME_LIB, File.join(THREE_LIB_DIR, 'libthree_runtime.a'))
    RUNTIME_HEADERS.each do |header|
      file = File.basename(header)
      BuildFunctions::install(header, "#{THREE_INCLUDE_DIR}/runtime/#{file}")
    end
  end

  desc "Uninstall the runtime library and C headers"
  task :uninstall do
    FileUtils.rm_rf("#{THREE_INCLUDE_DIR}/runtime")
    FileUtils.rm_f(File.join(THREE_LIB_DIR, 'libthree_runtime.a'))
  end
end
