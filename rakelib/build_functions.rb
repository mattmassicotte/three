module BuildFunctions
  extend Rake::DSL
  
  def self.log(label, action)
    # make the label bold
    puts("[\e[1m#{label}\e[0m] #{action}")
  end
  
  def self.compiler_for(source_file)
    case File.extname(source_file)
    when '.cpp', '.cc', '.hpp'
      "clang++ -std=c++0x -stdlib=libc++ -I/usr/local/include"
    when ".c", ".h"
      "clang -std=c11 -I/usr/local/include"
    else
      raise("Don't know how to compile #{source_file}")
    end
  end

  def self.optimal_parallelism
    `sysctl -An "machdep.cpu.thread_count"`.chomp.to_i
  end

  def self.install(src, dest)
    self.log("install", dest)
    FileUtils.cp_r(src, dest)
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

  def self.output_extension_for_file(source_file)
    case File.extname(source_file)
    when '.h', '.hpp'
      '.gch'
    when '.c', '.cc', '.cpp'
      '.o'
    else
      raise
    end
  end

  def self.define_dependencies_for_source(source, opts={})
    # define the path
    object_dir  = BuildFunctions::absolute_path(File.dirname(source))
    output_ext  = BuildFunctions::output_extension_for_file(source)
    object_path = BuildFunctions::absolute_path(File.basename(source, '.*') + output_ext)

    # define a dependency on the object path
    directory(object_dir)

    # make sure to clean this object
    CLEAN.include(object_path)

    # setup object file dependencies
    dependencies = [__FILE__, object_dir]

    compiler_flags = opts[:flags]

    dependencies.concat(opts[:deps])
    dependencies.concat(BuildFunctions::get_dependencies(source, compiler_flags)) if opts[:find_deps]

    # Add the pch include flag only to the compilation, not when finding dependencies.  This is because
    # the prefix header might not be compiled yet.
    if opts[:pch]
      compiler_flags += BuildFunctions::compiler_flag_for_pch(opts[:pch])
    end

    # and finally, setup the rule to build this object
    file(object_path => dependencies) do
      BuildFunctions::compile(source, object_path, compiler_flags)
    end

    object_path
  end

  def self.objects_for_sources(filelist, opts={})
    opts[:find_deps] = true if opts[:find_deps].nil?
    opts[:deps] ||= []

    # define the dependencies for the pch file, if any
    pch_output = define_pch_dependencies(opts)
    if pch_output
      opts[:deps] << pch_output
    end

    object_files = []
    filelist.each do |source|
      object_files << define_dependencies_for_source(source, opts)
    end

    object_files
  end

  def self.define_pch_dependencies(opts)
    pch_source = opts[:pch]
    return nil if pch_source.nil?

    opts = opts.dup
    opts.delete(:pch)

    define_dependencies_for_source(pch_source, opts)
  end

  def self.compiler_flag_for_pch(pch)
    # remove the extension, create an absolute path to the build directory
    pch = BuildFunctions::absolute_path(File.basename(pch, '.*'))

    " -include #{pch}"
  end
end
