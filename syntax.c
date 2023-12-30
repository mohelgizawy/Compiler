#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

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

Token currentToken;

void term(FILE *input); 
Token getNextToken(FILE *input);
void match(TokenType expectedType,FILE *input);
void factor(FILE *input);
void expression (FILE *input);


int main() {
    FILE *inputFile = fopen("syntacinput.txt", "r");
    if (!inputFile) {
        perror("Error opening file");
        return 1;
    }

    currentToken = getNextToken(inputFile);
    expression(inputFile);

    if (currentToken.type == END) {
        printf("Syntax analysis successful!\n");
    } else {
        printf("Error: Unexpected token at the end\n");
    }

    fclose(inputFile);
    return 0;
}


Token getNextToken(FILE *input) {
    Token token;
    char c;
    int i = 0;

    // Skip white spaces
    while ((c = fgetc(input)) != EOF && (isspace(c) || c=='\n'))
        ;

    // Check for end of file
    if (c == EOF) {
        token.type = END;
        return token;
    }

    // Identify the token type
    if (isalpha(c)) {
        token.type = IDENTIFIER;
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

void match(TokenType expectedType, FILE *input) {
    if (currentToken.type == expectedType) {
        currentToken = getNextToken(input);
    } else {
        printf("Error: Expected %d but got %d\n", expectedType, currentToken.type);
        // Handle error or exit
    }
}



void factor(FILE *input) {
    if (currentToken.type == IDENTIFIER || currentToken.type == LITERAL) {
        match(currentToken.type, input);
    } else if (currentToken.type == OPERATOR && currentToken.lexeme[0] == '(' ) {
        match(OPERATOR, input);
        expression(input);
        match(OPERATOR, input);
    } else {
        printf("Error: Unexpected token in factor - lexeme: %s\n", currentToken.lexeme);
        // Handle error or exit
    }
}

void expression(FILE *input) {
    term(input);
    while (currentToken.type == OPERATOR && (currentToken.lexeme[0] == '+' || currentToken.lexeme[0] == '-')) {
        match(OPERATOR, input);
        term(input);
    }
}


void term(FILE *input) {
    factor(input);
    while (currentToken.type == OPERATOR && (currentToken.lexeme[0] == '*' || currentToken.lexeme[0] == '/')) {
        match(OPERATOR, input);
        factor(input);
    }
}







