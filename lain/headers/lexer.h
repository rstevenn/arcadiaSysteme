#ifndef __TOKENS_H__
#define __TOKENS_H__

#include <stdlib.h>

typedef enum {
    // keywords
    LEX_IF,
    LEX_ELSE,
    LEX_WHILE,
    LEX_DO,
    LEX_FOR,
    LEX_RETURN,

    // types
    LEX_INT_TYPE,
    LEX_CHAR_TYPE,
    LEX_FLOAT_TYPE,

    // literals
    LEX_INT_LITERAL,
    LEX_CHAR_LITERAL,
    LEX_FLOAT_LITERAL,

    // specials chars
    LEX_SEMICOLONE,
    LEX_POINT,
    LEX_COLONE,
    LEX_HASHTAG,
    LEX_COMMA,
    LEX_SINGLEQUOTE,
    LEX_DOUBLEQUOTE,

    LEX_LBRACKET, // []
    LEX_RBRACKET,
    LEX_LPARENTHESIS, // ()
    LEX_RPARENTHESIS,
    LEX_LBRACE, // {}
    LEX_RBRACE,

    // calc
    LEX_PLUS,
    LEX_MINUS,
    LEX_MULT,
    LEX_DIV,
    LEX_EQ,
    LEX_GT,
    LEX_LT,

    // names
    LEX_NAME,

    // spécial type
    LEX_METALAIN,
    LEX_STRING,
    LEX_COMMENT,

    // separetor
    LEX_SPACE,
    LEX_TAB,
    LEX_NLINE,
    LEX_RETLINE,

    LEX_ENDOFTOKENS
} TokensType;

typedef struct {
    size_t global;
    size_t line;
    size_t lineNb;
} Pos;

typedef struct {
    Pos pos;
    char* raw;
    TokensType type;
} Token;



// pos handler
Pos getPos(size_t global, size_t line, size_t lineNb);

// main
Token* lexer(const char* data);
void showTokens(Token* tokens, int showRaw);
char* TokenType2Char(TokensType type);

// string functions
int isWord(const char* raw, size_t pos, char* word);
int valideNameChar(char chr);

// numbers function
int isLiteralInt(const char* raw, size_t pos);
int isLiteralFloat(const char* raw, size_t pos);
int isLiteralFloat(const char* raw, size_t pos);
int isName(const char* raw, size_t pos);

#endif