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

typedef struct _notify_session* NotifySession;

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
 * One may assume that %NOTIFY_ERROR_NO_ERROR will always evaluate to false.
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
 * @NOTIFY_ERROR_DBUS_SEND: unable to send the notification
 * @NOTIFY_ERROR_INVALID_REPLY: invalid reply was received from server
 * @NOTIFY_ERROR_NO_NOTIFICATION_ID: no notification-id set in #Notification
 *
 * A tinynotify error code.
 *
 * Note that %NOTIFY_ERROR_NO_ERROR is guaranteed to be always 0. Thus, one can
 * use this enum as a boolean for error indication.
 */

typedef enum {
	NOTIFY_ERROR_NO_ERROR = 0,

	NOTIFY_ERROR_DBUS_CONNECT,
	NOTIFY_ERROR_DBUS_SEND,
	NOTIFY_ERROR_INVALID_REPLY,
	NOTIFY_ERROR_NO_NOTIFICATION_ID,

	/*< private >*/
	NOTIFY_ERROR_COUNT
} NotifyError;

/**
 * notify_session_new
 * @app_name: default application name for the session
 * @app_icon: default application icon for the session
 *
 * Create and initialize a new libtinynotify session. Sets the default
 * @app_name (unless %NOTIFY_SESSION_NO_APP_NAME) and @app_icon (unless
 * %NOTIFY_SESSION_NO_APP_ICON).
 *
 * This function always succeeds. If it is unable to allocate the memory,
 * program execution will be aborted.
 *
 * Returns: a newly-instantiated NotifySession
 */
NotifySession notify_session_new(const char* app_name, const char* app_icon);

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
 * Returns: positive #NotifyError or %NOTIFY_ERROR_NO_ERROR if no error
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
 * and returns %NOTIFY_ERROR_NO_ERROR.
 *
 * Returns: a #NotifyError or %NOTIFY_ERROR_NO_ERROR if connection succeeds.
 * For additional error details, see notify_session_get_error_message().
 */
NotifyError notify_session_connect(NotifySession session);

/**
 * NOTIFY_SESSION_NO_APP_NAME
 *
 * A constant specifying that no default app name is to be specified.
 */
extern const char* const NOTIFY_SESSION_NO_APP_NAME;

/**
 * notify_session_set_app_name
 * @session: session to operate on
 * @app_name: a new app name
 *
 * Set the default application name for notifications sent through this session.
 *
 * If %NOTIFY_SESSION_NO_APP_NAME is passed, the default application name will
 * be cleared. Otherwise, the given string will be copied to #NotifySession.
 */
void notify_session_set_app_name(NotifySession session, const char* app_name);

/**
 * NOTIFY_SESSION_NO_APP_ICON
 *
 * A constant specifying that no default app icon is to be specified.
 */
extern const char* const NOTIFY_SESSION_NO_APP_ICON;

/**
 * notify_session_set_app_icon
 * @session: session to operate on
 * @app_icon: a new icon name
 *
 * Set the default application icon for notifications sent through this
 * session.
 *
 * If %NOTIFY_SESSION_NO_APP_ICON is passed, the default application icon will
 * be cleared. Otherwise, the given string will be copied to #NotifySession.
 */
void notify_session_set_app_icon(NotifySession session, const char* app_icon);

/**
 * SECTION: notification
 * @short_description: API to deal with a single notification
 * @include: tinynotify.h
 *
 * A single notification in libtinynotify is represented by a #Notification
 * instance. A new #Notification instance can be obtained using
 * notification_new(), and should be freed when no longer used using
 * notification_free().
 *
 * Although notifications aren't directly associated with sessions, they must
 * use one in order to access the message bus. Thus, all functions interacting
 * with the message bus require passing a #NotifySession explicitly.
 *
 * Such a function may store connection-specific information within
 * the #Notification (e.g. the notification ID). However, it is guaranteed that
 * it won't store any reference to the #NotifySession or any data contained
 * within it. In other words, one may safely free a #NotifySession after use,
 * and reuse the same #Notification in another session.
 */

/**
 * Notification
 *
 * A type describing a single notification.
 *
 * It should be created using notification_new(), and disposed using
 * notification_free().
 */

typedef struct _notification* Notification;

