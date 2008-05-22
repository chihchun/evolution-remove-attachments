/* Copyright (C) 2004-2006 Novell, Inc */
/* Authors: Rex Tsai */

/* This file is licensed under the GNU GPL v2 or later */
/* This code base is from mail-to-task and the other evolution plugins */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glib/gi18n-lib.h>
#include <string.h>
#include <stdio.h>

#include <gconf/gconf-client.h>
#include <libecal/e-cal.h>
#include <libedataserverui/e-source-selector-dialog.h>
#include <camel/camel-folder.h>
#include <camel/camel-medium.h>
#include <camel/camel-mime-message.h>
#include <camel/camel-multipart.h>
#include <camel/camel-stream.h>
#include <camel/camel-stream-mem.h>
#include <camel/camel-utf8.h>
#include "mail/em-menu.h"
#include "mail/em-popup.h"
#include "mail/em-utils.h"
#include <calendar/common/authentication.h>

typedef struct {
	ECal *client;
	struct _CamelFolder *folder;
	GPtrArray *uids;
} AsyncData;

void org_gnome_remove_attachments (void *ep, EMPopupTargetSelect *t);
void org_gnome_remove_attachments_menu (EPlugin *ep, EMMenuTargetSelect *t);

static void
copy_uids (char *uid, GPtrArray *uid_array) 
{
	g_ptr_array_add (uid_array, g_strdup (uid));
}

static void
remove_attachments (GPtrArray *uid_array, struct _CamelFolder *folder)
{
    GPtrArray *uids = uid_array;
    int i,j;

    camel_folder_freeze(folder);
    for (i = 0; i < (uids ? uids->len : 0); i++) {
        CamelMimeMessage *message;
        CamelDataWrapper *containee;
        char *uid;
        gboolean deleted = FALSE;
        
        uid = g_ptr_array_index (uids, i);

        /* retrieve the message from the CamelFolder */
        message = camel_folder_get_message (folder, uid, NULL);
        if (!message) {
            continue;
        }

	containee = camel_medium_get_content_object((CamelMedium *)message);
	if (containee == NULL) {
            continue;
        }

	if (CAMEL_IS_MULTIPART(containee)) {
            int parts;

            parts = camel_multipart_get_number((CamelMultipart *)containee);
            for (j = 0; j < parts; j++) {
                CamelMimePart *mpart = camel_multipart_get_part((CamelMultipart *)containee, j);
                const char *disposition = camel_mime_part_get_disposition(mpart);
                if (disposition && !strcmp(disposition, "attachment")) {
                    char *desc;
                    const char *filename;

                    filename = camel_mime_part_get_filename(mpart);
                    g_warning ("Deleteing \"%s\"\n", filename);

                    desc = g_strdup_printf(_("File \"%s\" has been removed."), filename);
                    camel_mime_part_set_disposition (mpart, "inline");
                    camel_mime_part_set_content(mpart, desc, strlen(desc), "text/plain");
                    camel_mime_part_set_content_type(mpart, "text/plain");
                    deleted = TRUE;
                }
            }

            if(deleted) {
                CamelMessageInfo *info, *newinfo;
                guint32 flags;
                CamelException *ex = camel_exception_new();

                info = camel_folder_get_message_info (folder, uid);
                newinfo = camel_message_info_new_from_header(NULL, ((CamelMimePart *)message)->headers);
                flags = camel_folder_get_message_flags(folder, uid);

                // make a copy.
                camel_message_info_set_flags(newinfo, flags, flags);
                camel_folder_append_message(folder, message, newinfo, NULL, ex);
                
                if (!camel_exception_is_set (ex)) {
                    // marked to delete.
                    camel_message_info_set_flags(info, CAMEL_MESSAGE_DELETED, CAMEL_MESSAGE_DELETED);
                    camel_folder_free_message_info(folder, info);
                }

                camel_exception_free(ex);
            }
        }
    }
    camel_folder_sync(folder, FALSE, NULL);
    camel_folder_thaw(folder);
}


void
org_gnome_remove_attachments (void *ep, EMPopupTargetSelect *t)
{
	GPtrArray *uid_array = NULL;

	if (t->uids->len > 0) {
		/* FIXME Some how in the thread function the values inside t->uids gets freed 
		   and are corrupted which needs to be fixed, this is sought of work around fix for
		   the gui inresponsiveness */
		uid_array = g_ptr_array_new ();
		g_ptr_array_foreach (t->uids, (GFunc)copy_uids, (gpointer) uid_array);
	} else {
		return;
	}

	remove_attachments (uid_array, t->folder);
}

void org_gnome_remove_attachments_menu (EPlugin *ep, EMMenuTargetSelect *t)
{
	GPtrArray *uid_array = NULL;

	if (t->uids->len > 0) {
		/* FIXME Some how in the thread function the values inside t->uids gets freed 
		   and are corrupted which needs to be fixed, this is sought of work around fix for
		   the gui inresponsiveness */
		uid_array = g_ptr_array_new ();
		g_ptr_array_foreach (t->uids, (GFunc)copy_uids, (gpointer) uid_array);
	} else {
		return;
	}

	remove_attachments (uid_array, t->folder);
}

int e_plugin_lib_enable(EPluginLib *ep, int enable);

int
e_plugin_lib_enable(EPluginLib *ep, int enable)
{
	return 0;
}
