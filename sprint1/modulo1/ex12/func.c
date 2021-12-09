#include <sys/types.h>
#include <unistd.h>

int spawn_childs(int n) {
	int i;
	for(i = 0; i < n; i++) {
		pid_t p = fork();
		if(p == 0)
			return i;
	}
	return -1;
}
