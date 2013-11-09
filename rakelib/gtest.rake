GTEST_VERSION = '1.6.0'
GTEST_DIR     = File.absolute_path(File.join(VENDOR_DIR, 'gtest'))

GTEST_INCLUDE_DIR = File.join(GTEST_DIR, 'include')
GTEST_HEADER  = File.join(GTEST_INCLUDE_DIR, 'gtest', 'gtest.h')
GTEST_LIB     = "#{BUILD_DIR}/libgtest.a"

GTEST_CPP_FLAGS = "-I#{GTEST_DIR} -I#{GTEST_INCLUDE_DIR} -Wno-logical-op-parentheses -DGTEST_HAS_TR1_TUPLE=0"

CLOBBER.include(GTEST_LIB)

namespace :gtest do

  desc 'Removes the gtest sources'
  task :remove do
    FileUtils.rm_f(GTEST_DIR)
    FileUtils.rm_f("#{GTEST_DIR}.zip")
  end

  desc "Downloads the gtest #{GTEST_VERSION} sources"
  task :download => VENDOR_DIR do
    Dir.chdir(VENDOR_DIR) do
      RakeCompile.log('DOWNLOAD'.red, "gtest-#{GTEST_VERSION}")
      path = "gtest-#{GTEST_VERSION}.zip"
      sh("curl -o #{path} http://googletest.googlecode.com/files/#{path}")
      sh("unzip -q #{path}")

      FileUtils.rm_f(path)
      FileUtils.mv("gtest-#{GTEST_VERSION}", 'gtest')
    end
  end

  desc "Build the gtest #{GTEST_VERSION} objects as a static lib"
  task :build => :define_tasks do
    Rake::Task[GTEST_LIB].invoke
  end

  task :define_tasks => GTEST_HEADER do
    cpp_flags GTEST_CPP_FLAGS
    static_library GTEST_LIB do |target|
      target.add_object("#{GTEST_DIR}/src/gtest_main.cc", "#{BUILD_DIR}/gtest/gtest_main.o")
      target.add_object("#{GTEST_DIR}/src/gtest-all.cc", "#{BUILD_DIR}/gtest/gtest-all.o")
    end
  end

end

file GTEST_HEADER do
  Rake::Task['gtest:download'].invoke
end
