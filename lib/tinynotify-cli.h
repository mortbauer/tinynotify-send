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
 * notification_new_from_cmdline
 * @argc: command-line argument count
 * @argv: command-line argument values
 * @version_str: string to output on --version
 * @use_systemwide: location to store '--system-wide' request status
 *
 * Parse the command-line options and create a new #Notification instance
 * from them (with unformatted summary & body).
 *
 * This function does handle complete command-line parsing for the application,
 * including handling invalid args, --help and --version. It should be called
 * ASAP, and if it returns %NULL, the application should terminate immediately
 * and quietly (it will handle the necessary output itself).
 *
 * If '--system-wide' is passed, @use_systemwide will be set to a non-zero
 * value. If '--local' is passed, @use_systemwide will be set to zero.
 * Otherwise, @use_systemwide will be left unmodified.
 *
 * Returns: a newly-allocated #Notification instance, or %NULL if application
 * should terminate (invalid args, --help, --version)
 */
Notification notification_new_from_cmdline(int argc, char *argv[],
		const char *version_str, int *use_systemwide);

#endif
