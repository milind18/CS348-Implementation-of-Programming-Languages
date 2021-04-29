Name: Milind B Prabhu
Roll: 180101091

Instructions to compile the code on a linux machine:

1. The lex specification is in 180101091_lex.l . To compile it from the terminal use:
$ lex 180101091_lex.l

2. After step 1 which generates lex.yy.c, to compile tokenizer.cpp and generate the executable run:
$ g++ 180101091_tokenizer.cpp lex.yy.c

3. To run the executable on input.pas use:
$ ./a.out < input.pas


Some Helpful information:
1. The input_id() and input_num() have been implemented at the end of the lex.l file. 
2. The codes.h header file contains the definitions of macros and the codes corresponding to different tokens.
3. The hash_table.h contains the definition of the entry_table struct and the definition of the hash function used to insert elements in the hash table.
4. The output that is printed when a.out is executed is due to the code in tokenizer.cpp
