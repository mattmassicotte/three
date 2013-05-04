def compiler_for(source_file)
  case File.extname(source_file)
  when ".cpp", ".cc", ".hpp"
    "clang++ -std=c++0x"
  when ".c", ".h"
    "clang -std=c11"
  else
    raise("Don't know how to compile #{source_file}")
  end
end

def compile(input, output, opts=nil)
  puts("Compile: #{output}")
  sh("#{compiler_for(input)} #{opts} -o '#{output}' -c #{input}")
end

def executable(inputs, output)
  puts("   Link: #{output}")
  inputs = inputs.join(" ")
  sh("clang++ -o '#{output}' #{inputs}")
end

def library(inputs, output)
  rm_f(output)
  
  puts("Library: #{output}")
  inputs = inputs.join(" ")
  sh("/usr/bin/ar rs '#{output}' #{inputs}")
end

def get_dependencies(input)
  string = `#{compiler_for(input)} -MM #{full_path}`.chomp()
  
  dependencies = string.split(" \\\n  ")
  dependencies = cached_dependencies[1, cached_dependencies.length]
  
  dependencies.collect { |x| File.absolute_path(x) }
end
