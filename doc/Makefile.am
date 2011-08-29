DOC_MODULE = libtinynotify-cli
DOC_MODULE_VERSION = $(PACKAGE_VERSION)

DOC_MAIN_SGML_FILE = $(DOC_MODULE)-docs.sgml

DOC_SOURCE_DIR = $(top_srcdir)/lib/

# Used for dependencies. The docs will be rebuilt if any of these change.
HFILE_GLOB = $(top_srcdir)/lib/*.h
CFILE_GLOB = $(top_srcdir)/lib/*.c

# This includes the standard gtk-doc make rules, copied by gtkdocize.
include $(top_srcdir)/gtk-doc.make

# Comment this out if you want 'make check' to test you doc status
# and run some sanity checks
if ENABLE_GTK_DOC
TESTS_ENVIRONMENT = cd $(srcdir) && \
	DOC_MODULE=$(DOC_MODULE) DOC_MAIN_SGML_FILE=$(DOC_MAIN_SGML_FILE) \
	SRCDIR=$(abs_srcdir) BUILDDIR=$(abs_builddir)
TESTS = $(GTKDOC_CHECK)
endif
