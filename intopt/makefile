CC	= cc

# put the one you prefer last
CFLAGS	= -g -O3 -std=c18 -pedantic -Werror -mcpu=power8 -pthread
CFLAGS	= -g -O3 -std=c18 -pedantic -Werror -Wall
CFLAGS	= -g

SANFLAGS = -g -fno-common -fsanitize=address

LDFLAGS	= -g -pthread -lm

OBJS	= intopt.o main.o error.o


main: compile
	./a.out -t 4

compile: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS)
	rm -f score

test_compile: $(OBJS)
	gcc $(CFLAGS) test_main.c intopt.c -o test.out $(LDFLAGS)

test_hugo_compile: $(OBJS)
	gcc intopt_hugo.c -o test_hugo.out -lm

test_sanitizer: $(OBJS)
	gcc $(CFLAGS) test_main.cintopt.c -o test.out $(SANFLAGS) $(LDFLAGS)
	./test.out < is/i

test: test_compile
	./test.out

test_hugo: test_hugo_compile
	./test_hugo.out

valgrind: compile
	valgrind --leak-check=full --show-reachable=yes ./a.out

test_valgrind: test_compile
	timeout 10 valgrind ./test.out < is/i6

sanitizer: $(OBJS)
	$(CC) $(OBJS) $(SANFLAGS) $(LDFLAGS)
	rm -f score
	./a.out -t 4

clean:
	rm -f *.o a.out

# for use on power.cs.lth.se and other linux machines with oprofile
operf: a.out
	operf -e CYCLES:100000:0:0:1 ./a.out -t 120
