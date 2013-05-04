def compile(input, output, opts=nil)
  puts("Compile: #{output}")
  sh("clang++ -std=c++0x #{opts} -o '#{output}' -c #{input}")
end

def executable(inputs, output)
  puts("   Link: #{output}")
  inputs = inputs.join(" ")
  sh("clang++ -std=c++0x -o '#{output}' #{inputs}")
end

def library(inputs, output)
  rm_f(output)
  
  puts("Library: #{output}")
  inputs = inputs.join(" ")
  sh("/usr/bin/ar rs '#{output}' #{inputs}")
end
