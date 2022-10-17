#ifndef __AST_H__
#define __AST_H__

#include <stdlib.h>
#include <stdio.h>

#include "../utils/base_log.h"
#include "../headers/lexer.h"

typedef enum {
    AST_PROGRAM,
    AST_BLOCK, // a block of code starting by { and finishing by }
    AST_VARDECLARATION, // declare a var 
    AST_VARDECLARETABLE, // detect if a var declaration is a table

    // type handeling
    AST_TYPE, 
    AST_PTRTYPE,
    AST_CHARTYPE,
    AST_FLOATTYPE,
    AST_INTTYPE,

    AST_NAME,

    // value
    AST_LITERALINT,
    

} ASTNodeType;

typedef struct astNode {
    struct astNode* children;
    size_t length;
    ASTNodeType type;
    void* value;
    unsigned int hasValue : 1;
} AST;


AST buildAST(Token* tokens);

// build other nodes
int buildBLOCK(Token* tokens, size_t* ptr, AST* node, size_t stop);
int buildName(Token* tokens, size_t* ptr, AST* node, size_t stop);

int buildVarDecl(Token* tokens, size_t* ptr, AST* node, size_t stop);
int buildVarDeclTable(Token* tokens, size_t* ptr, AST* node, size_t stop);
int buildType(Token* tokens, size_t* ptr, AST* node, size_t stop);

int buildPtrType(Token* tokens, size_t* ptr, AST* node, size_t stop);
int buildFloatType(Token* tokens, size_t* ptr, AST* node, size_t stop);
int buildIntType(Token* tokens, size_t* ptr, AST* node, size_t stop);
int buildCharType(Token* tokens, size_t* ptr, AST* node, size_t stop);

// TODO
int buildLiteralInt(Token* tokens, size_t* ptr, AST* node, size_t stop);


// utils
AST newNode(ASTNodeType type);
char* NodeType2String(ASTNodeType type);
void addToChildren(AST* parent, AST children);


// SHOW AST HELPERS
void showAST(AST tree);
void showASThelper(AST tree, size_t depth);
void padAST(size_t depth);
void reprASTvalue(AST tree);

#endif