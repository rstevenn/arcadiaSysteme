#include "../headers/lexer.h"
#include "../utils/base_log.h"

#include <stdio.h>
#include <string.h>

// helpers
void posTracker(const char* data, Pos* pos)
{
    switch(data[pos->global]) {
        case ('\n'):
            pos->lineNb++;
            pos->line=1;
            break;

        case ('\t'):
            pos->line+=4;
            break;

        default:
            pos->line++;
            break;
    }
}

int isBackslashChar(size_t pos, const char* data)
{   // safe to use in if with no craches is pos == 0
    if (pos == 0)
        return 1;

    return data[pos-1] == '\\';
}


Token getToken(Pos pos, const char* data, size_t size, TokensType type)
{
    Token token;

    token.pos  = pos;
    token.type = type;
    data = &(data[pos.global]);

    // allocate memory and sotre raw data
    token.raw = (char*)malloc(sizeof(char) * (size+1));
    CHECK_ALLOCATE(token.raw, "Can't allocate memory for the token %s", data);
    token.raw[size] = '\0'; //nul terminetaed

    token.raw = memcpy(token.raw, data, size);
    CHECK_ALLOCATE(token.raw, "Can't copy %d chars frome the token %s", size, data)

    return token;
}

void addToken(Token** tokens, size_t* size, Token token)
{
    tokens[0] = realloc(tokens[0], sizeof(Token)*(size[0]+1));
    CHECK_ALLOCATE(tokens[0], "cant allocate memory for the token nb %d", size[0]+1)
    tokens[0][size[0]] = token;

    size[0]++;
}

// pos hadler
Pos getPos(size_t global, size_t line, size_t lineNb)
{
    Pos pos;
    pos.global = global;
    pos.line   = line;
    pos.lineNb = lineNb;

    return pos;
}

// pos hadler
Pos addPos(Pos a, Pos b)
{
    return getPos(a.global+b.global,
                  a.line+b.line,
                  a.lineNb+b.lineNb);
}


// main functions

