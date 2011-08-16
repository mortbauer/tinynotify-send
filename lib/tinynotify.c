/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#include "config.h"
#include "tinynotify.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <dbus/dbus.h>

struct _tinynotify_notify_session {
	DBusConnection *conn;

	char* app_name;

	NotifyError error;
	char* error_details;
};

NotifySession notify_session_new(void) {
	struct _tinynotify_notify_session *ret;

	assert(ret = malloc(sizeof(*ret)));
	ret->conn = NULL;
	ret->app_name = NULL;
	ret->error = NOTIFY_ERROR_NO_ERROR;
	return ret;
}

void notify_session_free(NotifySession session) {
	notify_session_disconnect(session);
	free(session);
}

static NotifyError _notify_session_set_error(
		struct _tinynotify_notify_session *s,
		NotifyError new_error)
{
	s->error = new_error;
	return new_error;
}

NotifyError notify_session_get_error(NotifySession session) {
	struct _tinynotify_notify_session *s = session;

	return s->error;
}

static const char* _error_messages[NOTIFY_ERROR_COUNT] = {
	"No error"
};

const char* notify_session_get_error_message(NotifySession session) {
	struct _tinynotify_notify_session *s = session;

	return _error_messages[s->error];
}

NotifyError notify_session_connect(NotifySession session) {
	struct _tinynotify_notify_session *s = session;

	if (!s->conn) {
		DBusError err;

		dbus_error_init(&err);
		s->conn = dbus_bus_get_private(DBUS_BUS_SESSION, &err);

		assert(!s->conn == dbus_error_is_set(&err));
		if (!s->conn) {
			s->conn = NULL;
			/* XXX: copy error message */
			dbus_error_free(&err);
			return _notify_session_set_error(s, NOTIFY_ERROR_DBUS_CONNECT);
		} else
			dbus_connection_set_exit_on_disconnect(s->conn, FALSE);
	}

	return _notify_session_set_error(s, NOTIFY_ERROR_NO_ERROR);
}

void notify_session_disconnect(NotifySession session) {
	struct _tinynotify_notify_session *s = session;

	if (s->conn) {
		dbus_connection_close(s->conn);
		dbus_connection_unref(s->conn);
		s->conn = NULL;
	}

	_notify_session_set_error(s, NOTIFY_ERROR_NO_ERROR);
}

void notify_session_set_app_name(NotifySession session, const char* app_name) {
	struct _tinynotify_notify_session *s = session;

	if (s->app_name)
		free(s->app_name);
	if (app_name)
		assert(s->app_name = strdup(app_name));
	else
		s->app_name = NULL;
}

struct _tinynotify_notification {
};

Notification notification_new(void) {
	struct _tinynotify_notification *ret;

	assert(ret = malloc(sizeof(*ret)));
	return ret;
}

void notification_free(Notification n) {
	free(n);
}
