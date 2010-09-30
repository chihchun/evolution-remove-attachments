/* This file is licensed under the GNU GPL v2 or later 
 * This code base is from mail-to-task and the other evolution plugins 
 *
 * Authors:
 * Rex Tsai <chihchun@kalug.linux.org.tw>
 *
 * Copyright (C) 2006 - 2010 Rex Tsai
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <stdio.h>
#include <glib/gi18n-lib.h>

#include "e-util/e-config.h"
#include "e-util/e-dialog-utils.h"
#include "mail/e-mail-browser.h"
#include "mail/e-mail-reader.h"
#include "shell/e-shell-content.h"
#include "shell/e-shell-view.h"

gboolean mail_browser_init (GtkUIManager *ui_manager, EMailBrowser *browser);
gboolean mail_shell_view_init (GtkUIManager *ui_manager, EShellView *shell_view);
static void action_mail_remove_attachment_cb (GtkAction *action, EMailReader *reader);

static GtkActionEntry multi_selection_entries[] = {
    { "mail-remove-attachment",
        "stock_delete",
        "_Remove attachments",
        NULL,
        "Remove attachment",
        G_CALLBACK (action_mail_remove_attachment_cb) },
};

static void
setup_actions (EMailReader *reader, GtkUIManager *ui_manager)
{
    GtkActionGroup *action_group;

    action_group = gtk_action_group_new ("mail-remove-attachments");
    gtk_action_group_add_actions ( action_group, multi_selection_entries, G_N_ELEMENTS (multi_selection_entries), reader);
    gtk_ui_manager_insert_action_group (ui_manager, action_group, 0);
    g_object_unref (action_group);
}

gboolean mail_browser_init (GtkUIManager *ui_manager, EMailBrowser *browser)
{
    setup_actions (E_MAIL_READER (browser), ui_manager);
    return TRUE;
}

gboolean mail_shell_view_init (GtkUIManager *ui_manager, EShellView *shell_view)
{
    EShellContent *shell_content;
    shell_content = e_shell_view_get_shell_content (shell_view);
    setup_actions (E_MAIL_READER (shell_content), ui_manager);
    return TRUE;
}

static void
action_mail_remove_attachment_cb (GtkAction *action, EMailReader *reader)
{
    CamelFolder *folder;
    GPtrArray *uids;
    int i,j;

    folder = e_mail_reader_get_folder (reader);
    uids = e_mail_reader_get_selected_uids (reader);

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
                if (disposition && (!strcmp(disposition, "attachment") || !strcmp(disposition, "inline"))) {
                    char *desc;
                    const char *filename;

                    filename = camel_mime_part_get_filename(mpart);
                    desc = g_strdup_printf("File \"%s\" has been removed.", filename);
                    camel_mime_part_set_disposition (mpart, "inline");
                    camel_mime_part_set_content(mpart, desc, strlen(desc), "text/plain");
                    camel_mime_part_set_content_type(mpart, "text/plain");
                    deleted = TRUE;
                }
            }

            if(deleted) {
                // copy the original message with the deleted attachment.
                CamelMessageInfo *info, *newinfo;
                guint32 flags;
                CamelException *ex = camel_exception_new();

                info = camel_folder_get_message_info (folder, uid);
                newinfo = camel_message_info_new_from_header(NULL, ((CamelMimePart *)message)->headers);
                flags = camel_folder_get_message_flags(folder, uid);

                // make a copy of the message.
                camel_message_info_set_flags(newinfo, flags, flags);
                camel_folder_append_message(folder, message, newinfo, NULL, ex);
                
                if (!camel_exception_is_set (ex)) {
                    // marked the original message deleted.
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
