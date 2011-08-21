AC_PREREQ([2.60])
AC_INIT([tinynotify-send], [0.0.1])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.6 foreign dist-bzip2 no-dependencies -Wall])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES])

AC_PROG_CC
AM_PROG_CC_C_O
AC_USE_SYSTEM_EXTENSIONS

PKG_CHECK_MODULES([LIBTINYNOTIFY], [libtinynotify libtinynotify-cli])

AC_ARG_ENABLE([regular],
	[AS_HELP_STRING([--disable-regular],
		[Disable building regular tinynotify-send])])
AM_CONDITIONAL([ENABLE_REGULAR], [test x"$enable_regular" != x"no"])

AC_ARG_ENABLE([system-wide],
	[AS_HELP_STRING([--disable-system-wide],
		[Disable building system-wide tinynotify-send (sw-notify-send)])])
AS_IF([test x"$enable_system_wide" != x"no"], [
	PKG_CHECK_MODULES([LIBTINYNOTIFY_SYSTEMWIDE], [libtinynotify-systemwide],, [
		AC_MSG_WARN([libtinynotify-systemwide not found, disabling system-wide])
		enable_system_wide=no
	])
])
AM_CONDITIONAL([ENABLE_SYSTEMWIDE], [test x"$enable_systemwide" != x"no"])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
