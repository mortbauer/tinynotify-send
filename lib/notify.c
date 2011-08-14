/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include <dbus/dbus.h>

#include "notify.h"

struct _tinynotify_session {
	int initiated;

	char *app_name;
	DBusConnection *conn;
};

struct _tinynotify_session _notify_session = {0};

int notify_init(const char* const app_name) {
	DBusError err;

	if (_notify_session.initiated)
		return 1; /* XXX */

	dbus_error_init(&err);
	/* We're just a small lib, don't fiddle with our parent's potential
	 * connections. */
	_notify_session.conn = dbus_bus_get_private(DBUS_BUS_SESSION, &err);

	if (dbus_error_is_set(&err)) {
		dbus_error_free(&err);
		return 1; /* XXX */
	}

	dbus_connection_set_exit_on_disconnect(_notify_session.conn, FALSE);

	_notify_session.app_name = strdup(app_name ? app_name : "");
	_notify_session.initiated = 1;
	return 0;
}

void notify_uninit(void) {
	if (!_notify_session.initiated)
		return;

	dbus_connection_close(_notify_session.conn);
	dbus_connection_unref(_notify_session.conn);
	free(_notify_session.app_name);
	_notify_session.initiated = 0;
}
