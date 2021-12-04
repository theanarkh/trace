build:
	make ptrace && make test && make ptrace-attach

ptrace: ptrace.c
	gcc ptrace.c -o ptrace

test: test.c
	gcc test.c -o test

ptrace-attach: ptrace-attach.c
	gcc ptrace-attach.c -o ptrace-attach

run-ptrace:
	./ptrace test

run-ptrace-attach::
	node test-attach.js & 
	pid=`ps -aux | grep test-attach | awk '{print $$2}'` && ./ptrace-attach $$pid 
