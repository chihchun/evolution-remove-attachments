SUBDIRS = src po

ACLOCAL_AMFLAGS = -I m4

INTLTOOL =                      \
        intltool-extract.in     \
        intltool-merge.in       \
        intltool-update.in

DPKG_DIST= \
	     debian/rules \
	     debian/changelog \
	     debian/copyright \
	     debian/compat \
	     debian/docs \
	     debian/control \
	     debian/evolution-remove-attachments.install

EXTRA_DIST = \
	$(INTLTOOL)		\
	po/POTFILES.in		\
	autogen.sh		\
	ChangeLog

DISTCLEANFILES = \
  intltool-extract \
  intltool-merge \
  intltool-update
