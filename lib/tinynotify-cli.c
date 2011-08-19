/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#include "config.h"
#include "tinynotify.h"
#include "tinynotify-cli.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

static void _handle_version(const char *version_str) {
	fprintf(stderr, "%s (libtinynotify %s)\n", version_str, PACKAGE_VERSION);
}

/* remember to keep all option-related stuff in the same order! */

static const char* const _option_descs[] = {
	"ICON", "application icon (name or path)",
	NULL, "show help message",
	NULL, "output version information"
};

static const char* const _getopt_optstring = "i:hV";

static void _handle_help(const char *argv0) {
	const char* opt;
	const char* const* desc;

	char buf[22];

	fprintf(stderr, "Usage: %s [options] summary [body]\n\n", argv0);

	for (opt = _getopt_optstring, desc = _option_descs;
			*opt; opt++, desc++) {
		if (*opt == ':')
			opt++; /* last will be 'V', so we don't need to recheck *opt */
		sprintf(buf, "-%c %s", *opt, *desc ? *desc : "");
		fprintf(stderr, "  %-20s %s\n", buf, *(++desc));
	}
}

Notification notification_new_from_cmdline(int argc, char *argv[], const char *version_str) {
	int arg;

	const char *icon = NOTIFICATION_DEFAULT_APP_ICON;
	const char *summary;
	const char *body = NOTIFICATION_NO_BODY;

	Notification n;

	while (((arg = getopt(argc, argv, _getopt_optstring))) != -1) {
		switch (arg) {
			case 'i':
				icon = optarg;
				break;
			case 'V':
				_handle_version(version_str);
				return NULL;
			case 'h':
			case '?':
				_handle_help(argv[0]);
				return NULL;
		}
	}

	if (optind >= argc) {
		fputs("No summary specified.\n", stderr);
		return NULL;
	}

	summary = argv[optind++];
	if (optind < argc)
		body = argv[optind++];
	if (optind < argc) {
		fputs("Too many arguments.\n", stderr);
		return NULL;
	}

	n = notification_new_unformatted(summary, body);
	if (icon)
		notification_set_app_icon(n, icon);

	return n;
}
