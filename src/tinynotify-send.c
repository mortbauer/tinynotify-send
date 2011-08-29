/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#include "config.h"

#include <tinynotify.h>
#include <tinynotify-cli.h>

#ifdef BUILDING_SYSTEMWIDE
#	include <tinynotify-systemwide.h>
#else
#	ifdef SYSTEMWIDE_EXEC
#		include <unistd.h>
#	endif
#endif

#include <stdio.h>

int main(int argc, char *argv[]) {
	NotifySession s;
	Notification n;
	NotifyCLIFlags fl;

	int ret;

	n = notification_new_from_cmdline(argc, argv,
#ifdef BUILDING_SYSTEMWIDE
			"systemwide-"
#endif
			PACKAGE_STRING, &fl);
	if (!n)
		return 1;

#ifndef BUILDING_SYSTEMWIDE
	if (notify_cli_flags_get_systemwide(fl)) {
		notification_free(n);

#	ifdef SYSTEMWIDE_EXEC
		/* If successful, shouldn't return */
		execvp(SYSTEMWIDE_EXEC, argv);
#	endif

		fputs("System-wide notifications not supported.\n", stderr);
		return 1;
	}
#endif

	s = notify_session_new("tinynotify-send", NOTIFY_SESSION_NO_APP_ICON);

#ifdef BUILDING_SYSTEMWIDE
	if (!notify_cli_flags_get_local(fl))
		ret = notification_send_systemwide(n, s);
	else
#endif
		ret = !notification_send(n, s);

	if (!ret)
		fprintf(stderr, "%s\n", notify_session_get_error_message(s));

	notify_session_free(s);
	notification_free(n);

	return !ret;
}
