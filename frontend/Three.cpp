#include "../compiler/Parser.h"
#include "../compiler/CodeGen/CSourceContext.h"
#include "../compiler/AST/ASTNode.h"
#include "../compiler/CSourceIndexer.h"

#include <getopt.h>
#include <iostream>
#include <fstream>

typedef struct {
    bool debug;
    bool printAST;
    bool trace;

    std::string outputFile;
} build_options_t;

void getOptions(build_options_t* options, int argc, char** argv)
{
    int32_t c;

    static struct option longopts[] = {
        { "debug",     no_argument,       NULL, 'd' },
        { "help",      no_argument,       NULL, 'h' },
        { "output",    required_argument, NULL, 'o' },
        { "print",     no_argument,       NULL, 'p' },
        { "trace",     no_argument,       NULL, 't' },
        { "version",   no_argument,       NULL, 'v' },
        { NULL,        0,                 NULL, 0   }
    };

    options->debug    = false;
    options->printAST = false;
    options->trace    = false;

    while ((c = getopt_long(argc, argv, "dho:ptv", longopts, NULL)) != -1) {
        switch (c) {
            case 'd':
                options->debug = true;
                break;
            case 'o':
                options->outputFile = std::string(optarg);
                break;
            case 'p':
                options->printAST = true;
                break;
            case 't':
                options->trace = true;
                break;
            case 'v':
                printf("Three Release: %s\n\n", "0.1");
                exit(0);
                break;
            case 'h':
            default:
                printf("usage: three [options] [--] [file] [arguments]\n");
                printf("\n");
                printf("  -d (--debug)      disable optimizations\n");
                printf("  -h (--help)       print this help message and exit\n");
                printf("  -o (--output)     name of output file\n");
                printf("  -p (--print)      print out the AST representation for the input file\n");
                printf("  -t (--trace)      trace compilation steps\n");
                printf("  -v (--version)    print versions\n");
                exit(0);
                break;
        }
    }
}

bool createCOutputs(Three::ParsingContext* context, const std::string& basePath) {
    Three::CSourceContext outputContext;

    context->rootNode()->codeGen(outputContext);
    
    std::ofstream bodyFile(basePath + ".c");

    // make sure to import the headers
    outputContext.internalDeclarations()->addHeader(true, basePath + ".h");
    outputContext.declarations()->addHeader(true, basePath + "_internal.h");

    bodyFile << "// Declarations" << std::endl;
    bodyFile << outputContext.declarations()->renderToString();
    bodyFile << std::endl << "// Definitions" << std::endl;
    bodyFile << outputContext.body()->renderToString();

    std::ofstream internalHeaderFile(basePath + "_internal.h");

    internalHeaderFile << outputContext.internalDeclarations()->renderToStringWithIncludeGuard("__" + basePath + "_internal__");

    std::ofstream externalHeaderFile(basePath + ".h");

    externalHeaderFile << outputContext.externalDeclarations()->renderToStringWithIncludeGuard("__" + basePath + "__");

    return true;
}

std::vector<std::string> defaultCIncludePaths(void) {
    std::vector<std::string> paths = *Three::CSourceIndexer::defaultCIncludePaths();

    paths.push_back("/usr/local/include/three/runtime");

    return paths;
}

bool compileCSource(Three::ParsingContext* context, const std::string& cSourcePath, const std::string& outputPath) {
    std::stringstream s;
    bool asMain = context->currentModule()->hasMainFunction();

    s << "clang -std=c11";
    s << " -o '" << outputPath;
    if (!asMain) {
        s << ".o";
    }
    s << "'";

    // only link if main symbol is present
    if (asMain) {
        s << " -L/usr/local/lib";
        s << " -lthree_runtime";
    }

    s << " -I.";

    for (const std::string& string : defaultCIncludePaths()) {
        s << " '-I" << string << "'";
    }

    if (!asMain) {
        s << " -c";
    }

    s << " '" << cSourcePath << "'";

    return system(s.str().c_str()) == 0;
}

int processInputFile(build_options_t* options, const std::string& inputFile) {
    Three::ParsingContext* parsingContext = Three::Parser::contextFromFile(inputFile);
    
    if (options->printAST) {
        std::cout << parsingContext->rootNode()->recursiveStr() << std::endl;
        return 0;
    }

    // create a sensible output path default, if none was supplied
    if (options->outputFile.length() == 0) {
        if (inputFile.substr(inputFile.length() - 2, 2) == ".3") {
            options->outputFile = inputFile.substr(0, inputFile.length() - 2);
        } else {
            options->outputFile = std::string("three_output");
        }
    }

    createCOutputs(parsingContext, options->outputFile);
    compileCSource(parsingContext, options->outputFile + ".c", options->outputFile);

    return 0;
}

int main(int argc, char** argv) {
    build_options_t options;

    // read options, and then adjust the inputs
    getOptions(&options, argc, argv);
    argc -= optind;
    argv += optind;

    if (argc == 0) {
        // no input files, REPL should be kicked off here
        fprintf(stderr, "[Compile] No input files\n");
        return 1;
    }

    return processInputFile(&options, std::string(argv[0]));

}
