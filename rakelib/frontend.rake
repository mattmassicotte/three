# Compiler rakefile

FRONTEND_SOURCES = FileList['frontend/**/*.cpp']

object_files = []
FRONTEND_SOURCES.each do |source|
  # define the path
  object_dir  = BuildFunctions::absolute_path(File.dirname(source))
  object_path = BuildFunctions::absolute_path(File.basename(source, '.*') + '.o')

  # define a dependency on the object path
  directory(object_dir)

  object_files << object_path

  # make sure to clean this object
  CLEAN.include(object_path)

  # setup object file dependencies
  dependencies = BuildFunctions::get_dependencies(source)
  dependencies << __FILE__
  dependencies << object_dir

  # and finally, setup the rule to build this object
  file(object_path => dependencies) do
    BuildFunctions::compile(source, object_path)
  end
end

CLOBBER.include("#{BUILD_DIR}/three")
file "#{BUILD_DIR}/three" => [object_files, "#{BUILD_DIR}/libthree.a"].flatten do
  BuildFunctions::executable(object_files, "#{BUILD_DIR}/three", "'-L#{BUILD_DIR}' -lthree")
end

desc "Build the three compiler tool"
task 'compiler:frontend' => "#{BUILD_DIR}/three"
