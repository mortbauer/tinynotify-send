#include "tinynotify.h"

#include <stdio.h>

void print_errors(NotifySession s) {
	fprintf(stderr, "ERROR: %d (%s)\n", notify_session_get_error(s),
			notify_session_get_error_message(s));
}

int main(void) {
	NotifySession s;
	Notification n;

	s = notify_session_new("foobar", "web-browser");
	print_errors(s);
	notify_session_connect(s);
	print_errors(s);

	n = notification_new("foo %s", "test bar %d");
	notification_send(n, s, "bar", 1);
	print_errors(s);
	notification_set_summary(n, "baz?");
	notification_update(n, s, 2);
	print_errors(s);
	notification_free(n);

	notify_session_free(s);

	return 0;
}
