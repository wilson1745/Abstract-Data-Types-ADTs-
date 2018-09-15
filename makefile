CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
TARGETS = testarr week9p2 week9p3

all: $(TARGETS)

testarr: testarr.c arr.c arr.h
	$(CC) -g -D ARRINTS testarr.c arr.c -o $@ $(CFLAGS)

week9p2: testset.c arr.c set.c arr.h set.h
	$(CC) -O3 -D ARRINTS testset.c arr.c set.c -o $@ $(CFLAGS)

week9p3: janeausten.c arr.c set.c arr.h set.h
	$(CC) -O3 -D ARRSTRINGS arr.c set.c janeausten.c -o $@ $(CFLAGS)

clean:
	rm -fr $(TARGETS)

run: all
	./testarr
	./week9p2
	./week9p3

.PHONY: clean run all
