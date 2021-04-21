EXECS=args sha512

sha512: sha512.c
	gcc -o sha512 sha512.c

test: tests.sh
	./tests.sh

clean:
	rm -f $(EXECS)