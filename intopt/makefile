CC	= cc

# put the one you prefer last
CFLAGS	= -g -O3 -std=c18 -pedantic -Werror -mcpu=power8 -pthread
CFLAGS	= -g -O3 -std=c18 -pedantic -Werror
CFLAGS	= -g

LDFLAGS	= -g -pthread -lm

OBJS	= intopt.o main.o error.o

main: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS)
	rm -f score
	./a.out -t 4

clean:
	rm -f *.o a.out

# for use on power.cs.lth.se and other linux machines with oprofile
operf: a.out
	operf -e CYCLES:100000:0:0:1 ./a.out -t 120
