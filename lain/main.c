#include <stdio.h>
#include <stdlib.h>

#include "utils/base_log.h"

#include "headers/parse_args.h"
#include "headers/lexer.h"
#include "headers/ast.h"

char* readAllFile(char* path)
{
    // open file
    FILE* fp = fopen(path, "rb");
    CHECK_ALLOCATE(fp, "Can't read the file %s", path);

    // get file size
    fseek(fp, 0, SEEK_END); 
    size_t size = ftell(fp); 
    fseek(fp, 0, SEEK_SET);

    // read data
    char* buffer = (char*) malloc(sizeof(char) * (size+1));
    CHECK_ALLOCATE(buffer, "Unable to allocate a buffer of %d chars", size)

    CHECK_READ_WRITE(size, fread(buffer, sizeof(char), size, fp), "unable to read the file %s", path)
    buffer[size] = '\0';

    // close file
    fclose(fp);

    return buffer;
} 


int main(size_t argc, char* argv[])
{
    // read args
    if (argc <= 1)
        ERROR("No file pass as arg")

    ARGS args = argsParser(argc-1, argv +1);

    // read file
    char* rawText = readAllFile(args.args[ARG_InputFile].args[0]);

    // tokenize
    Token* tokens = lexer((const char*) rawText);
    //showTokens(tokens, 1);

    // build ast 
    AST ast = buildAST(tokens);
    showAST(ast);

    // end
    INFO("ENDED")
    return 1;
}