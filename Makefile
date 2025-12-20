CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -Wall -Wextra -g

SRCDIR = src
OBJDIR = obj

LEXER_SRC = $(SRCDIR)/lexer.l
PARSER_SRC = $(SRCDIR)/parser.y
AST_SRC = $(SRCDIR)/ast.c
MAIN_SRC = $(SRCDIR)/main.c

LEXER_C = $(SRCDIR)/lex.yy.c
PARSER_C = $(SRCDIR)/parser.tab.c
PARSER_H = $(SRCDIR)/parser.tab.h

LEXER_OBJ = $(OBJDIR)/lex.yy.o
PARSER_OBJ = $(OBJDIR)/parser.tab.o
AST_OBJ   = $(OBJDIR)/ast.o
MAIN_OBJ  = $(OBJDIR)/main.o

TARGET = parser

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(PARSER_C) $(PARSER_H): $(PARSER_SRC)
	$(BISON) -d -o $(PARSER_C) $(PARSER_SRC)

$(LEXER_C): $(LEXER_SRC) $(PARSER_H)
	$(FLEX) -o $(LEXER_C) $(LEXER_SRC)

$(LEXER_OBJ): $(LEXER_C) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(LEXER_C) -o $(LEXER_OBJ)

$(PARSER_OBJ): $(PARSER_C) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(PARSER_C) -o $(PARSER_OBJ)

$(AST_OBJ): $(AST_SRC) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(AST_SRC) -o $(AST_OBJ)

$(MAIN_OBJ): $(MAIN_SRC) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $(MAIN_SRC) -o $(MAIN_OBJ)

$(TARGET): $(LEXER_OBJ) $(PARSER_OBJ) $(AST_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $^

clean:
	rm -f $(SRCDIR)/lex.yy.c $(SRCDIR)/parser.tab.c $(SRCDIR)/parser.tab.h
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
