#include "../headers/ast.h"

AST buildAST(Token* tokens)
{
    AST node = newNode(AST_PROGRAM);
    AST candidat;
    size_t ptr = 0;


    while (tokens[ptr].type != LEX_ENDOFTOKENS)
    {
        // find blocks !! order matters
        if (buildBLOCK(tokens, &ptr, &candidat, -1)) {
            addToChildren(&node, candidat);

        } else if(buildVarDeclFunc(tokens, &ptr, &candidat, -1)) {
            addToChildren(&node, candidat);

        } else if(buildVarDeclTable(tokens, &ptr, &candidat, -1)) {
            addToChildren(&node, candidat);
        
        } else if(buildVarDecl(tokens, &ptr, &candidat, -1)) {
            addToChildren(&node, candidat);
        
        } else {
            ptr++;
        }
    }

    return node;
}

// build other nodes
int buildBLOCK(Token* tokens, size_t* ptr, AST* node, size_t stop)
{
    size_t cursor = *ptr;
    size_t end = *ptr + 1;

    // check start
    if (tokens[cursor].type != LEX_LBRACE)
        return 0;

    // find end
    int opened = 1;
    
    while (opened > 0)
    {
        if (tokens[end].type == LEX_LBRACE)
            opened++;

        if (tokens[end].type == LEX_RBRACE)
            opened--;

        // error
        if (tokens[end].type == LEX_ENDOFTOKENS || (end == stop))
            ERROR("Block opend on line %d char %d never closed", tokens[cursor].pos.lineNb, tokens[cursor].pos.lineNb)
        
        end++;
    }

    // create node
    node[0] = newNode(AST_BLOCK);
    ptr[0] = end;   

    // check children
    AST candidat;
    cursor++;

    while (cursor < end-1 && cursor != stop)
    {
        // find blocks !! order matters
        if (buildBLOCK(tokens, &cursor, &candidat, end-1)) {
            addToChildren(node, candidat);
    
        } else if(buildVarDeclTable(tokens, &cursor, &candidat, end-1)) {
            addToChildren(node, candidat);
    
        } else if(buildVarDecl(tokens, &cursor, &candidat, end-1)) {
            addToChildren(node, candidat);

        } else {
            cursor++;
        }
    }

    return 1;
}

int buildName(Token* tokens, size_t* ptr, AST* node, size_t stop)
{
    if (tokens[*ptr].type != LEX_NAME)
        return 0;

    // build node
    node[0] = newNode(AST_NAME);
    node->hasValue = 1;
    node->value = (void*)tokens[*ptr].raw;

    ptr[0]++;

    return 1;
}

int buildVarDeclFunc(Token* tokens, size_t* ptr, AST* node, size_t stop)
{
    AST type, name, block;
    size_t cursor = *ptr;

    // check type
    if (!buildType(tokens, &cursor, &type, stop))
        return 0;

    // check name
    if (!buildName(tokens, &cursor, &name, stop))
        return 0;

    // check args
    if (tokens[cursor].type != LEX_LPARENTHESIS)
        return 0;

    AST* args = NULL;
    size_t argc = 0;
    cursor++;

    while (tokens[cursor].type != LEX_RPARENTHESIS) {
        AST arg;

        // args
        if (buildVarDeclTable(tokens, &cursor, &arg, stop)) 
        {
            SECURE_REALLOCATE(args, AST*, sizeof(AST)*(argc+1), 
                              "Unable to allocate a new arg node")
            args[argc] = arg;
            argc++; 

        } else if (buildVarDecl(tokens, &cursor, &arg, stop)) {
            SECURE_REALLOCATE(args, AST*, sizeof(AST)*(argc+1), 
                              "Unable to allocate a new arg node")
            args[argc] = arg;
            argc++; 
        } else {
            ERROR("Token should have been var declarcation but have %s", tokens[cursor].raw)
        }

        // separetor
        if (tokens[cursor].type != LEX_COMMA && 
            tokens[cursor].type != LEX_RPARENTHESIS) {
            ERROR("Token should have been \",\" or \")\" but have %s", tokens[cursor].raw)
        }
        
        if (tokens[cursor].type == LEX_COMMA)
            cursor++;
    }

    // check block
    cursor++;
    if (!buildBLOCK(tokens, &cursor, &block, stop)) 
        ERROR("Token should have been \"{\" but have %s", tokens[cursor].raw)

    // build node
    node[0] = newNode(AST_VARDECLAREFUNC);
    addToChildren(node, type);
    addToChildren(node, name);

    for (size_t i = 0; i<argc; i++)
        addToChildren(node, args[i]);
    
    addToChildren(node, block);

    ptr[0] = cursor;
    
    free(args);
    return 1;
}

