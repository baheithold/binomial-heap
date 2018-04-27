#Makefile modified from magarwal on the Beastie forums.
#Created 03/23/2018.

OBJS 	= integer.o real.o string.o dll.o sll.o queue.o binomial.o
OOPTS 	= -Wall -Wextra -std=c99 -g -c
LOPTS 	= -Wall -Wextra -std=c99 -g -lm
TESTS 	= binomial-0-0 binomial-0-1 binomial-0-2 binomial-0-3 binomial-0-4 \
		  binomial-0-5 binomial-0-6 binomial-0-7 binomial-0-8 binomial-0-9 \
		  binomial-0-10

all: 	$(OBJS) $(TESTS) test-binomial

################################################################################
#                                                 Classes for Primitive Types
#INTEGER
integer.o: 	integer.c integer.h
	gcc $(OOPTS) integer.c

#REAL
real.o: 	real.c real.h
	gcc $(OOPTS) real.c

#STRING
string.o: 	string.c string.h
	gcc $(OOPTS) string.c

################################################################################
#                                                                         SLL

sll.o: 	sll.c sll.h
	gcc $(OOPTS) sll.c

################################################################################
#                                                                         DLL

dll.o: 	dll.c dll.h
	gcc $(OOPTS) dll.c

################################################################################
#                                                                         QUEUE

queue.o: 	queue.c queue.h sll.h
	gcc $(OOPTS) queue.c

################################################################################
#                                                                         BINOMIAL

binomial.o: 	binomial.c binomial.h dll.h
	gcc $(OOPTS) binomial.c

test-binomial.o: 	./Testing/test-binomial.c binomial.c binomial.h dll.h
	gcc $(OOPTS) ./Testing/test-binomial.c

################################################################################
#                                                Test everything except trees

test: 	all
	@for x in $(TESTS); do \
		echo Testing $$x...; \
		./$$x > ./Testing/0/actual/$$x.actual; \
		diff ./Testing/0/expected/$$x.expected ./Testing/0/actual/$$x.actual; \
		done

################################################################################
#                                            							Valgrind

valgrind: 	all
	@for x in $(TESTS); do \
		echo valgrind $$x; \
		echo -------------------------; \
		valgrind ./$$x; echo; \
		echo -------------------------; \
		echo; \
		done

################################################################################
#                                                         				Clean

clean:
	rm -f *.o vgcore.* $(TESTS)

################################################################################
#                                                                		tests

binomial: 	$(TESTS) $(OBJS)
	for x in $(TESTS); do \
		make $$x; \
		done

$(TESTS): 	%: ./Testing/0/%.c binomial.o
	gcc $(LOPTS) $(OBJS) $< -o $@

test-binomial: 	integer.o dll.o sll.o queue.o binomial.o test-binomial.o
	gcc $(LOPTS) integer.o dll.o sll.o queue.o binomial.o test-binomial.o -o test-binomial
