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
};

NotifySession notify_session_new(void) {
	struct _tinynotify_notify_session *ret;

	assert(ret = malloc(sizeof(*ret)));
	ret->connected = 0;
	return ret;
}

void notify_session_free(NotifySession session) {
	free(session);
}
