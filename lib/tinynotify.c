/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#include "config.h"
#include "tinynotify.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <assert.h>
#define _mem_assert(x) _mem_check(!!(x), #x)

#include <dbus/dbus.h>

void _mem_check(int res, const char* stmt) {
	if (!res) {
		fputs("Memory allocation failed for: ", stderr);
		fputs(stmt, stderr);
		fputs("\n", stderr);
		abort();
	}
}

struct _notify_session {
	DBusConnection *conn;

	char* app_name;
	char* app_icon;

	NotifyError error;
	char* error_details;
};

static const char* _error_messages[NOTIFY_ERROR_COUNT] = {
	"No error",
	"Connecting to D-Bus failed: %s",
	"Sending message over D-Bus failed: %s",
	"Invalid reply received: %s"
};

static NotifyError _notify_session_set_error(
		NotifySession s,
		NotifyError new_error,
		char *error_details)
{
	if (s->error_details)
		free(s->error_details);
	s->error = new_error;
	_mem_assert(asprintf(&s->error_details, _error_messages[new_error],
			error_details) != -1);

	return new_error;
}

NotifySession notify_session_new(const char* app_name, const char* app_icon) {
	NotifySession s;

	_mem_assert(s = malloc(sizeof(*s)));
	s->conn = NULL;
	s->app_name = NULL;
	s->app_icon = NULL;
	s->error_details = NULL;

	_notify_session_set_error(s, NOTIFY_ERROR_NO_ERROR, NULL);
	notify_session_set_app_name(s, app_name);
	notify_session_set_app_icon(s, app_icon);
	return s;
}

void notify_session_free(NotifySession s) {
	notify_session_disconnect(s);

	if (s->error_details)
		free(s->error_details);
	free(s->app_name);
	free(s->app_icon);
	free(s);
}

NotifyError notify_session_get_error(NotifySession s) {
	return s->error;
}

const char* notify_session_get_error_message(NotifySession s) {
	return s->error_details;
}

NotifyError notify_session_connect(NotifySession s) {
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

void notify_session_disconnect(NotifySession s) {
	if (s->conn) {
		dbus_connection_close(s->conn);
		dbus_connection_unref(s->conn);
		s->conn = NULL;
	}

	_notify_session_set_error(s, NOTIFY_ERROR_NO_ERROR, NULL);
}

const char* NOTIFY_SESSION_NO_APP_NAME = NULL;

void notify_session_set_app_name(NotifySession s, const char* app_name) {
	if (s->app_name)
		free(s->app_name);
	if (app_name && *app_name)
		_mem_assert(s->app_name = strdup(app_name));
	else
		s->app_name = NULL;
}

const char* NOTIFY_SESSION_NO_APP_ICON = NULL;

void notify_session_set_app_icon(NotifySession s, const char* app_icon) {
	if (s->app_icon)
		free(s->app_icon);
	if (app_icon && *app_icon)
		_mem_assert(s->app_icon = strdup(app_icon));
	else
		s->app_icon = NULL;
}

struct _notification {
	char* summary;
	char* body;

	char* app_icon;

	dbus_uint32_t message_id;
};

const char* NOTIFICATION_NO_BODY = NULL;

Notification notification_new(const char* summary, const char* body) {
	Notification n;

	assert(summary);

	_mem_assert(n = malloc(sizeof(*n)));
	/* can't use notification_set_summary() here because it has to free sth */
	_mem_assert(n->summary = strdup(summary));
	n->body = NULL;
	n->app_icon = NULL;
	n->message_id = 0;

	notification_set_body(n, body);
	return n;
}

void notification_free(Notification n) {
	free(n->summary);
	if (n->body)
		free(n->body);
	if (n->app_icon)
		free(n->app_icon);
	free(n);
}

const char* NOTIFICATION_DEFAULT_APP_ICON = NULL;
const char* NOTIFICATION_NO_APP_ICON = "";

void notification_set_app_icon(Notification n, const char* app_icon) {
	if (n->app_icon)
		free(n->app_icon);
	if (app_icon)
		_mem_assert(n->app_icon = strdup(app_icon));
	else
		n->app_icon = NULL;
}

void notification_set_summary(Notification n, const char* summary) {
	free(n->summary);
	assert(summary);
	_mem_assert(n->summary = strdup(summary));
}

void notification_set_body(Notification n, const char* body) {
	if (n->body)
		free(n->body);
	if (body && *body)
		_mem_assert(n->body = strdup(body));
	else
		n->body = NULL;
}

static NotifyError notification_update_va(Notification n, NotifySession s, va_list ap) {
	NotifyError ret;
	char *err_msg;
	char *f_summary, *f_body;

	DBusMessage *msg, *reply;
	DBusMessageIter iter, subiter;
	DBusError err;

	const char *app_name = s->app_name ? s->app_name : "";
	dbus_uint32_t replaces_id = n->message_id;
	const char *app_icon = n->app_icon ? n->app_icon :
			s->app_icon ? s->app_icon : "";
	const char *summary = n->summary;
	const char *body = n->body ? n->body : "";
	dbus_int32_t expire_timeout = -1;

	if (notify_session_connect(s))
		return s->error;

	_mem_assert(vasprintf(&f_summary, summary, ap) != -1);
	_mem_assert(vasprintf(&f_body, body, ap) != -1);

	_mem_assert(msg = dbus_message_new_method_call("org.freedesktop.Notifications",
				"/org/freedesktop/Notifications",
				"org.freedesktop.Notifications",
				"Notify"));

	_mem_assert(dbus_message_append_args(msg,
				DBUS_TYPE_STRING, &app_name,
				DBUS_TYPE_UINT32, &replaces_id,
				DBUS_TYPE_STRING, &app_icon,
				DBUS_TYPE_STRING, &f_summary,
				DBUS_TYPE_STRING, &f_body,
				DBUS_TYPE_INVALID));

	dbus_message_iter_init_append(msg, &iter);

	/* actions */
	_mem_assert(dbus_message_iter_open_container(&iter,
				DBUS_TYPE_ARRAY, DBUS_TYPE_STRING_AS_STRING, &subiter));
	_mem_assert(dbus_message_iter_close_container(&iter, &subiter));

	/* hints */
	_mem_assert(dbus_message_iter_open_container(&iter,
				DBUS_TYPE_ARRAY,
				DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING
				DBUS_TYPE_STRING_AS_STRING
				DBUS_TYPE_VARIANT_AS_STRING
				DBUS_DICT_ENTRY_END_CHAR_AS_STRING,
				&subiter));
	_mem_assert(dbus_message_iter_close_container(&iter, &subiter));

	_mem_assert(dbus_message_iter_append_basic(&iter,
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
	free(f_summary);
	free(f_body);
	return _notify_session_set_error(s, ret, err_msg);
}

static NotifyError notification_send_va(Notification n, NotifySession s, va_list ap) {
	n->message_id = 0;
	return notification_update_va(n, s, ap);
}

NotifyError notification_send(Notification n, NotifySession s, ...) {
	va_list ap;
	NotifyError ret;

	va_start(ap, s);
	ret = notification_send_va(n, s, ap);
	va_end(ap);

	return ret;
}

NotifyError notification_update(Notification n, NotifySession s, ...) {
	va_list ap;
	NotifyError ret;

	va_start(ap, s);
	ret = notification_update_va(n, s, ap);
	va_end(ap);

	return ret;
}
