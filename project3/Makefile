CC=gcc
FLEX=flex
BISON=bison

.lex: lex.l
	$(FLEX) lex.l
.syntax: syntax.y
	$(BISON) -t -d syntax.y
splc: .lex .syntax
	@mkdir bin
	touch bin/splc
	@chmod +x bin/splc
	$(CC) syntax.tab.c tree.c visit.c idtype.c symtab_ll.c stack.c -lfl -ly -o ./bin/splc
clean:
	@rm -rf bin/
.PHONY: splc