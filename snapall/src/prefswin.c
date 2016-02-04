/* Snapshot All Windows 
 * prefswin.c
 *
 *Copyright (C) 2005 Eugene Morenko(More)
 *mailto: more@irpin.com
 *web: http://snapall.linux.kiev.ua/
 *icq:325697438
 *

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <X11/X.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>



#include "snap.h"
#include "utils.h"
#include "key.h"

static GtkWidget *prefwin;
static GtkWidget *entry1;
static GtkWidget *spinbutton1;
static GtkWidget *entry2;
static GtkWidget *spinbutton2;
static GtkWidget *entry3;
static GtkWidget *spinbutton3;

void save_confif_to_file(GtkButton *button,Snap *snap)
{
	ConfigFile *cfgfile;
	gchar *filename;
	int key=0;
	int mod=0;

	key=StringToModifier_Key(g_strdup(snap->key->root_wind),&mod);
	ungrab_key (snap->screen,key);
	
	key=StringToModifier_Key(g_strdup(snap->key->id_wind),&mod);
	ungrab_key (snap->screen,key);
	
	key=StringToModifier_Key(g_strdup(snap->key->x_y_wind),&mod);
	ungrab_key (snap->screen,key);
	
	snap->key->root_wind=g_strdup(gtk_entry_get_text (GTK_ENTRY (entry1)));
	snap->key->t_root_wind=gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinbutton1));
	
	snap->key->id_wind=g_strdup(gtk_entry_get_text (GTK_ENTRY (entry2)));
	snap->key->t_id_wind=gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinbutton2));
	
	snap->key->x_y_wind=g_strdup(gtk_entry_get_text (GTK_ENTRY (entry3)));
	snap->key->t_x_y_wind=gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinbutton3));
	
	/*printf("SP:%d\n",snap->key->t_root_wind);
	printf("SP:%d\n",snap->key->t_id_wind);
	printf("SP:%d\n",snap->key->t_x_y_wind);*/
	
	filename= g_strconcat(g_get_home_dir(),"/.snap/config", NULL);
	
	//printf("%s\n",filename);
	cfgfile = snap_cfg_open_file(filename);

	if (!cfgfile)
		cfgfile = snap_cfg_new();

	snap_cfg_write_string (cfgfile, "snap", "root_wind", snap->key->root_wind);
	snap_cfg_write_int (cfgfile, "snap", "t_root_wind", snap->key->t_root_wind);
	
	snap_cfg_write_string (cfgfile, "snap", "id_wind", snap->key->id_wind);
	snap_cfg_write_int (cfgfile, "snap", "t_id_wind", snap->key->t_id_wind);
	
	snap_cfg_write_string (cfgfile, "snap", "x_y_wind",snap->key->x_y_wind);
	snap_cfg_write_int (cfgfile, "snap", "t_x_y_wind",snap->key->t_x_y_wind);

	snap_cfg_write_file(cfgfile, filename);
	
	
	key=StringToModifier_Key(g_strdup(snap->key->root_wind),&mod);
	grab_key (snap->screen,key,mod);
	
	key=StringToModifier_Key(g_strdup(snap->key->id_wind),&mod);
	grab_key (snap->screen,key,mod);
	
	key=StringToModifier_Key(g_strdup(snap->key->x_y_wind),&mod);
	grab_key (snap->screen,key,mod);
	
	
	snap_cfg_free(cfgfile);
	g_free(filename);
	
	gtk_widget_destroy(prefwin);
	prefwin=NULL;
	
	
}