Token* lexer(const char* data)
{
    // setup data
    Token* tokens = NULL;
    size_t nbTokens = 0;

    char specialChars[] = ";.:#,'\"()[]{}+-/*";
    char** KeyWords;

    // parse
    Pos globalPos = getPos(0, 1, 1);


    while (data[globalPos.global] != '\0')
    {
        // meta lain
        if (data[globalPos.global] == '#' && data[globalPos.global+1] == '{')
        {
            // find end token
            char found = 0;

            // keep track
            Pos tmpPos = addPos(globalPos, getPos(2, 1, 0));

            while (!found)
            {
                if (data[tmpPos.global] == '\0')
                    ERROR("Meta Lain Block opened at line %d char %d and never closed",
                           globalPos.lineNb, globalPos.line)

                if (data[tmpPos.global] == '}' && data[tmpPos.global+1] == '#') {
                    found = 1;
                }

                // keep tarck of pos
                posTracker(data, &tmpPos);
                tmpPos.global++;
            }

            // store token
            addToken(&tokens, &nbTokens,
                     getToken(globalPos, data, tmpPos.global+1-globalPos.global, LEX_METALAIN));

            // update pos
            globalPos = addPos(tmpPos, getPos(0, 2, 0));

        } else if(data[globalPos.global] == '"') {

            // detect a string

            // find the end of the string
            char found = 0;
            Pos tmpPos = addPos(globalPos, getPos(1, 1, 0));

            while (!found)
            {
                if (data[tmpPos.global] == '\0')
                    ERROR("String opened at line %d char %d and never closed", globalPos.lineNb, globalPos.line)

                if (data[tmpPos.global] == '\n')
                    ERROR("String opened at line %d char %d and never closed before a \\n", globalPos.lineNb, globalPos.line)

                if (data[tmpPos.global] == '\\')
                {
                    tmpPos = addPos(tmpPos, getPos(2, 2, 0));
                } else if (data[tmpPos.global] == '"') {
                    found = 1;
                } else {
                    // keep tarck of pos
                    posTracker(data, &tmpPos);
                    tmpPos.global++;
                }
            }

            // store token
            addToken(&tokens, &nbTokens,
                     getToken(globalPos,data, tmpPos.global+1-globalPos.global, LEX_STRING));

            // update pos
            globalPos = addPos(tmpPos, getPos(0, 1, 0));


        } else if(data[globalPos.global] == '/' &&
                 (data[globalPos.global+1] == '*' || data[globalPos.global+1] == '/')) {

            // ignore comments
            Pos tmpPos = addPos(globalPos, getPos(2, 2, 0));

            // single line
            if (data[globalPos.global+1] == '/') {
                while (data[tmpPos.global] != '\n' && data[tmpPos.global] != '\0')
                {
                    posTracker(data, &tmpPos);
                    tmpPos.global++;
                }

                // store token
                addToken(&tokens, &nbTokens,
                         getToken(globalPos, data, tmpPos.global+1-globalPos.global, LEX_COMMENT));

                // update pos
                posTracker(data, &tmpPos);
                globalPos = tmpPos;
            } else {
                // multi line
                char found = 0;

                while (!found)
                {
                    if (data[tmpPos.global] == '\0') {
                        found = 1;
                        tmpPos.line += 1;

                    } else if (data[tmpPos.global] == '*' && data[tmpPos.global+1] == '/'){
                        found = 1;
                        tmpPos = addPos(tmpPos, getPos(1, 2, 0));

                    } else {
                        posTracker(data, &tmpPos);
                        tmpPos.global++;
                    }
                }

                // store token
                addToken(&tokens, &nbTokens,
                         getToken(globalPos, data, tmpPos.global+1-globalPos.global, LEX_COMMENT));

                // update pos
                posTracker(data, &tmpPos);
                globalPos = tmpPos;
            }

        // TYPES
        } else if(isWord(data, globalPos.global, "int")) {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 3, LEX_INT_TYPE));
            globalPos = addPos(globalPos, getPos(2, 3, 0));

        } else if(isWord(data, globalPos.global, "float")) {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 5, LEX_FLOAT_TYPE));
            globalPos = addPos(globalPos, getPos(4, 5, 0));

        } else if(isWord(data, globalPos.global, "char")) {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 4, LEX_CHAR_TYPE));
            globalPos = addPos(globalPos, getPos(3, 4, 0));

        //LITTERALS
        } else if (isLiteralInt(data, globalPos.global)) {
            // find the size of the int
            int adder = 0;
            while ( 48 <= data[globalPos.global+adder] && data[globalPos.global+adder] <= 57 ) {
                adder += 1;
            }

            addToken(&tokens, &nbTokens, getToken(globalPos, data, adder, LEX_INT_LITERAL));
            globalPos = addPos(globalPos, getPos(adder-1, adder, 0));

        }else if (isLiteralFloat(data, globalPos.global)) {
            // find the size of the float
            int adder = 0;

            while ( (48 <= data[globalPos.global+adder] && data[globalPos.global+adder] <= 57) || data[globalPos.global+adder] == '.') {
                adder += 1;
            }

            addToken(&tokens, &nbTokens, getToken(globalPos, data, adder, LEX_FLOAT_LITERAL));
            globalPos = addPos(globalPos, getPos(adder-1, adder, 0));

        } else if (data[globalPos.global] == '\'') {
            // char
            Pos tmpPos = addPos(globalPos, getPos(1, 1, 0));

            // find end of char
            while (data[tmpPos.global] != '\'')
            {
                if (data[tmpPos.global] == '\0')
                    ERROR("Char opened at line %d char %d and never closed", globalPos.lineNb, globalPos.line)

                if (data[tmpPos.global] == '\n')
                    ERROR("Char opened at line %d char %d and never closed before a \\n", globalPos.lineNb, globalPos.line)

                if (data[tmpPos.global] == '\\')
                {
                    tmpPos.global += 2;
                    tmpPos.line += 2;
                } else {
                    posTracker(data, &tmpPos);
                    tmpPos.global++;
                }
            }

            // store token
            addToken(&tokens, &nbTokens,
                     getToken(globalPos, data, tmpPos.global+1-globalPos.global, LEX_CHAR_LITERAL));

            // update pos
            globalPos = addPos(tmpPos, getPos(0, 1, 0));

        // KEY WORDS
        } else if (isWord(data, globalPos.global, "if")) {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 2, LEX_IF));
            globalPos = addPos(globalPos, getPos(2, 3, 0));

        } else if (isWord(data, globalPos.global, "else")) {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 4, LEX_ELSE));
            globalPos = addPos(globalPos, getPos(4, 5, 0));

        } else if (isWord(data, globalPos.global, "while")) {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 5, LEX_WHILE));
            globalPos = addPos(globalPos, getPos(5, 6, 0));

        } else if (isWord(data, globalPos.global, "do")) {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 2, LEX_DO));
            globalPos = addPos(globalPos, getPos(2, 3, 0));

        } else if (isWord(data, globalPos.global, "for")) {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 3, LEX_FOR));
            globalPos = addPos(globalPos, getPos(3, 4, 0));

        } else if (isWord(data, globalPos.global, "return")) {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 6, LEX_RETURN));
            globalPos = addPos(globalPos, getPos(6, 7, 0));

        // IS NAME
        } else if (isName(data, globalPos.global)){
            // get the length of the name
            int adder = 0;

            while (valideNameChar(data[globalPos.global+adder]))
                adder ++;

            addToken(&tokens, &nbTokens, getToken(globalPos, data, adder, LEX_NAME));
            globalPos = addPos(globalPos, getPos(adder-1, adder, 0));

        // SPECIAL CHARS
        } else if(data[globalPos.global] == ';') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_SEMICOLONE));
            posTracker(data, &globalPos);
        
        } else if(data[globalPos.global] == '.') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_POINT));
            posTracker(data, &globalPos);

        } else if(data[globalPos.global] == ':') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_COLONE));
            posTracker(data, &globalPos);
        
        } else if(data[globalPos.global] == '#') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_HASHTAG));
            posTracker(data, &globalPos);
                            
        } else if(data[globalPos.global] == ',') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_COMMA));
            posTracker(data, &globalPos);
        
        } else if(data[globalPos.global] == '\'') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_SINGLEQUOTE));
            posTracker(data, &globalPos);
        
        } else if(data[globalPos.global] == '"') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_DOUBLEQUOTE));
            posTracker(data, &globalPos);

        } else if(data[globalPos.global] == '[') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_LBRACKET));
            posTracker(data, &globalPos);

        } else if(data[globalPos.global] == ']') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_RBRACKET));
            posTracker(data, &globalPos);

        } else if(data[globalPos.global] == '(') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_LPARENTHESIS));
            posTracker(data, &globalPos);

        } else if(data[globalPos.global] == ')') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_RPARENTHESIS));
            posTracker(data, &globalPos);
        
        } else if(data[globalPos.global] == '{') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_LBRACE));
            posTracker(data, &globalPos);
        
        } else if(data[globalPos.global] == '}') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_RBRACE));
            posTracker(data, &globalPos);
        
        } else if(data[globalPos.global] == '=') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_EQ));
            posTracker(data, &globalPos);

        } else if(data[globalPos.global] == '+') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_PLUS));
            posTracker(data, &globalPos);

        } else if(data[globalPos.global] == '-') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_MINUS));
            posTracker(data, &globalPos);

        } else if(data[globalPos.global] == '*') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_MULT));
            posTracker(data, &globalPos);

        } else if(data[globalPos.global] == '/') {
            addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_DIV));
            posTracker(data, &globalPos);

        // SEPATRATOR
        } else if(data[globalPos.global] == ' ') {
            //addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_SPACE));
            posTracker(data, &globalPos);
        
        } else if(data[globalPos.global] == '\t') {
            //addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_TAB));
            posTracker(data, &globalPos);
        
        } else if(data[globalPos.global] == '\n') {
            //addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_NLINE));
            posTracker(data, &globalPos);

        } else if(data[globalPos.global] == '\r') {
            //addToken(&tokens, &nbTokens, getToken(globalPos, data, 1, LEX_RETLINE));
            posTracker(data, &globalPos);

        } else {
            // calul pos
            ERROR("invalid token at line %d, local pos %d, global pos %d, raw: [ %.20s ]\n", 
                  globalPos.lineNb, globalPos.line, globalPos.global, &data[globalPos.global])
            posTracker(data, &globalPos);
        }

        globalPos.global++;
    }

    // set the the end token
    tokens = realloc(tokens, sizeof(Token)*(nbTokens+1));
    CHECK_ALLOCATE(tokens, "cant allocate memory for the token nb %d", nbTokens+1)
    tokens[nbTokens].type = LEX_ENDOFTOKENS;

    return tokens;
}

