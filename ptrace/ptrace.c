/*
  use ptrace to find all system call that call by certain process
*/

#include <sys/ptrace.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/reg.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (pid < 0) {
        printf("fork failed");
        exit(-1);
    } else if (pid == 0) {
        // set state of child process to PTRACE
        ptrace(PTRACE_TRACEME,0,NULL,NULL);
	char **args = (char **)malloc(sizeof(char *) * argc);
	for (int i = 1; i < argc; i++) {
		args[i - 1] = argv[i];
	}
	args[argc - 1] = NULL; 
	// child will change to stopped state when in execve call, then send the signal to parent
        execve(argv[1], args, NULL);
	// we will not go here if call execve success, if fail, free the memory
	free(args);
    } else {
        int status;
        int bit = 1;
        long num;
        long ret;
        // wait for child
        wait(&status);
        if(WIFEXITED(status))
            return 0;
        // this is for execve call which will not return, and for os of 64-it => ORIG_RAX * 8 or os of 32-it => ORIG_EAX * 4
        num = ptrace(PTRACE_PEEKUSER, pid, ORIG_RAX * 8, NULL);
        printf("system call num = %ld\n", num);
        ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
        while(1) {
            wait(&status);
            if(WIFEXITED(status))
                return 0;
            // for enter system call
            if(bit) {
                num = ptrace(PTRACE_PEEKUSER, pid, ORIG_RAX * 8, NULL);
                printf("system call num = %ld", num);
                bit = 0;
            } else { // for return of system call
                ret = ptrace(PTRACE_PEEKUSER, pid, RAX*8, NULL);
                printf("system call return = %ld \n", ret);
                bit = 1;
            }
            // let this child process continue to run until call next system call
            ptrace(PTRACE_SYSCALL,pid,NULL,NULL);
        }
    }
}
