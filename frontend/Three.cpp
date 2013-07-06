#include "../compiler/Parser.h"

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

    options->outputFile = std::string("three_output.c");

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

Language::RootNode* createRootNodeFromFile(const std::string& path) {
    std::ifstream inputFile(path);

    Language::Lexer lexer(&inputFile);
    Language::Parser parser(&lexer);

    return parser.rootASTNode();
}

bool createCSource(Language::RootNode* node, const std::string& path) {
    Language::CSourceContext c;
    std::ofstream outputFile(path);

    node->codeGenCSource(c);

    outputFile << c.renderToString();

    return true;
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

    Language::RootNode* node;

    node = createRootNodeFromFile(std::string(argv[0]));

    if (options.printAST) {
        std::cout << node->recursiveStr() << std::endl;
        return 0;
    }

    createCSource(node, options.outputFile);

    return 0;
}
