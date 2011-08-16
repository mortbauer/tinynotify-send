/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#ifndef _TINYNOTIFY_H
#define _TINYNOTIFY_H

/**
 * SECTION: session
 * @short_description: API to deal with libtinynotify sessions
 * @include: tinynotify.h
 *
 * All interaction with libtinynotify is associated with a single session,
 * represented by the #NotifySession type. Before calling any of the library
 * functions, one must instantiate a new session using notify_session_new().
 * When done with it, one should free the session using notify_session_free().
 *
 * One must not pass custom, invalid or freed #NotifySession to any of
 * the functions. Otherwise, the results are unpredictable (and a segfault is
 * the most harmless of them). One must not attempt to modify or manually free
 * a #NotifySession instance, and one shall not assume anything specific about
 * its actual type. When in question, one should pass a pointer to it rather
 * than casting #NotifySession to one.
 */

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
 * SECTION: error
 * @short_description: API to deal with libtinynotify errors
 */

/**
 * NotifyError
 * @NOTIFY_ERROR_NO_ERROR: no error
 * @NOTIFY_ERROR_DBUS_CONNECT: unable to connect to the session bus
 *
 * A tinynotify error code.
 *
 * Note that #NOTIFY_ERROR_NO_ERROR is guaranteed to be always 0. Thus, one can
 * use this enum as a boolean for error indication.
 */

typedef enum {
	NOTIFY_ERROR_NO_ERROR = 0,

	NOTIFY_ERROR_DBUS_CONNECT,

	/*< private >*/
	NOTIFY_ERROR_COUNT
} NotifyError;

/**
 * notify_session_new
 *
 * Create and initialize a new libtinynotify session.
 *
 * This function always succeeds. If it is unable to allocate the memory,
 * program execution will be aborted.
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
 *
 * This function always succeeds. After a call to this function,
 * a #NotifySession is no longer valid.
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
 * Returns: a statically allocated or constant string (not to be freed)
 */
const char* notify_session_get_error_message(NotifySession session);

/**
 * notify_session_connect
 * @session: session to operate on
 *
 * Establish a connection to the D-Bus session bus.
 *
 * Note that calling this function is not obligatory. If not used,
 * the connection will be established when sending the first notification.
 *
 * Returns: a #NotifyError or #NOTIFY_ERROR_NO_ERROR if connection succeeds.
 * For additional error details, see notify_session_get_error_message().
 */
NotifyError notify_session_connect(NotifySession session);

#endif
