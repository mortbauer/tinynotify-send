ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS =

if ENABLE_REGULAR
bin_PROGRAMS += tinynotify-send

tinynotify_send_SOURCES = src/tinynotify-send.c
endif

if ENABLE_SYSTEMWIDE
bin_PROGRAMS += sw-notify-send

sw_notify_send_CPPFLAGS = $(LIBTINYNOTIFY_SYSTEMWIDE_CFLAGS) -DBUILDING_SYSTEMWIDE
sw_notify_send_LDADD = $(LIBTINYNOTIFY_SYSTEMWIDE_LIBS)
sw_notify_send_SOURCES = src/tinynotify-send.c
endif
