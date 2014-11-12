#include <jni.h>
#include "PtraceLibJNI.h"
#include "common.h"

#ifdef __86__
#define filename "syscall_output.txt"
#endif

#ifdef __x86_64__
#define filename "syscall64_out.txt"
#endif


/**
 * This is code for our adb client that can utilize the ptrace commands as well.
 */


int main(int argc, char **argv){

	parse_system_call_name("syscall64_out.txt");

	int pid;
	int i = 1;
	i+=2;

	pid = atoi(argv[1]);

	//printf("Hello, world (i=%d)!\n", pid);

	jint stat=0;
	jint res=0;
	long syscall;
	printf("Attaching to a process %d",pid);

	if ((ptrace(PTRACE_ATTACH, pid, NULL, NULL)) != 0) {

		printf("Could not attach to process, errno is %d",errno);
	}

	res = waitpid(pid, &stat, WUNTRACED);

	__android_log_print(ANDROID_LOG_INFO,LOG_TAG,"Status of the traced process %d\n",stat);


	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);

	while(1){
		if(trace_syscall(pid)!=0)
			break;
		print_syscall(pid);

		if(trace_syscall(pid)!=0)
			break;
		get_return_value(pid);

	}
	cleanup_syscall_tabel();
	return 0;
}

