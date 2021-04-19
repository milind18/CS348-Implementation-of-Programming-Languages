parser: lex.yy.c y.tab.c
	gcc -g -w lex.yy.c y.tab.c -o pasc

y.tab.c: pasc.y
	yacc -d pasc.y

lex.yy.c: y.tab.c pasc.l
	lex pasc.l

clean: 
	rm -rf lex.yy.c y.tab.c y.tab.h pasc