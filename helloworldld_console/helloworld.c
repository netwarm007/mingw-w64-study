#include <stdio.h>

#define UNUSED(x) (void)(x);

int main(int argc, void** argv) {
	UNUSED(argc)
	UNUSED(argv)
	printf("Hello, world!\n");
}

