ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = tinynotify-send.1.in
DISTCLEANFILES =

if ENABLE_LIBRARY

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libtinynotify-cli.la
pkgconfig_DATA = libtinynotify-cli.pc
include_HEADERS = lib/tinynotify-cli.h

libtinynotify_cli_la_LDFLAGS = -version-info 2:0:1
libtinynotify_cli_la_CPPFLAGS = $(LIBTINYNOTIFY_CFLAGS)
libtinynotify_cli_la_LIBADD = $(LIBTINYNOTIFY_LIBS)
libtinynotify_cli_la_SOURCES = lib/tinynotify-cli.c lib/tinynotify-cli.h
endif

bin_PROGRAMS =
dist_man1_MANS =

if ENABLE_REGULAR
bin_PROGRAMS += tinynotify-send

DISTCLEANFILES += tinynotify-send.1

if ENABLE_LIBRARY
tinynotify_send_CPPFLAGS = -I$(top_srcdir)/lib
tinynotify_send_LDADD = libtinynotify-cli.la
endif
tinynotify_send_SOURCES = src/tinynotify-send.c

tinynotify-send.1: tinynotify-send.1.in
	rm -f $@ $@.tmp
	$(SED) \
		-e "s|@PROGRAM[@]|tinynotify-send|g" \
		-e "s|@VERSION[@]|@VERSION@|g" \
		$< > $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@
endif

if ENABLE_SYSTEMWIDE
bin_PROGRAMS += sw-notify-send
dist_man1_MANS += sw-notify-send.1
DISTCLEANFILES += sw-notify-send.1

sw_notify_send_CPPFLAGS = $(LIBTINYNOTIFY_SYSTEMWIDE_CFLAGS) -DBUILDING_SYSTEMWIDE
sw_notify_send_LDADD = $(LIBTINYNOTIFY_SYSTEMWIDE_LIBS)
if ENABLE_LIBRARY
sw_notify_send_CPPFLAGS += -I$(top_srcdir)/lib
sw_notify_send_LDADD += libtinynotify-cli.la
endif
sw_notify_send_SOURCES = src/tinynotify-send.c

sw-notify-send.1: tinynotify-send.1.in
	rm -f $@ $@.tmp
	$(SED) \
		-e "s|@PROGRAM[@]|sw-notify-send|g" \
		-e "s|@VERSION[@]|@VERSION@|g" \
		$< > $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@
endif

EXTRA_DIST += NEWS
NEWS: configure.ac Makefile.am
	git for-each-ref refs/tags --sort '-*committerdate' \
		--format '# %(tag) (%(*committerdate:short))%0a%(contents:body)' \
		> NEWS
