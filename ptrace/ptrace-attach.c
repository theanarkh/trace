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
        int status;
        int bit = 1;
        long num;
        long ret;
	pid_t pid = atoi(argv[1]);
        ptrace(PTRACE_ATTACH, pid ,NULL,NULL);
	wait(&status);
        if(WIFEXITED(status))
          return 0;
	ptrace(PTRACE_SYSCALL,pid,NULL,NULL);
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
