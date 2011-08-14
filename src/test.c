#include "tinynotify.h"

#include <stdio.h>

int main(void) {
	printf("notify_init() -> %d\n", notify_init("foo"));
	notify_uninit();

	return 0;
}