int buildVarDeclTable(Token* tokens, size_t* ptr, AST* node, size_t stop)
{
    AST type, name, size;
    size_t cursor = ptr[0];

    // check
    if ( !buildType(tokens, &cursor, &type, stop))
        return 0;

    if ( !buildName(tokens, &cursor, &name, stop))
        return 0;

    if (tokens[cursor].type != LEX_LBRACKET)
        return 0;

    cursor++;

    // build Node
    if (tokens[cursor].type == LEX_RBRACKET)
    {
        node[0] = newNode(AST_VARDECLARETABLE);
        addToChildren(node, type);
        addToChildren(node, name);
    
    } else if (buildLiteralInt(tokens, &cursor, &size, stop)) {
        node[0] = newNode(AST_VARDECLARETABLE);
        addToChildren(node, type);
        addToChildren(node, name);
        addToChildren(node, size);
    } else {
        return 0;
    }

    ptr[0] = cursor+1;

    return 1;
}

int buildVarDecl(Token* tokens, size_t* ptr, AST* node, size_t stop)
{
    AST type;
    AST name;
    size_t cursor = ptr[0];

    // check
    if ( !buildType(tokens, &cursor, &type, stop))
        return 0;

    if ( !buildName(tokens, &cursor, &name, stop))
        return 0;

    // build Node
    node[0] = newNode(AST_VARDECLARATION);
    addToChildren(node, type);
    addToChildren(node, name);
    ptr[0] = cursor;

    return 1;
}

int buildType(Token* tokens, size_t* ptr, AST* node, size_t stop)
{
    AST candidat;
    AST ptrCandidat;

    switch (tokens[*ptr].type)
    {
        case LEX_CHAR_TYPE:
            if (!buildCharType(tokens, ptr, &candidat, stop))
                return 0;
            break;

        case LEX_INT_TYPE:
            if (!buildIntType(tokens, ptr, &candidat, stop))
                return 0;
            break;

        case LEX_FLOAT_TYPE:
            if (!buildFloatType(tokens, ptr, &candidat, stop))
                return 0;
            break;

        default:
            return 0;
    }
    
    node[0] = newNode(AST_TYPE);
    addToChildren(node, candidat);

    // check if ptr
    if (buildPtrType(tokens, ptr, &ptrCandidat, stop))
        addToChildren(node, ptrCandidat);

    return 1;
}

int buildPtrType(Token* tokens, size_t* ptr, AST* node, size_t stop)
{
    if (tokens[*ptr].type != LEX_MULT)
        return 0;

    // check ptr depth
    size_t depth = 0; 

    while (tokens[*ptr+depth].type == LEX_MULT)
        depth++;

    // build Node
    size_t* value = (size_t*)malloc(sizeof(size_t));
    CHECK_ALLOCATE(value, "Unable to save the depthj of the pointer")
    value[0] = depth;

    node[0] = newNode(AST_PTRTYPE);
    node->hasValue = 1;
    node->value = value;

    ptr[0] += depth;

    return 1;
}

int buildFloatType(Token* tokens, size_t* ptr, AST* node, size_t stop)
{
    if (tokens[*ptr].type != LEX_FLOAT_TYPE)
        return 0;

    // build node
    node[0] = newNode(AST_FLOATTYPE);
    ptr[0]++;

    return 1;
}

