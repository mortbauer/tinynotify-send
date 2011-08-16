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
 * @include: tinynotify.h
 *
 * All actual error handling in libtinynotify is done within the bounds of
 * a #NotifySession. Each of the libtinynotify functions (except for
 * notify_session_new(), notify_session_free() and error grabbing funcs) store
 * their results and any additional error details in the corresponding
 * #NotifySession.
 *
 * After a call to such a function, one may get its error status using
 * notify_session_get_error(). If a function returns #NotifyError, then it is
 * guaranteed that the return value is equal to the result of calling
 * notify_session_get_error() immediately after the function.
 *
 * One may assume that #NOTIFY_ERROR_NO_ERROR will always evaluate to false.
 * Thus, #NotifyError can be used as a boolean result as well.
 *
 * A more detailed error description can be obtained using
 * notify_session_get_error_message(). It can contain additional details not
 * available via #NotifyError like backend error messages.
 *
 * The libtinynotify functions don't check for existing error conditions. It is
 * unnecessary to reset the error within #NotifySession (and thus there is no
 * function implementing such a thing). Calling another function will implicitly
 * reset the current error status, and replace with its own result.
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
 * notify_session_disconnect
 * @session: session to operate on
 *
 * Drop the connection to the D-Bus session bus.
 *
 * Note that calling this function is not obligatory. It will be called
 * by notify_session_free() anyway.
 *
 * If no connection is established already, this function does nothing.
 */
void notify_session_disconnect(NotifySession session);

/**
 * notify_session_connect
 * @session: session to operate on
 *
 * Establish a connection to the D-Bus session bus.
 *
 * Note that calling this function is not obligatory. If not used,
 * the connection will be established when sending the first notification.
 *
 * If a connection is established already, this function does nothing
 * and returns #NOTIFY_ERROR_NO_ERROR.
 *
 * Returns: a #NotifyError or #NOTIFY_ERROR_NO_ERROR if connection succeeds.
 * For additional error details, see notify_session_get_error_message().
 */
NotifyError notify_session_connect(NotifySession session);

/**
 * notify_session_set_app_name
 * @session: session to operate on
 * @app_name: a new app name or #NULL
 *
 * Set the default application name for notifications sent through this session.
 *
 * If #NULL is passed, the default application name will be cleared. Otherwise,
 * the given string will be copied to #NotifySession.
 */
void notify_session_set_app_name(NotifySession session, const char* app_name);

/**
 * notify_session_set_app_icon
 * @session: session to operate on
 * @app_icon: a new icon name or #NULL
 *
 * Set the default application icon for notifications sent through this
 * session.
 *
 * If #NULL is passed, the default application icon will be cleared. Otherwise,
 * the given string will be copied to #NotifySession.
 */
void notify_session_set_app_icon(NotifySession session, const char* app_icon);

/**
 * SECTION: notification
 * @short_description: API to deal with a single notification
 * @include: tinynotify.h
 * @stability: Unstable
 *
 * In order to send a notification, one must first create a new #Notification
 * instance via notification_new(). When done with a particular notification,
 * one should pass it to notification_free().
 */

/**
 * Notification
 *
 * A type describing a single notification.
 *
 * It should be created using notification_new(), and disposed using
 * notification_free().
 */

typedef void* Notification;

/**
 * notification_new
 * @summary: short text summary of the notification
 * @body: complete body text of the notification (or #NULL)
 *
 * Create and initialize a new libtinynotify notification.
 *
 * This function always succeeds. If it is unable to allocate the memory,
 * program execution will be aborted.
 *
 * Returns: a newly-instantiated Notification
 */
Notification notification_new(const char* summary, const char* body);

/**
 * notification_free
 * @notification: the notification to free
 *
 * Free a libtinynotify notification.
 *
 * This function always succeeds. After a call to this function,
 * a #Notification is no longer valid.
 */
void notification_free(Notification notification);

#endif
