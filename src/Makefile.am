INCLUDES =							\
	-I$(EVOLUTION_SOURCE)					\
	$(EVOLUTION_MAIL_CFLAGS)				\
	$$(pkg-config  --cflags evolution-plugin)		\
	-I$(top_srcdir)

%.eplug: %.eplug.xml.in.in
	sed -e "s%+PLUGIN_INSTALL_DIR+%$(PLUGIN_INSTALL_DIR)%" \
	-e "s%@PLUGINDIR@%$(PLUGIN_INSTALL_DIR)%" \
	-e "s%+GETTEXT_PACKAGE+%$(GETTEXT_PACKAGE)%" \
	-e "s%+LOCALEDIR+%$(LOCALEDIR)%" \
	-e "s%+VERSION+%$(VERSION)%" \
	-e "s%+URL+%http://people.debian.org.tw/~chihchun/%" \
	$< > $@

plugindir = $(PLUGIN_INSTALL_DIR)
plugin_DATA = org-gnome-remove-attachments.eplug
plugin_LTLIBRARIES = liborg-gnome-evolution-remove-attachments.la

liborg_gnome_evolution_remove_attachments_la_SOURCES = remove-attachments.c
liborg_gnome_evolution_remove_attachments_la_LDFLAGS = -module -avoid-version 

EXTRA_DIST = org-gnome-remove-attachments.eplug.xml.in.in

BUILT_SOURCES = org-gnome-remove-attachments.eplug
CLEANFILES = $(BUILT_SOURCES)

