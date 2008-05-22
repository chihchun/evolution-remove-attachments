INCLUDES =						\
	-I$(top_srcdir)					\
	$(EVOLUTION_MAIL_CFLAGS)

@EVO_PLUGIN_RULE@

plugin_DATA = org-gnome-remove-attachments.eplug org-gnome-remove-attachments.xml
plugin_LTLIBRARIES = liborg-gnome-remove-attachments.la

liborg_gnome_remove_attachments_la_SOURCES = remove-attachments.c
liborg_gnome_remove_attachments_la_LDFLAGS = -module -avoid-version

EXTRA_DIST = org-gnome-remove-attachments.eplug.xml \
	     org-gnome-remove-attachments.xml

BUILT_SOURCES = org-gnome-remove-attachments.eplug
CLEANFILES = $(BUILT_SOURCES)