/**
 * NOTIFICATION_NO_BODY
 *
 * A constant specifying that the notification has no body (detailed message).
 */
extern const char* const NOTIFICATION_NO_BODY;

/**
 * notification_new
 * @summary: short text summary of the notification
 * @body: complete body text of the notification (or %NOTIFICATION_NO_BODY)
 *
 * Create and initialize a new libtinynotify notification.
 *
 * This function always succeeds. If it is unable to allocate the memory,
 * program execution will be aborted.
 *
 * Note: @summary & @body are printf()-style format strings by default. For
 * plain strings, please use notification_new_unformatted() instead.
 *
 * Returns: a newly-instantiated #Notification
 */
Notification notification_new(const char* summary, const char* body);

/**
 * notification_new_unformatted
 * @summary: short text summary of the notification
 * @body: complete body text of the notification (or %NOTIFICATION_NO_BODY)
 *
 * Create and initialize a new libtinynotify notification using unformatted
 * summary & body strings.
 *
 * This function always succeeds. If it is unable to allocate the memory,
 * program execution will be aborted.
 *
 * Returns: a newly-instantiated #Notification
 */
Notification notification_new_unformatted(const char* summary, const char* body);

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

/**
 * NOTIFICATION_DEFAULT_APP_ICON
 *
 * A constant specifying that the default app icon should be used (if specified
 * in #NotifySession).
 */
extern const char* const NOTIFICATION_DEFAULT_APP_ICON;

/**
 * NOTIFICATION_NO_APP_ICON
 *
 * A constant specifying that no app icon should ever be used (even if
 * #NotifySession specifies one).
 */
extern const char* const NOTIFICATION_NO_APP_ICON;

/**
 * notification_set_app_icon
 * @notification: notification to operate on
 * @app_icon: a new icon name
 *
 * Set the application icon for a single notification.
 *
 * If %NOTIFICATION_DEFAULT_APP_ICON is passed, the notification will be reset
 * to use default app icon specified in #NotifySession (if one is set). If
 * %NOTIFICATION_NO_APP_ICON is passed, the notification will not use any app
 * icon, even if #NotifySession specifies one. Otherwise, the given string will
 * be copied to #Notification.
 */
void notification_set_app_icon(Notification notification, const char* app_icon);

/**
 * NOTIFICATION_DEFAULT_EXPIRE_TIMEOUT
 *
 * A constant specifying that the default expire timeout should be used.
 */
extern const int NOTIFICATION_DEFAULT_EXPIRE_TIMEOUT;

/**
 * NOTIFICATION_NO_EXPIRE_TIMEOUT
 *
 * A constant specifying that the notification shall not expire.
 */
extern const int NOTIFICATION_NO_EXPIRE_TIMEOUT;

/**
 * notification_set_expire_timeout
 * @notification: notification to operate on
 * @expire_timeout: a new expiration timeout [ms]
 *
 * Set the expiration timeout for a notification, in milliseconds.
 *
 * If %NOTIFICATION_DEFAULT_EXPIRE_TIMEOUT is used, the notification expires
 * on a server-specified, default timeout. If %NOTIFICATION_NO_EXPIRE_TIMEOUT
 * is used, the notification doesn't expire and needs to be closed explicitly.
 */
void notification_set_expire_timeout(Notification notification, int expire_timeout);

/**
 * NotificationUrgency
 * @NOTIFICATION_URGENCY_LOW: low urgency level
 * @NOTIFICATION_URGENCY_NORMAL: normal urgency level
 * @NOTIFICATION_URGENCY_CRITICAL: critical urgency level
 *
 * Protocol-defined urgency levels, for notification_set_urgency().
 */

typedef enum {
	NOTIFICATION_URGENCY_LOW = 0,
	NOTIFICATION_URGENCY_NORMAL = 1,
	NOTIFICATION_URGENCY_CRITICAL = 2
} NotificationUrgency;

/**
 * NOTIFICATION_NO_URGENCY
 *
 * A constant specifying that no urgency level should be set in a notifcation.
 */
extern const short int NOTIFICATION_NO_URGENCY;

/**
 * notification_set_urgency
 * @notification: notification to operate on
 * @urgency: a new urgency level
 *
 * Set the urgency level for a notification.
 *
 * If set to %NOTIFICATION_NO_URGENCY, the current urgency level would be
 * cleared.
 */
