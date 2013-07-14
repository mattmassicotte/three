module BuildFunctions
  extend Rake::DSL
  
  def self.log(label, action)
    # make the label bold
    puts("[\e[1m#{label}\e[0m] #{action}")
  end
  
  def self.compiler_for(source_file)
    case File.extname(source_file)
    when '.cpp', '.cc', '.hpp'
      "clang++ -std=c++0x -stdlib=libc++"
    when ".c", ".h"
      "clang -std=c11"
    else
      raise("Don't know how to compile #{source_file}")
    end
  end

  def self.install(src, dest)
    self.log("install", dest)
    FileUtils.cp(src, dest)
  end

  def self.compile(input, output, opts=nil)
    self.log("compile", output)
    Rake::sh("#{compiler_for(input)} #{opts} -o '#{output}' -c #{input}")
  end

  def self.executable(inputs, output, libs=nil)
    self.log("link", output)
    inputs = inputs.join(" ")
    Rake::sh("clang++ -std=gnu+11 -stdlib=libc++ #{libs} -o '#{output}' #{inputs}")
  end

  def self.library(inputs, output)
    FileUtils.rm_f(output)

    self.log("library", output)
    inputs = inputs.join(" ")
    Rake::sh("/usr/bin/ar rs '#{output}' #{inputs}")
  end

  def self.get_dependencies(input, flags=nil)
    string = `#{compiler_for(input)} #{flags} -MM #{input}`.chomp()

    string.gsub!(" \\\n  ", " ")

    dependencies = string.split(" ")
    dependencies.delete_at(0)  # remove the first element, which is the object file

    dependencies.collect { |x| File.absolute_path(x) }
  end

  def self.absolute_path(input)
    File.absolute_path(File.join(BUILD_DIR, input))
  end
  
  def self.objects_for_sources(filelist, opts={})
    opts[:find_deps] = true if opts[:find_deps].nil?
    
    object_files = []
    filelist.each do |source|
      # define the path
      object_dir  = BuildFunctions::absolute_path(File.dirname(source))
      object_path = BuildFunctions::absolute_path(File.basename(source, '.*') + '.o')

      # define a dependency on the object path
      directory(object_dir)

      object_files << object_path

      # make sure to clean this object
      CLEAN.include(object_path)

      # setup object file dependencies
      dependencies = [__FILE__, object_dir]
      dependencies.concat(BuildFunctions::get_dependencies(source, opts[:flags])) if opts[:find_deps]

      # and finally, setup the rule to build this object
      file(object_path => dependencies) do
        BuildFunctions::compile(source, object_path, opts[:flags])
      end
    end
    
    object_files
  end
end
