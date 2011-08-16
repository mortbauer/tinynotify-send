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
 * NotifyError
 * @NOTIFY_ERROR_NO_ERROR: no error
 *
 * A tinynotify error code.
 *
 * Note that #NOTIFY_ERROR_NO_ERROR is guaranteed to be always 0. Thus, one can
 * use this enum as a boolean for error indication.
 */

typedef enum {
	NOTIFY_ERROR_NO_ERROR = 0,

	/*< private >*/
	NOTIFY_ERROR_COUNT
} NotifyError;

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
 * Free a libtinynotify session. This handles disconnecting and other cleanup
 * as well.
 */
void notify_session_free(NotifySession session);

/**
 * notify_session_get_error
 * @session: session to operate on
 *
 * Get current error for @session.
 *
 * Returns: positive #NotifyError or #NOTIFY_ERROR_NO_ERROR if no error
 */
NotifyError notify_session_get_error(NotifySession session);

/**
 * notify_session_get_error_message
 * @session: session to operate on
 *
 * Get detailed error message for @session.
 *
 * Returns: a statically allocated or constant string
 */
const char* notify_session_get_error_message(NotifySession session);

#endif
