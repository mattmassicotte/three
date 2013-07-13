GTEST_CC_FLAGS = '-Igtest -Igtest/include -Wno-logical-op-parentheses -DGTEST_HAS_TR1_TUPLE=0'

namespace :gtest do
  desc 'Installs the C++ unit testing package, gtest'
  task :install do
    FileUtils.rm_f('gtest')
    FileUtils.rm_f('gtest-1.6.0.zip')
    FileUtils.rm_f('gtest-1.6.0')

    BuildFunctions.log("download", "gtest-1.6.0")
    sh('curl -o gtest-1.6.0.zip http://googletest.googlecode.com/files/gtest-1.6.0.zip')
    sh('unzip -q gtest-1.6.0.zip')
    FileUtils.rm_f('gtest-1.6.0.zip')
    FileUtils.mv('gtest-1.6.0', 'gtest')
  end
  
  desc 'Builds the gtest binaries'
  task :build => ["#{BUILD_DIR}/gtest/libgtest.a", "#{BUILD_DIR}/gtest/libgtest_main.a"]
end

file 'gtest/src/gtest_main.cc' do
  Rake::Task['gtest:install'].invoke
end

file 'gtest/src/gtest-all.cc' do
  Rake::Task['gtest:install'].invoke
end

directory "#{BUILD_DIR}/gtest"
CLEAN.include("#{BUILD_DIR}/gtest/gtest-all.o")
file "#{BUILD_DIR}/gtest/gtest-all.o" => ['gtest/src/gtest-all.cc', "#{BUILD_DIR}/gtest"] do
  BuildFunctions::compile('gtest/src/gtest-all.cc', "#{BUILD_DIR}/gtest/gtest-all.o", GTEST_CC_FLAGS)
end

CLEAN.include("#{BUILD_DIR}/gtest/gtest_main.o")
file "#{BUILD_DIR}/gtest/gtest_main.o" => ['gtest/src/gtest_main.cc', "#{BUILD_DIR}/gtest"] do
  BuildFunctions::compile('gtest/src/gtest_main.cc', "#{BUILD_DIR}/gtest/gtest_main.o", GTEST_CC_FLAGS)
end

CLOBBER.include("#{BUILD_DIR}/gtest/libgtest.a")
file "#{BUILD_DIR}/gtest/libgtest.a" => ["#{BUILD_DIR}/gtest/gtest-all.o"] do
  BuildFunctions::library(["#{BUILD_DIR}/gtest/gtest-all.o"], "#{BUILD_DIR}/gtest/libgtest.a")
end

CLOBBER.include("#{BUILD_DIR}/gtest/libgtest_main.a")
file "#{BUILD_DIR}/gtest/libgtest_main.a" => ["#{BUILD_DIR}/gtest/gtest_main.o", "#{BUILD_DIR}/gtest"] do
  BuildFunctions::library(["#{BUILD_DIR}/gtest/gtest_main.o"], "#{BUILD_DIR}/gtest/libgtest_main.a")
end
