/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#include "config.h"
#include "tinynotify-cli.h"
#include "tinynotify.h"

#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

Notification notification_new_from_cmdline(int argc, char *argv[]) {
	int opterr_backup = opterr;
	int arg;

	const char *icon = NOTIFICATION_DEFAULT_APP_ICON;
	const char *summary;
	const char *body = NOTIFICATION_NO_BODY;

	Notification n;

	opterr = 0; /* be quiet about nonsense args */
	while (((arg = getopt(argc, argv, "i:"))) != -1) {
		switch (arg) {
			case 'i':
				icon = optarg;
				break;
		}
	}
	opterr = opterr_backup;

	if (optind >= argc) {
		/* XXX, better error handling? */
		return NULL;
	}

	summary = argv[optind++];
	if (optind < argc)
		body = argv[optind++];

	n = notification_new(summary, body);
	if (icon)
		notification_set_app_icon(n, icon);

	return n;
}
