RUNTIME_CC_FLAGS = '-I.'

RUNTIME_SOURCES = FileList['runtime/**/*.c']
RUNTIME_HEADERS = FileList['runtime/**/*.h']

RUNTIME_OBJECTS = BuildFunctions::objects_for_sources(RUNTIME_SOURCES)

RUNTIME_DIR = File.join(THREE_INCLUDE_DIR, 'runtime')

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
  task :install => [RUNTIME_LIB, THREE_LIB_DIR, RUNTIME_DIR] do
    BuildFunctions::install(RUNTIME_LIB, File.join(THREE_LIB_DIR, 'libthree_runtime.a'))
    RUNTIME_HEADERS.each do |header|
      file = File.basename(header)
      BuildFunctions::install(header, File.join(RUNTIME_DIR, file))
    end

    FileUtils.rm_rf("#{THREE_INCLUDE_DIR}/modules")
    BuildFunctions::install('modules', "#{THREE_INCLUDE_DIR}/modules")
  end

  desc "Uninstall the runtime library and C headers"
  task :uninstall do
    FileUtils.rm_rf("#{THREE_INCLUDE_DIR}/modules")
    FileUtils.rm_rf("#{THREE_INCLUDE_DIR}/runtime")
    FileUtils.rm_f(File.join(THREE_LIB_DIR, 'libthree_runtime.a'))
  end
end