void showTokens(Token* tokens, int showRaw)
{
    size_t i = 0;
    INFO("Tokens:\n")

    while (tokens[i].type != LEX_ENDOFTOKENS)
    {
        printf("Type: { %s } Pos: (line:%d pos:%d global pos:%d )",
            TokenType2Char(tokens[i].type), tokens[i].pos.lineNb, tokens[i].pos.line, tokens[i].pos.global);

        if (showRaw)
            printf(" Raw: [ %s ]", tokens[i].raw);
        printf("\n");

        i++;
    }

    printf("\n");
    INFO("End Tokens")
}

char* TokenType2Char(TokensType type)
{
    switch(type){
        case LEX_IF:
            return "If";
        case LEX_ELSE:
            return "Else";
        case LEX_WHILE:
            return "While";
        case LEX_DO:
            return "Do";
        case LEX_FOR:
            return "For";
        case LEX_RETURN:
            return "Return";
        case LEX_INT_TYPE:
            return "IntType";
        case LEX_CHAR_TYPE:
            return "CharType";
        case LEX_FLOAT_TYPE:
            return "FloatType";
        case LEX_INT_LITERAL:
            return "IntLiteral";
        case LEX_CHAR_LITERAL:
            return "CharLiteral";
        case LEX_FLOAT_LITERAL:
            return "FloatLiteral";
        case LEX_SEMICOLONE:
            return "SemiColone";
        case LEX_POINT:
            return "Point";
        case LEX_COLONE:
            return "Colone";
        case LEX_HASHTAG:
            return "Hashtag";
        case LEX_COMMA:
            return "Comma";
        case LEX_SINGLEQUOTE:
            return "SingleQuote";
        case LEX_DOUBLEQUOTE:
            return "DoubleQuote";
        case LEX_LBRACKET:
            return "Lbracket";
        case LEX_RBRACKET:
            return "Rbracket";
        case LEX_LPARENTHESIS:
            return "Lparenthesis";
        case LEX_RPARENTHESIS:
            return "Rparenthesis";
        case LEX_LBRACE:
            return "Lbraces";
        case LEX_RBRACE:
            return "Rbraces";
        case LEX_PLUS:
            return "Plus";
        case LEX_MINUS:
            return "Minus";
        case LEX_MULT:
            return "Mult";
        case LEX_DIV:
            return "Div";
        case LEX_NAME:
            return "Names";
        case LEX_METALAIN:
            return "MetaLain";
        case LEX_STRING:
            return "Strings";
        case LEX_COMMENT:
            return "Comments";
        case LEX_SPACE:
            return "Space";
        case LEX_TAB:
            return "Tab";
        case LEX_NLINE:
            return "Nline";
        case LEX_RETLINE:
            return "Retline";
        case LEX_EQ:
            return "Equal";
        default:
            ERROR("Unknow token type %d", type)
    }
}

