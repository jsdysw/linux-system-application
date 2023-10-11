#include <stdio.h>
#include <sys/syscall.h>

int main(void)
{
	long ret = syscall(436);
	printf("System call hello returned : %ld\n", ret);

	return 0;
}
