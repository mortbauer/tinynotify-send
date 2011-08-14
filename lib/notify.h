/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#ifndef _LIBNOTIFY_NOTIFY_H_
#define _LIBNOTIFY_NOTIFY_H_

/**
 * SECTION:notify
 * @title: basic API funcs
 * @short_description: basic API offered by libtinynotify
 * @include: libnotify/notify.h
 *
 * libtinynotify provides a simple and lightweight implementation of Desktop
 * Notification Specification. It tries to keep an API compatible with
 * libnotify but without introducing the unnecessary dependencies.
 */

/**
 * notify_init:
 * @app_name: Application name to pass to the daemon. Can be %NULL.
 *
 * Initializes the library and establishes the connection to the session bus.
 *
 * This function should be called before any other in libtinynotify.
 *
 * Returns: 1 on success, 0 on failure.
 */
int notify_init(const char *app_name);

/**
 * notify_uninit:
 *
 * Uninitializes the library, frees all internal data and releases the D-Bus
 * connection.
 */
void notify_uninit(void);

#endif
