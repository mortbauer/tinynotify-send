/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#ifndef _TINYNOTIFY_H
#define _TINYNOTIFY_H

/**
 * NotifySession
 *
 * A type describing a basic tinynotify session. It holds the data necessary for
 * the notifications to be sent, like the D-Bus connection. It also provides
 * a storage for errors.
 *
 * It should be created using notify_session_new(), and disposed using
 * notify_session_free().
 */

typedef void* NotifySession;

/**
 * notify_session_new
 *
 * Create and initialize a new libtinynotify session.
 *
 * Returns: a newly-instantiated NotifySession
 */
NotifySession notify_session_new(void);

/**
 * notify_session_free
 * @session: the session to free
 *
 * Free a libtinynotify session.
 */
void notify_session_free(NotifySession session);

#endif
