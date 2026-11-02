#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokenizer.h"

static const char *keywords[] = {
    "auto","break","case","char","const","continue","default","do","double",
    "else","enum","extern","float","for","goto","if","inline","int","long",
    "register","restrict","return","short","signed","sizeof","static","struct",
    "switch","typedef","union","unsigned","void","volatile","while",
    "_Bool","_Complex","_Imaginary", NULL
};

int tok_is_keyword(const char *w) {
    int i;
    for (i = 0; keywords[i]; i++) {
        if (strcmp(keywords[i], w) == 0) return 1;
    }
    return 0;
}

const char *tok_type_name(int type) {
    switch (type) {
    case TOK_KEYWORD: return "Keyword";
    case TOK_IDENTIFIER: return "Identifier";
    case TOK_NUMBER: return "Number";
    case TOK_OPERATOR: return "Operator";
    case TOK_SEPARATOR: return "Separator";
    case TOK_BRACKET: return "Bracket";
    case TOK_STRING: return "String";
    case TOK_CHARLIT: return "Char";
    default: return "Unknown";
    }
}

static int is_op_char(char c) {
    return strchr("+-*/%=<>!&|^~?:.", c) != NULL;
}

static int match_multiop(const char *s, char *out) {
    static const char *ops[] = {
        "<<=", ">>=", "==", "!=", "<=", ">=", "&&", "||", "++", "--",
        "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<", ">>", "->", NULL
    };
    int i;
    for (i = 0; ops[i]; i++) {
        size_t L = strlen(ops[i]);
        if (strncmp(s, ops[i], L) == 0) {
            strcpy(out, ops[i]);
            return (int)L;
        }
    }
    return 0;
}

int tok_tokenize(const char *src, struct Token **out_tokens) {
    size_t cap = 256, n = 0, i = 0, len;
    struct Token *toks;
    if (!src || !out_tokens) return -1;
    len = strlen(src);
    toks = (struct Token *)malloc(cap * sizeof *toks);
    if (!toks) return -1;
    while (i < len) {
        char c = src[i];
        if (isspace((unsigned char)c)) { i++; continue; }
        if (n >= cap) {
            cap *= 2;
            toks = (struct Token *)realloc(toks, cap * sizeof *toks);
            if (!toks) return -1;
        }
        /* identifier / keyword */
        if (isalpha((unsigned char)c) || c == '_') {
            size_t s = i, k = 0;
            while (i < len && (isalnum((unsigned char)src[i]) || src[i] == '_')) {
                if (k < 99) toks[n].value[k++] = src[i];
                i++;
            }
            toks[n].value[k] = '\0';
            (void)s;
            toks[n].type = tok_is_keyword(toks[n].value) ? TOK_KEYWORD : TOK_IDENTIFIER;
            n++;
        } else if (isdigit((unsigned char)c)) {
            size_t k = 0;
            int dot = 0;
            while (i < len && (isalnum((unsigned char)src[i]) || src[i] == '.' || src[i] == '_')) {
                if (src[i] == '.') { if (dot) break; dot = 1; }
                if (k < 99) toks[n].value[k++] = src[i];
                i++;
                if (k > 0 && !isdigit((unsigned char)src[i]) && src[i] != '.' &&
                    !isalpha((unsigned char)src[i]) && src[i] != '_') break;
            }
            toks[n].value[k] = '\0';
            toks[n].type = TOK_NUMBER;
            n++;
        } else if (c == '"') {
            size_t k = 0;
            toks[n].value[k++] = c; i++;
            while (i < len) {
                char d = src[i++];
                if (k < 99) toks[n].value[k++] = d;
                if (d == '\\' && i < len) { if (k < 99) toks[n].value[k++] = src[i]; i++; }
                else if (d == '"') break;
            }
            toks[n].value[k < 100 ? k : 99] = '\0';
            toks[n].type = TOK_STRING;
            n++;
        } else if (c == '\'') {
            size_t k = 0;
            toks[n].value[k++] = c; i++;
            while (i < len) {
                char d = src[i++];
                if (k < 99) toks[n].value[k++] = d;
                if (d == '\\' && i < len) { if (k < 99) toks[n].value[k++] = src[i]; i++; }
                else if (d == '\'') break;
            }
            toks[n].value[k < 100 ? k : 99] = '\0';
            toks[n].type = TOK_CHARLIT;
            n++;
        } else if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']') {
            toks[n].value[0] = c; toks[n].value[1] = '\0';
            toks[n].type = TOK_BRACKET; i++; n++;
        } else if (c == ',' || c == ';' || c == '#') {
            toks[n].value[0] = c; toks[n].value[1] = '\0';
            toks[n].type = TOK_SEPARATOR; i++; n++;
        } else if (is_op_char(c)) {
            char buf[8]; int L = match_multiop(src + i, buf);
            if (L > 0) {
                strcpy(toks[n].value, buf); i += (size_t)L;
            } else {
                toks[n].value[0] = c; toks[n].value[1] = '\0'; i++;
            }
            toks[n].type = TOK_OPERATOR; n++;
        } else {
            toks[n].value[0] = c; toks[n].value[1] = '\0';
            toks[n].type = TOK_UNKNOWN; i++; n++;
        }
    }
    *out_tokens = toks;
    return (int)n;
}

void tok_normalize_identifiers(struct Token *tokens, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (tokens[i].type == TOK_IDENTIFIER) {
            strcpy(tokens[i].value, "ID");
        }
    }
}

void tok_free(struct Token *t) { free(t); }
