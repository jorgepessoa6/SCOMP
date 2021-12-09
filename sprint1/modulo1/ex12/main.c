#include <stdio.h>
#include <sys/wait.h>
#include "func.h"

int main(void) {
	int index = spawn_childs(6);
	if(index == -1)
		wait(NULL);
	else
		printf("Child index: %d\n", (index + 1) * 2);
	
	return 0;

}
