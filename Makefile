rose_test:
	gcc -march=native -o rose_test.out rose_test.c rose.c lib.c

clean:
	rm *.out
