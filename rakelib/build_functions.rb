module BuildFunctions
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

  def self.compile(input, output, opts=nil)
    puts("Compile: #{output}")
    Rake::sh("#{compiler_for(input)} #{opts} -o '#{output}' -c #{input}")
  end

  def self.executable(inputs, output, libs=nil)
    puts("   Link: #{output}")
    inputs = inputs.join(" ")
    Rake::sh("clang++ -std=gnu+11 -stdlib=libc++ #{libs} -o '#{output}' #{inputs}")
  end

  def self.library(inputs, output)
    FileUtils.rm_f(output)
  
    puts("Library: #{output}")
    inputs = inputs.join(" ")
    Rake::sh("/usr/bin/ar rs '#{output}' #{inputs}")
  end

  def self.get_dependencies(input)
    string = `#{compiler_for(input)} -MM #{input}`.chomp()

    string.gsub!(" \\\n  ", " ")

    dependencies = string.split(" ")
    dependencies.delete_at(0)  # remove the first element, which is the object file

    dependencies.collect { |x| File.absolute_path(x) }
  end

  def self.absolute_path(input)
    File.absolute_path(File.join(BUILD_DIR, input))
  end
end
