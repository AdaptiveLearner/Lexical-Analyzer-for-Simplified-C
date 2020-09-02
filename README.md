# Lexical Scanner for Simplified C
Albert Wu, 2020.

To compile:
gcc lex_analysis.c -o executionfilename

Test C statements (lines 1126~1130):
    char *s[] = {"if(a == 0)$", 
                 "i=2*k+3;$", 
                 "double pi=3.1416;$",
                 "for(i=0; i<10; i++) {k++;}$",
                 "i=6; t=2; while(i>=0) {i-=1; t=2*t+1;}$"};

Design principles:
    Algorithms based on [1]
    Automata using tables based on [2]
    Textbook [3]
    Simplified C grammar [4]

[1] C. N. Fischer, et al., Crafting a Compiler, Pearson, 2009.
[2] A. W. Appel, Modern Compiler Implementation in C, Cambridge University Press, 2008.
[3] A. V. Aho, et al., Compilers: Principles, Techniques, and Tools, Addison Wesley, 2006.
[4] ANSI C grammar, Lex specification, https://www.lysator.liu.se/c/ANSI-C-grammar-l.html.