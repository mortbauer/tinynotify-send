/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#include "config.h"
#include "tinynotify-cli.h"
#include <tinynotify.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifdef HAVE_GETOPT_LONG
#	include <getopt.h>
#endif

static void _handle_version(const char *version_str) {
	fprintf(stderr, "%s (libtinynotify %s)\n", version_str, PACKAGE_VERSION);
}

/* remember to keep all option-related stuff in the same order! */

static const char* const _option_descs[] = {
	" CATEGORY", "category",
	" ICON", "application icon (name or path)",
	" TIME", "expiration timeout (in ms)",
	" LEVEL", "urgency level (0 - low, 1 - normal, 2 - critical)",
	NULL, "show help message",
	NULL, "output version information"
};

static const char* const _getopt_optstring = "c:i:t:u:?V";

#ifdef HAVE_GETOPT_LONG
static const struct option _getopt_longopts[] = {
	{ "category", required_argument, NULL, 'c' },
	{ "icon", required_argument, NULL, 'i' },
	{ "expire-time", required_argument, NULL, 't' },
	{ "urgency", required_argument, NULL, 'u' },

	{ "help", no_argument, NULL, '?' },
	{ "version", no_argument, NULL, 'V' },
	{ 0, 0, 0, 0 }
};
#endif

static void _handle_help(const char *argv0) {
#ifdef HAVE_GETOPT_LONG
	const struct option* opt;
#else
	const char* opt;
#endif
	const char* const* desc;

	char buf[25];

	fprintf(stderr, "Usage: %s [options] summary [body]\n\n", argv0);

#ifdef HAVE_GETOPT_LONG
	for (opt = _getopt_longopts, desc = _option_descs;
			opt->name; opt++, desc++) {
		sprintf(buf, "-%c, --%s%s", opt->val, opt->name, *desc ? *desc : "");
#else
	for (opt = _getopt_optstring, desc = _option_descs;
			*opt; opt++, desc++) {
		if (*opt == ':')
			opt++; /* last will be 'V', so we don't need to recheck *opt */
		sprintf(buf, "-%c%s", *opt, *desc ? *desc : "");
#endif
		fprintf(stderr, "  %-24s %s\n", buf, *(++desc));
	}
}

Notification notification_new_from_cmdline(int argc, char *argv[], const char *version_str) {
	int arg;

	const char *icon = NOTIFICATION_DEFAULT_APP_ICON;
	const char *summary;
	const char *body = NOTIFICATION_NO_BODY;
	int expire_timeout = NOTIFICATION_DEFAULT_EXPIRE_TIMEOUT;
	NotificationUrgency urgency = NOTIFICATION_NO_URGENCY;
	const char *category = NOTIFICATION_NO_CATEGORY;

	Notification n;

#ifdef HAVE_GETOPT_LONG
	while (((arg = getopt_long(argc, argv, _getopt_optstring,
						_getopt_longopts, NULL))) != -1) {
#else
	while (((arg = getopt(argc, argv, _getopt_optstring))) != -1) {
#endif
		switch (arg) {
			case 'c':
				category = optarg;
				break;
			case 'i':
				icon = optarg;
				break;
			case 't':
				expire_timeout = atoi(optarg);
				break;
			case 'u':
				urgency = atoi(optarg);
				break;
			case 'V':
				_handle_version(version_str);
				return NULL;
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
	notification_set_expire_timeout(n, expire_timeout);
	notification_set_urgency(n, urgency);
	if (category)
		notification_set_category(n, category);

	return n;
}
