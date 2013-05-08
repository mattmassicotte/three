COMPILER_SOURCES = FileList['compiler/**/*.cpp']

# COMPILER_SOURCES.each do |source|
#   # define the path
#   object_path = File.join(BUILD_DIR,File.dirname(source))
#   directory object_path
#   
#   object_path += File.basename(source, '.*') + '.o'
#   file object_path => source
#   puts source
# end

file 'compiler/Token.cpp'
file 'compiler/Token.h'
CLEAN.include("#{BUILD_DIR}/Token.o")
file "#{BUILD_DIR}/Token.o" => ['compiler/Token.cpp', 'compiler/Token.h', BUILD_DIR] do
  compile('compiler/Token.cpp', "#{BUILD_DIR}/Token.o")
end

file 'compiler/Lexer.cpp'
file 'compiler/Lexer.h'
CLEAN.include("#{BUILD_DIR}/Lexer.o")
file "#{BUILD_DIR}/Lexer.o" => ['compiler/Lexer.cpp', 'compiler/Lexer.h', 'compiler/Token.h', BUILD_DIR] do
  compile('compiler/Lexer.cpp', "#{BUILD_DIR}/Lexer.o")
end

file 'compiler/compiler.cpp'
CLEAN.include("#{BUILD_DIR}/Compiler.o")
file "#{BUILD_DIR}/Compiler.o" => ['compiler/compiler.cpp', BUILD_DIR] do
  compile('compiler/compiler.cpp', "#{BUILD_DIR}/Compiler.o")
end

file 'compiler/Parser.h' => 'compiler/Lexer.h'
file 'compiler/Parser.cpp' => 'compiler/Parser.h'
CLEAN.include("#{BUILD_DIR}/Parser.o")
file "#{BUILD_DIR}/Parser.o" => ['compiler/Parser.cpp', BUILD_DIR] do
  compile('compiler/Parser.cpp', "#{BUILD_DIR}/Parser.o")
end

directory "#{BUILD_DIR}/AST" => BUILD_DIR
file 'compiler/AST/ASTNode.cpp' => ['compiler/AST/ASTNode.h']
CLEAN.include("#{BUILD_DIR}/AST/ASTNode.o")
file "#{BUILD_DIR}/AST/ASTNode.o" => ['compiler/AST/ASTNode.cpp', "#{BUILD_DIR}/AST"] do
  compile('compiler/AST/ASTNode.cpp', "#{BUILD_DIR}/AST/ASTNode.o")
end

directory "#{BUILD_DIR}/AST" => BUILD_DIR
file 'compiler/AST/FunctionDefinitionNode.cpp' => ['compiler/AST/FunctionDefinitionNode.h']
CLEAN.include("#{BUILD_DIR}/AST/FunctionDefinitionNode.o")
file "#{BUILD_DIR}/AST/FunctionDefinitionNode.o" => ['compiler/AST/FunctionDefinitionNode.cpp', 'compiler/Parser.h', "#{BUILD_DIR}/AST"] do
  compile('compiler/AST/FunctionDefinitionNode.cpp', "#{BUILD_DIR}/AST/FunctionDefinitionNode.o")
end

directory "#{BUILD_DIR}/AST" => BUILD_DIR
file 'compiler/AST/FunctionCallNode.cpp' => ['compiler/AST/FunctionCallNode.h']
CLEAN.include("#{BUILD_DIR}/AST/FunctionCallNode.o")
file "#{BUILD_DIR}/AST/FunctionCallNode.o" => ['compiler/AST/FunctionCallNode.cpp', 'compiler/Parser.h', "#{BUILD_DIR}/AST"] do
  compile('compiler/AST/FunctionCallNode.cpp', "#{BUILD_DIR}/AST/FunctionCallNode.o")
end

directory "#{BUILD_DIR}/AST" => BUILD_DIR
file 'compiler/AST/RootNode.cpp' => ['compiler/AST/RootNode.h']
CLEAN.include("#{BUILD_DIR}/AST/RootNode.o")
file "#{BUILD_DIR}/AST/RootNode.o" => ['compiler/AST/RootNode.cpp', "#{BUILD_DIR}/AST"] do
  compile('compiler/AST/RootNode.cpp', "#{BUILD_DIR}/AST/RootNode.o")
end

directory "#{BUILD_DIR}/tests"

file 'tests/LexerTests.cpp'
CLEAN.include("#{BUILD_DIR}/tests/LexerTests.o")
file "#{BUILD_DIR}/tests/LexerTests.o" => ['tests/LexerTests.cpp', 'compiler/Lexer.h', 'compiler/Token.h', "#{BUILD_DIR}/tests"] do
  compile('tests/LexerTests.cpp', "#{BUILD_DIR}/tests/LexerTests.o", '-I. -Igtest/include')
end

file 'tests/ParserTests.cpp' => ['compiler/Parser.h']
CLEAN.include("#{BUILD_DIR}/tests/ParserTests.o")
file "#{BUILD_DIR}/tests/ParserTests.o" => ['tests/ParserTests.cpp', "#{BUILD_DIR}/tests"] do
  compile('tests/ParserTests.cpp', "#{BUILD_DIR}/tests/ParserTests.o", '-I. -Igtest/include')
end

CLOBBER.include("#{BUILD_DIR}/compiler_test")
object_files = []
object_files << "#{BUILD_DIR}/Token.o"
object_files << "#{BUILD_DIR}/Lexer.o"
object_files << "#{BUILD_DIR}/Parser.o"
object_files << "#{BUILD_DIR}/AST/ASTNode.o"
object_files << "#{BUILD_DIR}/AST/FunctionDefinitionNode.o"
object_files << "#{BUILD_DIR}/AST/FunctionCallNode.o"
object_files << "#{BUILD_DIR}/AST/RootNode.o"
object_files << "#{BUILD_DIR}/tests/LexerTests.o"
object_files << "#{BUILD_DIR}/tests/ParserTests.o"
object_files << "#{BUILD_DIR}/gtest/gtest_main.o"
object_files << "#{BUILD_DIR}/gtest/gtest-all.o"
file "#{BUILD_DIR}/compiler_test" => object_files do
  executable(object_files, "#{BUILD_DIR}/compiler_test")
end

task 'compiler:test' => "#{BUILD_DIR}/compiler_test" do
  sh("#{BUILD_DIR}/compiler_test")
end