static void root_press (GtkWidget *widget,GdkEventKey *event,Snap *snap)
{
	char str[1000];
	char *mkey;

	ModifierToString (event->state, str);

	mkey=g_strdup_printf ("%s%s%s",str,str[0] ? "+" : "",
		      (XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (), event->hardware_keycode, 0)) != NULL)
		      ? XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (),event->hardware_keycode, 0))
		      : g_strdup_printf ("%d",event->hardware_keycode));

	gtk_entry_set_text (GTK_ENTRY (entry1),mkey);
	g_free(mkey);
}
static void root_release (GtkWidget *widget,GdkEventKey *event,Snap *snap)
{
	guint i = 0;
	char **key_stat = g_strsplit(g_strdup(gtk_entry_get_text (GTK_ENTRY (entry1))),"+",4);

	while (key_stat[i])
		i++;

	if((g_strcasecmp (key_stat[i-1],"Control")==0)|| (g_strcasecmp (key_stat[i-1],"Shift")==0)|| (g_strcasecmp (key_stat[i-1],"Alt")==0)|| (g_strcasecmp (key_stat[i-1],"Control_R")==0)|| (g_strcasecmp (key_stat[i-1],"Shift_R")==0)|| (g_strcasecmp (key_stat[i-1],"Alt_R")==0)|| (g_strcasecmp (key_stat[i-1],"Control_L")==0)|| (g_strcasecmp (key_stat[i-1],"Shift_L")==0)|| (g_strcasecmp (key_stat[i-1],"Alt_l")==0))
	gtk_entry_set_text (GTK_ENTRY (entry1),_("None"));
}

static void id_wind_press (GtkWidget *widget,GdkEventKey *event,Snap *snap)
{
	char str[1000];
	char *mkey;

	ModifierToString (event->state, str);

	mkey=g_strdup_printf ("%s%s%s",str,str[0] ? "+" : "",
		      (XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (), event->hardware_keycode, 0)) != NULL)
		      ? XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (),event->hardware_keycode, 0))
		      : g_strdup_printf ("%d",event->hardware_keycode));

	gtk_entry_set_text (GTK_ENTRY (entry2),mkey);
	g_free(mkey);
}
static void id_wind_release (GtkWidget *widget,GdkEventKey *event,Snap *snap)
{
	guint i = 0;
	char **key_stat = g_strsplit(g_strdup(gtk_entry_get_text (GTK_ENTRY (entry2))),"+",4);

	while (key_stat[i])
		i++;

	if((g_strcasecmp (key_stat[i-1],"Control")==0)|| (g_strcasecmp (key_stat[i-1],"Shift")==0)|| (g_strcasecmp (key_stat[i-1],"Alt")==0)|| (g_strcasecmp (key_stat[i-1],"Control_R")==0)|| (g_strcasecmp (key_stat[i-1],"Shift_R")==0)|| (g_strcasecmp (key_stat[i-1],"Alt_R")==0)|| (g_strcasecmp (key_stat[i-1],"Control_L")==0)|| (g_strcasecmp (key_stat[i-1],"Shift_L")==0)|| (g_strcasecmp (key_stat[i-1],"Alt_l")==0))
	gtk_entry_set_text (GTK_ENTRY (entry2),_("None"));
}

static void x_y_wind_press (GtkWidget *widget,GdkEventKey *event,Snap *snap)
{
	char str[1000];
	char *mkey;

	ModifierToString (event->state, str);

	mkey=g_strdup_printf ("%s%s%s",str,str[0] ? "+" : "",
		      (XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (), event->hardware_keycode, 0)) != NULL)
		      ? XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (),event->hardware_keycode, 0))
		      : g_strdup_printf ("%d",event->hardware_keycode));

	gtk_entry_set_text (GTK_ENTRY (entry3),mkey);
	g_free(mkey);
}
static void x_y_wind_release (GtkWidget *widget,GdkEventKey *event,Snap *snap)
{
	guint i = 0;
	char **key_stat = g_strsplit(g_strdup(gtk_entry_get_text (GTK_ENTRY (entry3))),"+",4);

	while (key_stat[i])
		i++;

	if((g_strcasecmp (key_stat[i-1],"Control")==0)|| (g_strcasecmp (key_stat[i-1],"Shift")==0)|| (g_strcasecmp (key_stat[i-1],"Alt")==0)|| (g_strcasecmp (key_stat[i-1],"Control_R")==0)|| (g_strcasecmp (key_stat[i-1],"Shift_R")==0)|| (g_strcasecmp (key_stat[i-1],"Alt_R")==0)|| (g_strcasecmp (key_stat[i-1],"Control_L")==0)|| (g_strcasecmp (key_stat[i-1],"Shift_L")==0)|| (g_strcasecmp (key_stat[i-1],"Alt_l")==0))
	gtk_entry_set_text (GTK_ENTRY (entry3),_("None"));
}


