#include <stdio.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 100

typedef enum {
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    LITERAL,
    END
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LEN];
} Token;

Token getNextToken(FILE *input) {
    Token token;
    char c;
    int i = 0;

    // Skip white spaces
    while ((c = fgetc(input)) && isspace(c))
        ;

    // Check for end of file
    if (c == EOF) {
        token.type = END;
        return token;
    }

    // Identify the token type
    if (isalpha(c)) {
        token.type = KEYWORD;
    } else if (isdigit(c)) {
        token.type = LITERAL;
    } else {
        token.type = OPERATOR;
    }

    // Build the lexeme
    do {
        token.lexeme[i++] = c;
        c = fgetc(input);
    } while (c != EOF && (isalnum(c) || c == '.'));

    ungetc(c, input);
    token.lexeme[i] = '\0';

    return token;
}

int main() {
    FILE *inputFile = fopen("lexicalinput.txt", "r");
    if (!inputFile) {
        perror("Error opening file");
        return 1;
    }

    Token token;
    do {
        token = getNextToken(inputFile);
        switch (token.type) {
            case KEYWORD:
                printf("Keyword: %s\n", token.lexeme);
                break;
            case IDENTIFIER:
                printf("Identifier: %s\n", token.lexeme);
                break;
            case OPERATOR:
                printf("Operator: %s\n", token.lexeme);
                break;
            case LITERAL:
                printf("Literal: %s\n", token.lexeme);
                break;
            case END:
                printf("End of File\n");
                break;
        }
    } while (token.type != END);

    fclose(inputFile);
    return 0;
}