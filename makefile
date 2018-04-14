OBJS = integer.o sll.o dll.o vertex.o edge.o queue.o binomial.o
OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -std=c99 -g -lm
EXECUTABLES = test-binomial

all:	$(EXECUTABLES)

test-binomial:	$(OBJS) test-binomial.o
		gcc $(LOPTS) $(OBJS) test-binomial.o -o test-binomial

integer.o:	integer.c integer.h
		gcc $(OOPTS) integer.c

sll.o:	sll.c sll.h
		gcc $(OOPTS) sll.c

dll.o:	dll.c dll.h
		gcc $(OOPTS) dll.c

vertex.o: 	vertex.c vertex.h
		gcc $(OOPTS) vertex.c

edge.o: 	edge.c edge.h
		gcc $(OOPTS) edge.c

queue.o: 	queue.c queue.h
		gcc $(OOPTS) queue.c

binomial.o: 	binomial.c binomial.h
		gcc $(OOPTS) binomial.c

test-binomial.o:	./Testing/test-binomial.c dll.h integer.h
		gcc $(OOPTS) ./Testing/test-binomial.c

test:	$(EXECUTABLES)
		@echo Testing test-binomial...
		@./test-binomial

valgrind:	$(EXECUTABLES)
		@echo Valgrind test-binomial...
		@valgrind ./test-binomial

clean:
		rm -f vgcore.* *.o $(EXECUTABLES)