void notification_set_urgency(Notification notification, short int urgency);

/**
 * NOTIFICATION_NO_CATEGORY
 *
 * A constant specifying that no category should be used.
 */
extern const char* const NOTIFICATION_NO_CATEGORY;

/**
 * notification_set_category
 * @notification: notification to operate on
 * @category: a new category, or %NOTIFICATION_NO_CATEGORY
 *
 * Set the category for a notification.
 *
 * If set to %NOTIFICATION_NO_CATEGORY, the current category will be cleared;
 * otherwise, the category string will be copied into #Notification.
 */
void notification_set_category(Notification notification, const char* category);

/**
 * notification_send
 * @notification: the notification to send
 * @session: session to send the notification through
 * @...: additional arguments for summary & body format strings
 *
 * Send a notification to the notification daemon.
 *
 * If summary and/or body contains any printf()-style directives,
 * their arguments should be passed to this function. If both of them do,
 * arguments for the summary format string should be specified first,
 * and arguments to the body format string should immediately follow.
 *
 * If notification is displayed successfully, the received message ID is stored
 * within the #Notification type. The notification_update() function can be
 * used to update the notification afterwards.
 *
 * Returns: a positive #NotifyError or %NOTIFY_ERROR_NO_ERROR
 */
NotifyError notification_send(Notification notification, NotifySession session, ...);

/**
 * notification_update
 * @notification: the notification being updated
 * @session: session to send the notification through
 * @...: additional arguments for summary & body format strings
 *
 * Send an updated notification to the notification daemon. This will
 * replace/update the notification sent previously to server with the same
 * #Notification instance.
 *
 * If the #Notification has no ID stored, notification_update() will work
 * like notification_send(), and obtain a new ID.
 *
 * If summary and/or body contains any printf()-style directives,
 * their arguments should be passed to this function. If both of them do,
 * arguments for the summary format string should be specified first,
 * and arguments to the body format string should immediately follow.
 *
 * If notification is updated successfully, the received message ID is stored
 * within the #Notification type. Further updates to it can be done using
 * notification_update() then.
 *
 * Returns: a positive #NotifyError or %NOTIFY_ERROR_NO_ERROR
 */
NotifyError notification_update(Notification notification, NotifySession session, ...);

/**
 * notification_close
 * @notification: the notification to close
 * @session: session to send the request through
 *
 * Request closing the notification sent previously to server.
 *
 * This function succeeds unless a communication error occurs (or no ID was
 * set). It is undefined whether the notification was closed due to it, before
 * it or the notification identifier was invalid.
 *
 * This function unsets the notification ID stored in #Notification -- it is no
 * longer valid after the notification is closed.
 *
 * Returns: a positive #NotifyError or %NOTIFY_ERROR_NO_ERROR
 */
NotifyError notification_close(Notification notification, NotifySession session);

/**
 * notification_set_formatting
 * @notification: notification to operate on
 * @formatting: zero (false) to disable, non-zero (true) to enable
 *
 * Enable or disable formatting within a #Notification. If formatting is
 * enabled, summary & body are expected to be printf()-style format strings; if
 * it is disabled, they are treated as plain strings.
 *
 * Note: this function shouldn't be used unless necessary. It is preferred to
 * create a new #Notification instead.
 */
void notification_set_formatting(Notification notification, int formatting);

/**
 * notification_set_summary
 * @notification: notification to operate on
 * @summary: a new summary (format string)
 *
 * Set the summary of a notification.
 *
 * Note: this function shouldn't be used unless necessary. It is preferred to
 * create a new #Notification instead, or use variant (formatted) summary
 * in the constructor.
 */
void notification_set_summary(Notification notification, const char* summary);

/**
 * notification_set_body
 * @notification: notification to operate on
 * @body: a new body (format string, or %NOTIFICATION_NO_BODY)
 *
 * Set (or unset) the body of a notification.
 *
 * Note: this function shouldn't be used unless necessary. It is preferred to
 * create a new #Notification instead, or use variant (formatted) summary
 * in the constructor.
 */
void notification_set_body(Notification notification, const char* body);

#endif
