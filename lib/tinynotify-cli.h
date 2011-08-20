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
 */

/**
 * notification_new_from_cmdline
 * @argc: command-line argument count
 * @argv: command-line argument values
 * @version_str: string to output on --version
 *
 * Parse the command-line options and create a new #Notification instance
 * from them (with unformatted summary & body).
 *
 * This function does handle complete command-line parsing for the application,
 * including handling invalid args, --help and --version. It should be called
 * ASAP, and if it returns %NULL, the application should terminate immediately
 * and quietly (it will handle the necessary output itself).
 *
 * Returns: a newly-allocated #Notification instance, or %NULL if application
 * should terminate (invalid args, --help, --version)
 */
Notification notification_new_from_cmdline(int argc, char *argv[], const char *version_str);

#endif
