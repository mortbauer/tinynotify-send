#include "tinynotify.h"

#include <stdio.h>

int main(void) {
	NotifySession s;

	s = notify_session_new();
	notify_session_free(s);

	return 0;
}
