#include <stdio.h>
#include <sys/syscall.h>

#define EUDY 317

int main(int argc, char *argv[])
{
	int result;

	printf("1. Calling sys_eudyptula with params (1,1)\n");
	if (syscall(EUDY,1,1))
		printf("Test OK\n");
	else
		printf("Test failed\n");

	printf("2. Calling sys_eudyptula with params (0x7c1c,0xaf2f50d1)\n");
	if (!syscall(EUDY,0x7c1c,0xaf2f50d1))
		printf("Test OK\n");
	else
		printf("Test failed\n");
}
