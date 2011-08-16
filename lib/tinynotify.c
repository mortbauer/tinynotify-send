/* libtinynotify
 * (c) 2011 Michał Górny
 * 2-clause BSD-licensed
 */

#include "config.h"
#include "tinynotify.h"

#include <stdlib.h>
#include <assert.h>

struct _tinynotify_notify_session {
	int connected;

	NotifyError error;
	char* error_details;
};

NotifySession notify_session_new(void) {
	struct _tinynotify_notify_session *ret;

	assert(ret = malloc(sizeof(*ret)));
	ret->connected = 0;
	ret->error = NOTIFY_ERROR_NO_ERROR;
	return ret;
}

void notify_session_free(NotifySession session) {
	free(session);
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
