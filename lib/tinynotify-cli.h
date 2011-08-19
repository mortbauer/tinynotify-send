/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#ifndef _TINYNOTIFY_CLI_H
#define _TINYNOTIFY_CLI_H

#include <tinynotify.h>

/**
 * SECTION: libtinynotify-cli
 * @short_description: API to create notifications from command-line args
 * @include: tinynotify-cli.h
 *
 * The libtinynotify-cli helper library provides the routines for simple
 * command-line argument parsing (using getopt()) and turning those into
 * a #Notification instance.
 */

/**
 * notification_new_from_cmdline
 * @argc: command-line argument count
 * @argv: command-line argument values
 *
 * Parse the command-line options and create a new #Notification instance
 * from them (with unformatted summary & body).
 *
 * Currently, this recognizes '-i' for icon. It takes two first non-option args
 * for summary (obligatory) and body (optional). Further arguments and unknown
 * options are silently ignored.
 *
 * This function uses getopt(), and doesn't reset it. If it is used after
 * another getopt() call without reset, it will start where previous getopt()
 * left off. If getopt() is used after it without reset, it will start where
 * this functions lefts off.
 *
 * After a call to this function, optind contains the index of first unused
 * command-line argument after the option list (i.e. the third non-option
 * argument on the command-line).
 *
 * Returns: a newly-allocated #Notification instance, or %NULL if args
 * are invalid (e.g. summary missing)
 */
Notification notification_new_from_cmdline(int argc, char *argv[]);

#endif
