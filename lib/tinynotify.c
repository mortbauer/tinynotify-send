/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#include "config.h"
#include "tinynotify.h"

#include <stdlib.h>
#include <string.h>

#ifndef NDEBUG
#	include <assert.h>
#else
#	define assert
#endif

#include <dbus/dbus.h>

struct _tinynotify_notify_session {
	DBusConnection *conn;

	char* app_name;
	char* app_icon;

	NotifyError error;
	char* error_details;
};

NotifySession notify_session_new(void) {
	struct _tinynotify_notify_session *ret;

	assert(ret = malloc(sizeof(*ret)));
	ret->conn = NULL;
	ret->app_name = NULL;
	ret->app_icon = NULL;
	ret->error = NOTIFY_ERROR_NO_ERROR;
	return ret;
}

void notify_session_free(NotifySession session) {
	struct _tinynotify_notify_session *s = session;

	notify_session_disconnect(session);

	if (s->error_details)
		free(s->error_details);
	free(s->app_name);
	free(s->app_icon);
	free(session);
}

static NotifyError _notify_session_set_error(
		struct _tinynotify_notify_session *s,
		NotifyError new_error,
		char *error_details)
{
	if (s->error_details)
		free(s->error_details);
	s->error = new_error;
	s->error_details = error_details;
	return new_error;
}

NotifyError notify_session_get_error(NotifySession session) {
	struct _tinynotify_notify_session *s = session;

	return s->error;
}

static const char* _error_messages[NOTIFY_ERROR_COUNT] = {
	"No error",
	"Connecting to D-Bus failed: %s",
	"Sending message over D-Bus failed: %s",
	"Invalid reply received: %s"
};

const char* notify_session_get_error_message(NotifySession session) {
	struct _tinynotify_notify_session *s = session;
	static char* buf = NULL;

	if (buf)
		free(buf);
	assert(asprintf(&buf, _error_messages[s->error],
				s->error_details) != -1);

	return buf;
}

NotifyError notify_session_connect(NotifySession session) {
	struct _tinynotify_notify_session *s = session;

	if (!s->conn) {
		DBusError err;

		dbus_error_init(&err);
		s->conn = dbus_bus_get_private(DBUS_BUS_SESSION, &err);

		assert(!s->conn == dbus_error_is_set(&err));
		if (!s->conn) {
			char *err_msg = strdup(err.message);
			dbus_error_free(&err);
			return _notify_session_set_error(s, NOTIFY_ERROR_DBUS_CONNECT, err_msg);
		} else
			dbus_connection_set_exit_on_disconnect(s->conn, FALSE);
	}

	return _notify_session_set_error(s, NOTIFY_ERROR_NO_ERROR, NULL);
}

void notify_session_disconnect(NotifySession session) {
	struct _tinynotify_notify_session *s = session;

	if (s->conn) {
		dbus_connection_close(s->conn);
		dbus_connection_unref(s->conn);
		s->conn = NULL;
	}

	_notify_session_set_error(s, NOTIFY_ERROR_NO_ERROR, NULL);
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

void notify_session_set_app_icon(NotifySession session, const char* app_icon) {
	struct _tinynotify_notify_session *s = session;

	if (s->app_icon)
		free(s->app_icon);
	if (app_icon)
		assert(s->app_icon = strdup(app_icon));
	else
		s->app_icon = NULL;
}

struct _tinynotify_notification {
	char* summary;
	char* body;

	dbus_uint32_t message_id;
};

Notification notification_new(const char* summary, const char* body) {
	struct _tinynotify_notification *ret;

	assert(summary);

	assert(ret = malloc(sizeof(*ret)));
	assert(ret->summary = strdup(summary));
	if (body && *body)
		assert(ret->body = strdup(body));
	else
		ret->body = NULL;
	ret->message_id = 0;

	return ret;
}

void notification_free(Notification notification) {
	struct _tinynotify_notification *n = notification;

	if (n->body)
		free(n->body);
	free(n->summary);

	free(notification);
}

NotifyError notification_send(NotifySession session, Notification notification) {
	struct _tinynotify_notify_session *s = session;
	struct _tinynotify_notification *n = notification;
	NotifyError ret;
	char *err_msg;

	DBusMessage *msg, *reply;
	DBusMessageIter iter, subiter;
	DBusError err;

	const char *app_name = s->app_name ? s->app_name : "";
	dbus_uint32_t replaces_id = 0;
	const char *app_icon = s->app_icon ? s->app_icon : "";
	const char *summary = n->summary;
	const char *body = n->body ? n->body : "";
	dbus_int32_t expire_timeout = -1;

	if (notify_session_connect(session))
		return s->error;

	assert(msg = dbus_message_new_method_call("org.freedesktop.Notifications",
				"/org/freedesktop/Notifications",
				"org.freedesktop.Notifications",
				"Notify"));

	assert(dbus_message_append_args(msg,
				DBUS_TYPE_STRING, &app_name,
				DBUS_TYPE_UINT32, &replaces_id,
				DBUS_TYPE_STRING, &app_icon,
				DBUS_TYPE_STRING, &summary,
				DBUS_TYPE_STRING, &body,
				DBUS_TYPE_INVALID));

	dbus_message_iter_init_append(msg, &iter);

	/* actions */
	assert(dbus_message_iter_open_container(&iter,
				DBUS_TYPE_ARRAY, DBUS_TYPE_STRING_AS_STRING, &subiter));
	assert(dbus_message_iter_close_container(&iter, &subiter));

	/* hints */
	assert(dbus_message_iter_open_container(&iter,
				DBUS_TYPE_ARRAY,
				DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING
				DBUS_TYPE_STRING_AS_STRING
				DBUS_TYPE_VARIANT_AS_STRING
				DBUS_DICT_ENTRY_END_CHAR_AS_STRING,
				&subiter));
	assert(dbus_message_iter_close_container(&iter, &subiter));

	assert(dbus_message_iter_append_basic(&iter,
				DBUS_TYPE_INT32, &expire_timeout));

	dbus_error_init(&err);
	reply = dbus_connection_send_with_reply_and_block(s->conn,
			msg, 5000 /* XXX */, &err);

	assert(!reply == dbus_error_is_set(&err));
	if (!reply) {
		err_msg = strdup(err.message);
		dbus_error_free(&err);
		ret = NOTIFY_ERROR_DBUS_SEND;
	} else {
		dbus_uint32_t new_id;

		assert(dbus_message_get_type(reply) == DBUS_MESSAGE_TYPE_METHOD_RETURN);

		if (!dbus_message_get_args(reply, &err,
					DBUS_TYPE_UINT32, &new_id,
					DBUS_TYPE_INVALID)) {
			err_msg = strdup(err.message);
			dbus_error_free(&err);
			ret = NOTIFY_ERROR_INVALID_REPLY;
		} else {
			n->message_id = new_id;
			err_msg = NULL;
			ret = NOTIFY_ERROR_NO_ERROR;
		}

		dbus_message_unref(reply);
	}

	dbus_message_unref(msg);
	return _notify_session_set_error(s, ret, err_msg);
}
