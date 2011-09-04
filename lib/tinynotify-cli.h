/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#ifndef _TINYNOTIFY_CLI_H
#define _TINYNOTIFY_CLI_H

#include <tinynotify.h>

/**
 * SECTION: cli
 * @short_description: API to handle command-line arg parsing
 * @include: tinynotify-cli.h
 *
 * The libtinynotify-cli helper library provides the routines for simple
 * command-line argument parsing (using getopt()) and turning those into
 * a #Notification instance.
 *
 * To use it, simply add the necessary include to your code, and replace
 * the notification_new() function with our notification_new_from_cmdline().
 * The other functions work as usual, and the resulting #Notification is freed
 * with notification_free(), as usual.
 *
 * Although libtinynotify-cli links with the actual libtinynotify itself,
 * and uses its header, one should refer to the libtinynotify pkg-config
 * package explicitly and include its header anyway.
 */

/**
 * NotifyCLIFlags
 *
 * A type describing a set of additional notification flags as returned
 * by notification_new_from_cmdline().
 */
typedef const char* NotifyCLIFlags;

/**
 * notify_cli_flags_get_systemwide
 * @flags: flags returned by notification_new_from_cmdline()
 *
 * Check whether a system-wide notification was requested.
 *
 * Returns: a non-zero value if system-wide notification was explicitly
 * requested, zero otherwise.
 */
int notify_cli_flags_get_systemwide(NotifyCLIFlags flags);

/**
 * notify_cli_flags_get_local
 * @flags: flags returned by notification_new_from_cmdline()
 *
 * Check whether a local notification was requested.
 *
 * Returns: a non-zero value if local notification was explicitly requested, zero
 * otherwise.
 */
int notify_cli_flags_get_local(NotifyCLIFlags flags);

/**
 * notify_cli_flags_get_foreground
 * @flags: flags returned by notification_new_from_cmdline()
 *
 * Check whether running in the foreground (waiting for notification to be
 * closed) was requested.
 *
 * Returns: a non-zero value if running in the foreground was explicitly
 * requested, zero otherwise.
 */
int notify_cli_flags_get_foreground(NotifyCLIFlags flags);

/**
 * notify_cli_flags_get_background
 * @flags: flags returned by notification_new_from_cmdline()
 *
 * Check whether running in the background (waiting for notification to be
 * closed) is necessary. This may be a case when actions are used, and running
 * in foreground was not requested.
 *
 * Returns: a non-zero value if running in the background is necessary, zero
 * otherwise.
 */
int notify_cli_flags_get_background(NotifyCLIFlags flags);

/**
 * notification_new_from_cmdline
 * @argc: command-line argument count
 * @argv: command-line argument values
 * @version_str: string to output on --version
 * @flags: location to store additional flags
 *
 * Parse the command-line options and create a new #Notification instance
 * from them (with unformatted summary & body).
 *
 * This function does handle complete command-line parsing for the application,
 * including handling invalid args, --help and --version. It should be called
 * ASAP, and if it returns %NULL, the application should terminate immediately
 * and quietly (it will handle the necessary output itself).
 *
 * The pointer at location pointed by @flags argument will be set to point
 * to an internal buffer holding flag string. That string will contain
 * single-letter codes for any additional flags requested by user.
 *
 * Returns: a newly-allocated #Notification instance, or %NULL if application
 * should terminate (invalid args, --help, --version)
 */
Notification notification_new_from_cmdline(int argc, char *argv[],
		const char *version_str, NotifyCLIFlags *flags);

#endif
