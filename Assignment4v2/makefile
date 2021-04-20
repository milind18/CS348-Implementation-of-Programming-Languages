parser: lex.yy.c y.tab.c hash_table.c
	gcc -g lex.yy.c y.tab.c hash_table.c -o pasc

y.tab.c: pasc.y
	yacc -d pasc.y

lex.yy.c: y.tab.c pasc.l
	lex pasc.l


clean: 
	rm -rf lex.yy.c y.tab.c y.tab.h pasc