int buildIntType(Token* tokens, size_t* ptr, AST* node, size_t stop)
{
    if (tokens[*ptr].type != LEX_INT_TYPE)
        return 0;

    // build node
    node[0] = newNode(AST_INTTYPE);
    ptr[0]++;

    return 1;
}

int buildCharType(Token* tokens, size_t* ptr, AST* node, size_t stop)
{
    if (tokens[*ptr].type != LEX_CHAR_TYPE)
        return 0;

    // build node
    node[0] = newNode(AST_CHARTYPE);
    ptr[0]++;

    return 1;
}


int buildLiteralInt(Token* tokens, size_t* ptr, AST* node, size_t stop)
{
    if ((tokens[*ptr].type != LEX_INT_LITERAL && tokens[*ptr].type != LEX_MINUS)  || 
        (tokens[*ptr].type == LEX_MINUS && tokens[(*ptr) +1].type != LEX_INT_LITERAL))
        return 0;

    // setup data
    size_t start = *ptr;
    int data = 0;
    int signe = 1;

    if (tokens[start].type == LEX_MINUS) {
        signe = -1; 
        start += 1;
    }

    // convert chart to int
    size_t i=0;
    while (tokens[start].raw[i] != '\0')
    {
        data = 10*data + signe*(tokens[start].raw[i]-48);
        i++;
    }

    // build node
    int* value = (int*)malloc(sizeof(int));
    CHECK_ALLOCATE(value, "Unable to save the depthj of the pointer")
    value[0] = data;
    
    node[0] = newNode(AST_LITERALINT);
    node->hasValue = 1;    
    node->value = value;

    ptr[0] = start+1;

    return 1;

}


// utils
AST newNode(ASTNodeType type)
{
    AST node;
    node.type = type;
    node.children = NULL;
    node.length = 0;
    node.value = NULL;
    node.hasValue = 0;

    return node;
}

char* NodeType2String(ASTNodeType type)
{
    switch (type) {
        case AST_PROGRAM:
            return "Programm";
        case AST_BLOCK:
            return "Block";
        case AST_VARDECLARATION:
            return "VarDecl";
        case AST_TYPE:
            return "Type";
        case AST_PTRTYPE:
            return "PtrType";
        case AST_CHARTYPE:
            return "CharType";
        case AST_INTTYPE:
            return "IntType";
        case AST_FLOATTYPE:
            return "FloatType";
        case AST_NAME:
            return "Name";
        case AST_LITERALINT:
            return "LiteralInt";
        case AST_VARDECLARETABLE:
            return "TableVarDecl";
        case AST_VARDECLAREFUNC:
            return "FuncDecl";
        default:
            ERROR("Unknow NodeType %d", type)
    }
}

void addToChildren(AST* parent, AST children)
{
    SECURE_REALLOCATE(parent->children, AST*, sizeof(AST)*(parent->length+1),
                      "Unable to alocate a new node in the ast")
    parent->children[parent->length] = children;
    parent->length++; 
}


void showAST(AST tree) {
    INFO("AST:\n")
    showASThelper(tree, 0);
    printf("\n"); INFO("End AST");
}


void showASThelper(AST tree, size_t depth)
{
    padAST(depth);
    printf("%s", NodeType2String(tree.type));

    
    // show value
    if (tree.hasValue)
    {
        printf("(");
        reprASTvalue(tree);
        printf(")");
    }

    // show children with padding
    if (tree.children > 0)
    {
        printf(":\n");
        for (size_t i=0; i<tree.length; i++)
            showASThelper(tree.children[i], depth+1);
    } else {
        printf("\n");
    }
}

void padAST(size_t depth)
{
    for (size_t i=0; i<3*depth; i++)
        printf(" ");
}

void reprASTvalue(AST tree)
{
    switch(tree.type)
    {
        case AST_NAME:        
            printf("%s", (char*)tree.value);
            break;
        case AST_PTRTYPE:
            printf("depth=%d", *((size_t*)tree.value));
            break;
        case AST_LITERALINT:
            printf("value=%d", *((size_t*)tree.value));
            break;
        default:
            ERROR("Can't show data for node of type %s", NodeType2String(tree.type));
    }
}