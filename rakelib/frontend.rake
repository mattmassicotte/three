# Compiler rakefile

FRONTEND_CC_FLAGS = '-I.'

FRONTEND_SOURCES = FileList['frontend/**/*.cpp']
FRONTEND_OBJECTS = BuildFunctions::objects_for_sources(FRONTEND_SOURCES, :flags => FRONTEND_CC_FLAGS)

CLOBBER.include(FRONTEND_EXECUTABLE)
file FRONTEND_EXECUTABLE => [FRONTEND_OBJECTS, "#{BUILD_DIR}/libthree_compiler.a"].flatten do
  BuildFunctions::executable(FRONTEND_OBJECTS, FRONTEND_EXECUTABLE, "'-L#{BUILD_DIR}' -lthree_compiler")
end

namespace :compiler do
  desc "Build the three compiler tool"
  task :frontend => FRONTEND_EXECUTABLE
end
