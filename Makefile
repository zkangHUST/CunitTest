a.out:
	gcc -g convert.c main.c test.c -lcunit
clean:
	rm -rf *.o a.out
