#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100
#define MAX_SYMBOLS 1000
typedef enum {
    ID, NB, INTEGER, CHAR, BEGIN, END, IF, ELSE, WHILE, READ, WRITE, WRITELN,
    OPREL, OPADD, OPMUL, ASSIGN, SEMICOLON, COMMA, COLON, PROGRAM, EOF_TOKEN, 
    UNKNOWN, THEN, DO, CHAR_LITERAL  
} TokenType;
typedef enum {
    TYPE_INTEGER,
    TYPE_CHAR,
    TYPE_ERROR
} DataType;
typedef struct {
    TokenType type;
    char lexeme[MAX_TOKEN_LENGTH];
    DataType dataType;  
} Token;
typedef struct {
    char name[MAX_TOKEN_LENGTH];
    DataType type;
    int isDeclared;
} SymbolTableEntry;
Token currentToken;
FILE *file;
FILE *fichierTraduction; // Fichier de traduction
int NumLigne = 1;
SymbolTableEntry symbolTable[MAX_SYMBOLS];
int symbolTableSize = 0;
Token anallex();
void accepter(TokenType expected);
void accepter_char(char expected);
void P();
void Dcl();
void InstComposee();
void Inst();
void Liste_inst();
void I();
void Exp();
void Exp_simple();
void Terme();
void Facteur();
void initSymbolTable();
void addSymbol(const char* name, DataType type);
SymbolTableEntry* findSymbol(const char* name);
DataType getExpressionType(Token* token);
int isTypeCompatible(DataType target, DataType source);
void semanticError(const char* message);

void initSymbolTable() {
    symbolTableSize = 0;
}
void emettre(FILE *fichier, const char *texte) {
    if (fichier != NULL) {
        fprintf(fichier, "%s", texte);
    } else {
        fprintf(stderr, "Erreur : fichier de traduction non ouvert.\n");
        exit(EXIT_FAILURE);
    }
}
// dans cette fonction on va ajouter les variables dans la table de symboles
void addSymbol(const char* name, DataType type) {
    for (int i = 0; i < symbolTableSize; i++) {// strcmp  retourne 0 si deux chaine sont egales
        if (strcmp(symbolTable[i].name, name) == 0) {
            if (!symbolTable[i].isDeclared) {
                symbolTable[i].type = type;
                symbolTable[i].isDeclared = 1;
                return;
            }
            semanticError("Redeclaration of variable");
            return;
        }
    }
    if (symbolTableSize < MAX_SYMBOLS) {
        strcpy(symbolTable[symbolTableSize].name, name);
        symbolTable[symbolTableSize].type = type;
        symbolTable[symbolTableSize].isDeclared = 1;
        symbolTableSize++;
    } else {
        semanticError("Symbol table overflow");
    }
}

SymbolTableEntry* findSymbol(const char* name) {
    for (int i = 0; i < symbolTableSize; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return &symbolTable[i];
        }
    }
    return NULL;
}

