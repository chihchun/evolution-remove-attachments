SUBDIRS = src po

ACLOCAL_AMFLAGS = -I m4

INTLTOOL =                      \
        intltool-extract.in     \
        intltool-merge.in       \
        intltool-update.in	\
	po/POTFILES.in

EXTRA_DIST = \
	$(INTLTOOL)		\
	autogen.sh		\
	ChangeLog

DISTCLEANFILES = \
  intltool-extract \
  intltool-merge \
  intltool-update