void prefswin_dialog_create(Snap *snap) 
{
	GtkWidget *vbox;
	GtkWidget *frame;
	GtkWidget *alignment;
	GtkWidget *hbox;
	GtkWidget *image;
	GtkWidget *vbox2;
	GtkWidget *label2;
	GtkWidget *label3;
	GtkWidget *label1;
	GtkWidget *frame2;
	GtkWidget *alignment2;
	GtkWidget *hbox2;
	GtkWidget *frame5;
	GtkWidget *alignment5;
	GtkObject *spinbutton1_adj;
	GtkWidget *label7;
	GtkWidget *label4;
	GtkWidget *frame3;
	GtkWidget *hbox3;
	GtkWidget *frame6;
	GtkWidget *alignment6;
	GtkObject *spinbutton2_adj;
	GtkWidget *label8;
	GtkWidget *label5;
	GtkWidget *frame4;
	GtkWidget *alignment4;
	GtkWidget *hbox4;
	GtkWidget *frame7;
	GtkWidget *alignment7;
	GtkObject *spinbutton3_adj;
	GtkWidget *label9;
	GtkWidget *label6;
	GtkWidget *hbuttonbox1;
	GtkWidget *button1;
	GtkWidget *button2;

	prefwin = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (prefwin), _("Preference window"));
	gtk_window_set_destroy_with_parent (GTK_WINDOW (prefwin), TRUE);
	
	gtk_signal_connect_object (GTK_OBJECT (prefwin), "destroy",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (prefwin));

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);
	gtk_container_add (GTK_CONTAINER (prefwin), vbox);
	
	frame = gtk_frame_new (NULL);
	gtk_widget_show (frame);
	gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
	gtk_frame_set_label_align (GTK_FRAME (frame), 0.5, 0.5);
	
	alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment);
	gtk_container_add (GTK_CONTAINER (frame), alignment);
	
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_container_add (GTK_CONTAINER (alignment), hbox);
	
	image = gtk_image_new_from_stock ("gtk-dialog-info", GTK_ICON_SIZE_DIALOG);
	gtk_widget_show (image);
	gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 0);
	
	vbox2 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox2);
	gtk_box_pack_start (GTK_BOX (hbox), vbox2, TRUE, TRUE, 0);
	
	label2 = gtk_label_new (_("<span color=\"red\" size=\"large\">Snapshot All Window 0.1</span>"));
	gtk_widget_show (label2);
	gtk_box_pack_start (GTK_BOX (vbox2), label2, FALSE, FALSE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label2), TRUE);
	gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_RIGHT);
	
	label3 = gtk_label_new (_("<span color=\"#039523\" size=\"large\">Author: Eugene Morenko aka  (more) </span>\n<span color=\"#aa2343\" size=\"large\">mailto: more@irpin.com </span>\n<span color=\"#888723\" size=\"large\">Web:http://snapall.linux.kiev.ua/ </span>"));
	gtk_widget_show (label3);
	gtk_box_pack_start (GTK_BOX (vbox2), label3, FALSE, FALSE, 0);
	gtk_label_set_use_markup (GTK_LABEL (label3), TRUE);
	
	label1 = gtk_label_new (_("<b>About</b>"));
	gtk_widget_show (label1);
	gtk_frame_set_label_widget (GTK_FRAME (frame), label1);
	gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);
	gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_CENTER);
	
	frame2 = gtk_frame_new (NULL);
	gtk_widget_show (frame2);
	gtk_box_pack_start (GTK_BOX (vbox), frame2, TRUE, TRUE, 0);
	gtk_frame_set_label_align (GTK_FRAME (frame2), 0.5, 0.5);
	
	alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment2);
	gtk_container_add (GTK_CONTAINER (frame2), alignment2);
	
	hbox2 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox2);
	gtk_container_add (GTK_CONTAINER (alignment2), hbox2);
	
	entry1 = gtk_entry_new ();
	gtk_widget_show (entry1);
	gtk_box_pack_start (GTK_BOX (hbox2), entry1, TRUE, TRUE, 0);
	gtk_entry_set_text (GTK_ENTRY (entry1),snap->key->root_wind);
	
	gtk_signal_connect (GTK_OBJECT (entry1), "key_press_event",
				GTK_SIGNAL_FUNC (root_press),snap);
	
	gtk_signal_connect (GTK_OBJECT (entry1), "key_release_event",
				GTK_SIGNAL_FUNC (root_release),snap);
	
	frame5 = gtk_frame_new (NULL);
	gtk_widget_show (frame5);
	gtk_box_pack_start (GTK_BOX (hbox2), frame5, TRUE, TRUE, 0);
	gtk_frame_set_label_align (GTK_FRAME (frame5), 0.5, 0.5);
	gtk_frame_set_shadow_type (GTK_FRAME (frame5), GTK_SHADOW_NONE);
	
	alignment5 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment5);
	gtk_container_add (GTK_CONTAINER (frame5), alignment5);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment5), 0, 0, 12, 0);
	
	spinbutton1_adj = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
	spinbutton1 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton1_adj), 1, 0);
	
	gtk_widget_show (spinbutton1);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(spinbutton1),snap->key->t_root_wind);
	gtk_container_add (GTK_CONTAINER (alignment5), spinbutton1);
	
	label7 = gtk_label_new (_("Snapshot delay"));
	gtk_widget_show (label7);
	gtk_frame_set_label_widget (GTK_FRAME (frame5), label7);
	gtk_label_set_use_markup (GTK_LABEL (label7), TRUE);
	
	label4 = gtk_label_new (_("<b>Full Screen</b>"));
	gtk_widget_show (label4);
	gtk_frame_set_label_widget (GTK_FRAME (frame2), label4);
	gtk_label_set_use_markup (GTK_LABEL (label4), TRUE);
	
	frame3 = gtk_frame_new (NULL);
	gtk_widget_show (frame3);
	gtk_box_pack_start (GTK_BOX (vbox), frame3, TRUE, TRUE, 0);
	gtk_frame_set_label_align (GTK_FRAME (frame3), 0.5, 0.5);
	
	hbox3 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox3);
	gtk_container_add (GTK_CONTAINER (frame3), hbox3);
	
	entry2 = gtk_entry_new ();
	gtk_widget_show (entry2);
	gtk_box_pack_start (GTK_BOX (hbox3), entry2, TRUE, TRUE, 0);
	gtk_entry_set_text (GTK_ENTRY (entry2),snap->key->id_wind);
	
	gtk_signal_connect (GTK_OBJECT (entry2), "key_press_event",
				GTK_SIGNAL_FUNC (id_wind_press),snap);
	
	gtk_signal_connect (GTK_OBJECT (entry2), "key_release_event",
				GTK_SIGNAL_FUNC (id_wind_release),snap);
	
	frame6 = gtk_frame_new (NULL);
	gtk_widget_show (frame6);
	gtk_box_pack_start (GTK_BOX (hbox3), frame6, TRUE, TRUE, 0);
	gtk_frame_set_label_align (GTK_FRAME (frame6), 0.5, 0.5);
	gtk_frame_set_shadow_type (GTK_FRAME (frame6), GTK_SHADOW_NONE);
	
	alignment6 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment6);
	gtk_container_add (GTK_CONTAINER (frame6), alignment6);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment6), 0, 0, 12, 0);
	
	spinbutton2_adj = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
	spinbutton2 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton2_adj), 1, 0);
	gtk_widget_show (spinbutton2);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(spinbutton2),snap->key->t_id_wind);
	gtk_container_add (GTK_CONTAINER (alignment6), spinbutton2);
	
	label8 = gtk_label_new (_("Snapshot delay"));
	gtk_widget_show (label8);
	gtk_frame_set_label_widget (GTK_FRAME (frame6), label8);
	gtk_label_set_use_markup (GTK_LABEL (label8), TRUE);
	
	label5 = gtk_label_new (_("<b>Window Under Cursor</b>"));
	gtk_widget_show (label5);
	gtk_frame_set_label_widget (GTK_FRAME (frame3), label5);
	gtk_label_set_use_markup (GTK_LABEL (label5), TRUE);
	
	frame4 = gtk_frame_new (NULL);
	gtk_widget_show (frame4);
	gtk_box_pack_start (GTK_BOX (vbox), frame4, TRUE, TRUE, 0);
	gtk_frame_set_label_align (GTK_FRAME (frame4), 0.5, 0.5);
	
	alignment4 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment4);
	gtk_container_add (GTK_CONTAINER (frame4), alignment4);
	
	hbox4 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox4);
	gtk_container_add (GTK_CONTAINER (alignment4), hbox4);
	
	entry3 = gtk_entry_new ();
	gtk_widget_show (entry3);
	gtk_box_pack_start (GTK_BOX (hbox4), entry3, TRUE, TRUE, 0);
	gtk_entry_set_text (GTK_ENTRY (entry3),snap->key->x_y_wind);
	
	gtk_signal_connect (GTK_OBJECT (entry3), "key_press_event",
				GTK_SIGNAL_FUNC (x_y_wind_press),snap);
	
	gtk_signal_connect (GTK_OBJECT (entry3), "key_release_event",
				GTK_SIGNAL_FUNC (x_y_wind_release),snap);
	
	frame7 = gtk_frame_new (NULL);
	gtk_widget_show (frame7);
	gtk_box_pack_start (GTK_BOX (hbox4), frame7, TRUE, TRUE, 0);
	gtk_frame_set_label_align (GTK_FRAME (frame7), 0.5, 0.5);
	gtk_frame_set_shadow_type (GTK_FRAME (frame7), GTK_SHADOW_NONE);
	
	alignment7 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment7);
	gtk_container_add (GTK_CONTAINER (frame7), alignment7);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment7), 0, 0, 12, 0);
	
	spinbutton3_adj = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
	spinbutton3 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton3_adj), 1, 0);
	gtk_widget_show (spinbutton3);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON(spinbutton3),snap->key->t_x_y_wind);
	gtk_container_add (GTK_CONTAINER (alignment7), spinbutton3);
	
	label9 = gtk_label_new (_("Snapshot delay"));
	gtk_widget_show (label9);
	gtk_frame_set_label_widget (GTK_FRAME (frame7), label9);
	gtk_label_set_use_markup (GTK_LABEL (label9), TRUE);
	
	label6 = gtk_label_new (_("<b>Region</b>"));
	gtk_widget_show (label6);
	gtk_frame_set_label_widget (GTK_FRAME (frame4), label6);
	gtk_label_set_use_markup (GTK_LABEL (label6), TRUE);
	
	hbuttonbox1 = gtk_hbutton_box_new ();
	gtk_widget_show (hbuttonbox1);
	gtk_box_pack_start (GTK_BOX (vbox), hbuttonbox1, TRUE, TRUE, 0);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_END);
	gtk_box_set_spacing (GTK_BOX (hbuttonbox1), 20);
	
	button1 = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (button1);
	gtk_container_add (GTK_CONTAINER (hbuttonbox1), button1);
	GTK_WIDGET_SET_FLAGS (button1, GTK_CAN_DEFAULT);
	gtk_signal_connect_object (GTK_OBJECT (button1), "clicked",
				GTK_SIGNAL_FUNC (gtk_widget_destroy),
				GTK_OBJECT (prefwin));
	
	button2 = gtk_button_new_from_stock ("gtk-save");
	gtk_widget_show (button2);
	g_signal_connect ((gpointer) button2, "clicked",
                    G_CALLBACK (save_confif_to_file),
                    snap);
	gtk_container_add (GTK_CONTAINER (hbuttonbox1), button2);
	GTK_WIDGET_SET_FLAGS (button2, GTK_CAN_DEFAULT);
	
	
	gtk_widget_show_all(prefwin);
}

