# Simple-Compiler
Lexical and syntax phase of the compiler is implemented. Lexical analysis is done using 'lex' tool, the grammar for which is present in "lexicalanalyzer.l" and "header.h" contains auxillary information required in lexical phase.

LL(1) parser is implemented to use for parsing the program. 

Valid lexical units can be known from file "lexicalanalyzer.l" and syntax for the language could be known from "grammar.txt".

Test cases are included in subdirectory "TC".

Following the grammar of language, a program can be compiled by following instruction: ./runthis <file_name.in.