int isWord(const char* raw, size_t pos, char* word)
{
    size_t i=0;

    while (word[i] != '\0'){
        if (raw[pos+i] == '\0' || word[i] != raw[pos+i])
            return 0;
        i++;
    }

    if (pos == 0)
        return !(valideNameChar(raw[pos+i]));

    return (!valideNameChar(raw[pos+i])) && (!valideNameChar(raw[pos-1]));
}

int valideNameChar(char chr){

    if (chr == 95) // _
        return 1;

    if (48 <= chr && chr <= 57) // 0-9
        return 1;

    if (65 <= chr && chr <= 90) // A-Z
        return 1;

    if (97 <= chr && chr <= 119) // a-z
        return 1;

    return 0;
}

int isLiteralInt(const char* raw, size_t pos)
{
    // check that the character before isn't a valid varname char and dot
    if (pos > 0) {
        if (valideNameChar(raw[pos-1]) || raw[pos-1] == '.')
            return 0;
    }

    if ( 48 > raw[pos] || raw[pos] > 57)
        return 0;

    size_t i=1;
    while (48 <= raw[pos+i] && raw[pos+i] <= 57) {
        i++;
    }

    // check that the character after isn't a valid varname char and dot
    return !valideNameChar(raw[pos+i]) && raw[pos+i] != '.';
}

int isLiteralFloat(const char* raw, size_t pos)
{
    // check that the current char is a nb
    if ( (48 > raw[pos] || raw[pos] > 57) && raw[pos] != '.')
        return 0;

    // check the previous char
    if (pos > 0) {
        if (valideNameChar(raw[pos-1]) || raw[pos-1] == '.')
            return 0;
    }

    // if start by a .
    char foundDot = (raw[pos] == '.');

    // check chars
    size_t i=1;
    while ((48 <= raw[pos+i] && raw[pos+i] <= 57) || raw[pos+i] == '.') {
        if (raw[pos+i] == '.' && foundDot)
            return 0;

        if (raw[pos+i] == '.')
            foundDot = 1;
        i++;
    }

    // check end char
    return !valideNameChar(raw[pos+i]) && foundDot;
}


int isName(const char* raw, size_t pos)
{
    if (pos > 0) {
        if (valideNameChar(raw[pos-1]))
            return 0;
    }

    // check that the 1st is not a valid char but not number
    return valideNameChar(raw[pos]) && (48 > raw[pos] || raw[pos] > 57);
}


