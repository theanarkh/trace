build:
	make ptrace && make test
ptrace: ptrace.c
	gcc ptrace.c -o ptrace

test: test.c
	gcc test.c -o test
run:
	./ptrace test
