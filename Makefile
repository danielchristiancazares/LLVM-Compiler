.PHONY: clean strip depend

COMPILER = glc
PRODUCTS = $(COMPILER) 
default: $(PRODUCTS)

SRCDIR = ./src
OBJDIR = ./obj

SRCS = $(wildcard $(SRCDIR)/*.cc)
OBJS = $(patsubst %.cc, $(OBJDIR)/%.o, $(notdir $(SRCS)))

JUNK =  $(OBJDIR)/*.o lex.yy.c dpp.yy.c y.tab.c y.tab.h *.core core *~

CC= g++
LD = g++
LEX = flex
YACC = bison

CFLAGS = -g -Wall -Wno-unused -Wno-sign-compare `llvm-config --cxxflags` 
LEXFLAGS = -d
YACCFLAGS = -dvty

LIBS = -lc -lm -ll `llvm-config --ldflags --libs`

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(CC) $(CFLAGS) -c -o $@ $<

lex.yy.c: scanner.l
	$(LEX) $(LEXFLAGS) scanner.l

y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c -o y.tab.o y.tab.c

y.tab.h y.tab.c: parser.y
	$(YACC) $(YACCFLAGS) parser.y

$(COMPILER):  $(OBJS)
	$(LD) -o $@ $^ $(LIBS)

strip : $(PRODUCTS)
	strip $(PRODUCTS)
	rm -rf $(JUNK)

depend:
	$(CC) -MM $(SRCS) > .depend

-include .depend

clean:
	rm -f $(JUNK) y.output $(PRODUCTS)
