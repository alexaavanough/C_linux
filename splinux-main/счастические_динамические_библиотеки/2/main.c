#include <stdio.h>
#include <stdlib.h>
#include "first.h"
#include "second.h"

int main(int argc, char const *argv[])
{
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	printf("tsf = %d\n", first(a, b));
}