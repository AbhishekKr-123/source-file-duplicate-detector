#ifndef TOKENIZER_H
#define TOKENIZER_H

enum TokenType {
    TOK_KEYWORD = 0,
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_OPERATOR,
    TOK_SEPARATOR,
    TOK_BRACKET,
    TOK_STRING,
    TOK_CHARLIT,
    TOK_UNKNOWN
};

struct Token {
    char value[100];
    int type;
};

int tok_is_keyword(const char *w);
const char *tok_type_name(int type);
int tok_tokenize(const char *src, struct Token **out_tokens);
void tok_normalize_identifiers(struct Token *tokens, int n);
void tok_free(struct Token *t);

#endif
