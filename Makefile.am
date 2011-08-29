ACLOCAL_AMFLAGS = -I m4
DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

if ENABLE_LIBRARY
SUBDIRS = doc

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libtinynotify-cli.la
pkgconfig_DATA = libtinynotify-cli.pc
include_HEADERS = lib/tinynotify-cli.h

libtinynotify_cli_la_LDFLAGS = -version-info 1:0:0
libtinynotify_cli_la_CPPFLAGS = $(LIBTINYNOTIFY_CFLAGS)
libtinynotify_cli_la_LIBADD = $(LIBTINYNOTIFY_LIBS)
libtinynotify_cli_la_SOURCES = lib/tinynotify-cli.c lib/tinynotify-cli.h
endif

bin_PROGRAMS =

if ENABLE_REGULAR
bin_PROGRAMS += tinynotify-send

if ENABLE_LIBRARY
tinynotify_send_CPPFLAGS = -I$(top_srcdir)/lib
tinynotify_send_LDADD = libtinynotify-cli.la
endif
tinynotify_send_SOURCES = src/tinynotify-send.c
endif

if ENABLE_SYSTEMWIDE
bin_PROGRAMS += sw-notify-send

sw_notify_send_CPPFLAGS = $(LIBTINYNOTIFY_SYSTEMWIDE_CFLAGS) -DBUILDING_SYSTEMWIDE
sw_notify_send_LDADD = $(LIBTINYNOTIFY_SYSTEMWIDE_LIBS)
if ENABLE_LIBRARY
sw_notify_send_CPPFLAGS += -I$(top_srcdir)/lib
sw_notify_send_LDADD += libtinynotify-cli.la
endif
sw_notify_send_SOURCES = src/tinynotify-send.c
endif
