OBJS = integer.o dll.o
OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -std=c99 -g
EXECUTABLES = test-binomial

all:	$(EXECUTABLES)

test-binomial:	$(OBJS) test-binomial.o
		gcc $(LOPTS) $(OBJS) test-binomial.o -o test-binomial

integer.o:	integer.c integer.h
		gcc $(OOPTS) integer.c

dll.o:	dll.c dll.h
		gcc $(OOPTS) dll.c

test-binomial.o:	test-binomial.c dll.h integer.h
		gcc $(OOPTS) test-binomial.c

test:	$(EXECUTABLES)
		@echo Testing test-binomial...
		@./test-binomial

valgrind:	$(EXECUTABLES)
		@echo Valgrind test-binomial...
		@valgrind ./test-binomial

clean:
		rm -f vgcore.* *.o $(EXECUTABLES)