void semanticError(const char* message) {
    printf("Semantic Error: %s at line %d\n", message, NumLigne);
    exit(1);
}
//semantique
DataType getExpressionType(Token* token) {
    SymbolTableEntry* entry;
    switch (token->type) {
        case NB:
            return TYPE_INTEGER;
        case CHAR_LITERAL:
            return TYPE_CHAR;
        case ID:
            entry = findSymbol(token->lexeme);
            if (entry == NULL) {
                addSymbol(token->lexeme, TYPE_ERROR);
                return TYPE_ERROR;
            }
            return entry->type;
        default:
            return TYPE_ERROR;
    }
}
//semantique
int isTypeCompatible(DataType target, DataType source) {
    if (target == TYPE_ERROR || source == TYPE_ERROR)// continuer l analyse meme s y l a des erreurs de type
        return 1; 

    return (target == source);
}
Token anallex() {
    Token token;
    char c;
    int i = 0;
    //Ignore les espaces blancs et compte les nouvelles lignes
    while ((c = fgetc(file)) != EOF && isspace(c)) {
        if (c == '\n') NumLigne++;
    }
    // Vérifie la fin de fichier
    if (c == EOF) {
        token.type = EOF_TOKEN;
        return token;
    }
    //lit caractere de format '@'
    if (c == '\'') {
        token.type = CHAR_LITERAL;
        c = fgetc(file);
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
        c = fgetc(file);  // lit caractere par caractere de fichier 
        if (c != '\'') {
            printf("Error at line %d: Unterminated character literal\n", NumLigne);
            exit(1);
        }
        return token;
    }

    if (isalpha(c)) {
        token.lexeme[i++] = c;
        while ((c = fgetc(file)) != EOF && (isalnum(c) || c == '_')) {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        ungetc(c, file);

        if (strcmp(token.lexeme, "program") == 0) token.type = PROGRAM;
        else if (strcmp(token.lexeme, "integer") == 0) token.type = INTEGER;
        else if (strcmp(token.lexeme, "char") == 0) token.type = CHAR;
        else if (strcmp(token.lexeme, "begin") == 0) token.type = BEGIN;
        else if (strcmp(token.lexeme, "end") == 0) token.type = END;
        else if (strcmp(token.lexeme, "if") == 0) token.type = IF;
        else if (strcmp(token.lexeme, "else") == 0) token.type = ELSE;
        else if (strcmp(token.lexeme, "while") == 0) token.type = WHILE;
        else if (strcmp(token.lexeme, "read") == 0) token.type = READ;
        else if (strcmp(token.lexeme, "write") == 0) token.type = WRITE;
        else if (strcmp(token.lexeme, "writeln") == 0) token.type = WRITELN;
        else if (strcmp(token.lexeme, "then") == 0) token.type = THEN;
        else if (strcmp(token.lexeme, "do") == 0) token.type = DO;
        else token.type = ID;

        return token;
    }

    if (isdigit(c)) {
        token.lexeme[i++] = c;
        while ((c = fgetc(file)) != EOF && isdigit(c)) {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        ungetc(c, file);
        token.type = NB;
        return token;
    }

    token.lexeme[0] = c;
    token.lexeme[1] = '\0';

switch (c) {
    case '=':
        token.type = OPREL;
        break;
    case '<':
        c = fgetc(file);
        if (c == '>') {
            token.type = OPREL;
            strcat(token.lexeme, "<>");
        } else if (c == '=') {
            token.type = OPREL;
            strcat(token.lexeme, "<=");
        } else {
            token.type = OPREL;
            ungetc(c, file);
        }
        break;
    case '>':
        c = fgetc(file);
        if (c == '=') {
            token.type = OPREL;
            strcat(token.lexeme, ">=");
        } else {
            token.type = OPREL;
            ungetc(c, file);
        }
        break;
    case '+':
        token.type = OPADD;
        break;
    case '-':
        token.type = OPADD;
        break;
    case '|':
        c = fgetc(file);
        if (c == '|') {
            token.type = OPADD;
            strcat(token.lexeme, "||");
        } else {
            token.type = UNKNOWN;
            ungetc(c, file);
        }
        break;
    case '*':
        token.type = OPMUL;
        break;
    case '/':
        token.type = OPMUL;
        break;
    case '%':
        token.type = OPMUL;
        break;
    case '&':
        c = fgetc(file);
        if (c == '&') {
            token.type = OPMUL;
            strcat(token.lexeme, "&&");
        } else {
            token.type = OPMUL;
            ungetc(c, file);
        }
        break;
    case ':':
        if ((c = fgetc(file)) == '=') {
            token.type = ASSIGN;
            strcat(token.lexeme, "=");
        } else {
            token.type = COLON;
            ungetc(c, file);
        }
        break;
    case ';':
        token.type = SEMICOLON;
        break;
    case ',':
        token.type = COMMA;
        break;
    default:
        token.type = UNKNOWN;
        break;
}

    return token;
}
void accepter(TokenType expected) {
    if (currentToken.type == expected) {
        currentToken = anallex();
    } else {
        printf("Syntactic Error at line %d: unexpected token '%s'\n", NumLigne, currentToken.lexeme);
        exit(1);
    }
}

void accepter_char(char expected) {
    if (currentToken.lexeme[0] == expected && currentToken.lexeme[1] == '\0') {
        currentToken = anallex();
    } else {
        printf("Syntactic Error at line %d: unexpected character '%s'\n", NumLigne, currentToken.lexeme);
        exit(1);
    }
}

void Dcl() {
    if (currentToken.type == ID) {
        char varName[MAX_TOKEN_LENGTH];
        strcpy(varName, currentToken.lexeme);
       
        accepter(ID);
        accepter(COLON);

        DataType varType;
        if (currentToken.type == INTEGER) {
            varType = TYPE_INTEGER;
            accepter(INTEGER);
        } else if (currentToken.type == CHAR) {
            varType = TYPE_CHAR;
            accepter(CHAR);
        } else {
            semanticError("Invalid type declaration");
            return;
        }
        addSymbol(varName, varType);

        accepter(SEMICOLON);
        Dcl();
    }
}

void InstComposee() {
    accepter(BEGIN);
    Liste_inst();  
    accepter(END);
}

void Inst() {
    if (currentToken.type == ID || currentToken.type == IF ||
        currentToken.type == WHILE || currentToken.type == READ ||
        currentToken.type == WRITE || currentToken.type == WRITELN ||
        currentToken.type == BEGIN) {  
        Liste_inst();
    }
}

void Liste_inst() {
    I();
    while (currentToken.type == SEMICOLON) {
        accepter(SEMICOLON);
        if (currentToken.type != END) {  
            I();
        }
    }
}

void I() {
   
    switch(currentToken.type) {
        case ID:
        case IF:
        case WHILE:
        case READ:
        case WRITE:
        case WRITELN:
        case BEGIN:
            break;
        default:
            return;  
    }
    if (currentToken.type == ID) {
        char varName[MAX_TOKEN_LENGTH];
        strcpy(varName, currentToken.lexeme);
        SymbolTableEntry* entry = findSymbol(currentToken.lexeme);
        if (entry == NULL) {
            semanticError("Undeclared variable");
            return;
        }
        DataType targetType = entry->type;
        emettre(fichierTraduction, varName);
        emettre(fichierTraduction, "\n");
        accepter(ID);
        
        accepter(ASSIGN);
        Token expToken = currentToken;
        Exp_simple();
        DataType sourceType = getExpressionType(&expToken);
        if (!isTypeCompatible(targetType, sourceType)) {
            semanticError("Type misaccepter in assignment");
        }
    }
    else if (currentToken.type == IF) {
         emettre(fichierTraduction, "SI VRAI\n");
        accepter(IF);
        Exp();  
        accepter(THEN);
        I();   
        if (currentToken.type == ELSE) {
            emettre(fichierTraduction, "ELSE\n");
            accepter(ELSE);
            I(); 
        }
    }
    else if (currentToken.type == WHILE) {
         emettre(fichierTraduction, " WHILE\n");
        accepter(WHILE);
        Exp();  
        accepter(DO);
        I();    
    }
    else if (currentToken.type == READ) {
        accepter(READ);
        accepter_char('(');
        if (currentToken.type != ID) {
            semanticError("Expected identifier in read statement");
        }
      
        if (findSymbol(currentToken.lexeme) == NULL) {
            semanticError("Undeclared variable in read statement");
        }
        accepter(ID);
        accepter_char(')');
    }
    else if (currentToken.type == WRITE || currentToken.type == WRITELN) {
        TokenType ioType = currentToken.type;
        accepter(ioType);
        accepter_char('(');
        if (currentToken.type != ID) {
            semanticError("Expected identifier in write statement");
        }
     
        if (findSymbol(currentToken.lexeme) == NULL) {
            semanticError("Undeclared variable in write statement");
        }
        accepter(ID);
        accepter_char(')');
    }
    else {
        semanticError("Invalid instruction");
    }
}

void P() {
    accepter(PROGRAM);
    accepter(ID);
    accepter(SEMICOLON);
    Dcl();
    InstComposee();
    accepter_char('.');
}

void Exp() {
    Exp_simple();
    if (currentToken.type == OPREL) {
        emettre(fichierTraduction, currentToken.lexeme);
        accepter(OPREL);
        Exp_simple();
    }
}

void Exp_simple() {
    Terme();
    while (currentToken.type == OPADD) {
         emettre(fichierTraduction, currentToken.lexeme);
        accepter(OPADD);
        Terme();
    }
}

void Terme() {
    Facteur();
    while (currentToken.type == OPMUL) {
         emettre(fichierTraduction, currentToken.lexeme);
        accepter(OPMUL);
        Facteur();
    }
}

void Facteur() {
    if (currentToken.type == ID) {
       
        SymbolTableEntry* entry = findSymbol(currentToken.lexeme);
        if (entry == NULL) {
            semanticError("Undeclared variable");
            return;
        }
        accepter(ID);
    } else if (currentToken.type == NB) {
        
        accepter(NB);
    } else if (currentToken.type == CHAR_LITERAL) {
        accepter(CHAR_LITERAL);
    } else if (currentToken.lexeme[0] == '(') {
        accepter_char('(');
        Exp_simple();
        accepter_char(')');
    } else {
        printf("Error at line %d: Expected identifier, number, character literal, or '(' but got '%s'\n", 
               NumLigne, currentToken.lexeme);
        semanticError("Invalid factor in expression");
    }
}

int main() {
    file = fopen("C:\\Users\\amine\\Desktop\\sfax\\test.txt", "r");

    if (file == NULL) {
        printf("Error: Cannot open file 'test.txt'\n");
        return 1;
    }


    initSymbolTable();

    currentToken = anallex();
    P();

    fclose(file);
    printf("Syntactic and Semantic Analysis Successful!\n");
     fichierTraduction = fopen("traduction.txt", "w");
    if (fichierTraduction == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier de traduction.\n");
        return 1;
    }

    emettre(fichierTraduction, "Début de la traduction\n");
    // Ajoutez ici les appels à emettre pour écrire les traductions
    emettre(fichierTraduction, "Fin de la traduction\n");

    fclose(fichierTraduction);
    return 0;
}