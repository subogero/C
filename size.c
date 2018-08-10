#include <stdio.h>
#include <time.h>
int main(void)
{
	printf("char    %ld\n", sizeof(char));
	printf("short   %ld\n", sizeof(short));
	printf("int     %ld\n", sizeof(int));
	printf("long    %ld\n", sizeof(long));
	printf("float   %ld\n", sizeof(float));
	printf("double  %ld\n", sizeof(double));
	printf("pointer %ld\n", sizeof(void *));
	printf("time    %ld\n", sizeof(time_t));
	return 0;
}
