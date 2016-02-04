/* Snapshot All Windows 
 * utils.c
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
#include <math.h>

#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Xmu/WinUtil.h>
#include <X11/Xatom.h>

#include "snap.h"
#include "utils.h"
#include "eggtrayicon.h"



/*Start Greate Image*/
#include "../images/snap_icons.h"

typedef struct {
	const guint id;
	const guint8 *data;
} Tpixmap;

static Tpixmap tp[] = {
	{1, snap}
};
/* This is an internally used function to create pixmaps. */
GdkPixbuf *create_pixbuf (gint type, int width, int height)
{
	gint i;
	GdkPixbuf *pixbuf;

		for (i=0;i<(sizeof(tp)/sizeof(Tpixmap));i++) {
			if (tp[i].id == type) {
				break;
			}
		}
		if (i >= sizeof(tp)/sizeof(Tpixmap)) {
			DEBUG_MSG(_("new_pixmap, requested unknown type %d\n"), type);
			i = sizeof(tp)/sizeof(Tpixmap)-1;
		}
		pixbuf = gdk_pixbuf_new_from_inline(-1,tp[i].data,FALSE,NULL);
		    g_assert (pixbuf);

    if ((width > 0 && height > 0)
	|| (gdk_pixbuf_get_width (pixbuf) != width
	    && gdk_pixbuf_get_height (pixbuf) != height))
    {
	GdkPixbuf *scaled;

	scaled =
	    gdk_pixbuf_scale_simple (pixbuf,
				     width >
				     0 ? width : gdk_pixbuf_get_width (pixbuf),
				     height >
				     0 ? height :
				     gdk_pixbuf_get_height (pixbuf),
				     GDK_INTERP_BILINEAR);
	pixbuf=scaled;
    }
	return pixbuf;
}

GtkWidget *new_pixmap_mod(gint type, int width, int height) {
	GtkWidget *wid;
	gint i;
	GdkPixbuf *pixbuf;

		for (i=0;i<(sizeof(tp)/sizeof(Tpixmap));i++) {
			if (tp[i].id == type) {
				break;
			}
		}
		if (i >= sizeof(tp)/sizeof(Tpixmap)) {
			DEBUG_MSG(_("new_pixmap, requested unknown type %d\n"), type);
			i = sizeof(tp)/sizeof(Tpixmap)-1;
		}
		pixbuf = gdk_pixbuf_new_from_inline(-1,tp[i].data,FALSE,NULL);
		    g_assert (pixbuf);

    if ((width > 0 && height > 0)
	|| (gdk_pixbuf_get_width (pixbuf) != width
	    && gdk_pixbuf_get_height (pixbuf) != height))
    {
	GdkPixbuf *scaled;

	scaled =
	    gdk_pixbuf_scale_simple (pixbuf,
				     width >
				     0 ? width : gdk_pixbuf_get_width (pixbuf),
				     height >
				     0 ? height :
				     gdk_pixbuf_get_height (pixbuf),
				     GDK_INTERP_BILINEAR);
	pixbuf=scaled;
    }
	wid = gtk_image_new_from_pixbuf(pixbuf);
	g_object_unref(pixbuf);
	return wid;
}
/*END Greate Image*/


GtkWidget* xfce_create_mixed_button (const gchar *stock, const gchar *text);
gboolean xfce_confirm (const gchar *text, const gchar *stock_id,const gchar *action);

/* compat stub */
GtkWidget*
mixed_button_new (const char *stock, const char *text)
{
    return xfce_create_mixed_button (stock, text);
}

/**
 * xfce_create_mixed_button:
 * @stock: a stock item name.
 * @text: a text to display.
 *
 * Creates a button with both @stock icon and @text.
 *
 * Return value: the newly created mixed button widget.
 **/
GtkWidget*
xfce_create_mixed_button (const gchar *stock, const gchar *text)
{
    GtkWidget *button, *align, *image, *hbox, *label;

    button = gtk_button_new ();
    label = gtk_label_new_with_mnemonic (text);
    gtk_label_set_mnemonic_widget (GTK_LABEL (label), button);

    image = gtk_image_new_from_stock (stock, GTK_ICON_SIZE_BUTTON);
    hbox = gtk_hbox_new (FALSE, 2);

    align = gtk_alignment_new (0.5, 0.5, 0.0, 0.0);

    gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 0);
    gtk_box_pack_end (GTK_BOX (hbox), label, FALSE, FALSE, 0);

    gtk_container_add (GTK_CONTAINER (button), align);
    gtk_container_add (GTK_CONTAINER (align), hbox);
    gtk_widget_show_all (align);

    return button;
}

static void
message_dialog (GtkMessageType type, const gchar * message)
{
	GtkWidget *dlg;
	
	dlg = gtk_message_dialog_new (NULL,
				  GTK_DIALOG_MODAL,
				  type, GTK_BUTTONS_CLOSE, message);
	gtk_label_set_use_markup (GTK_LABEL (GTK_MESSAGE_DIALOG (dlg)->label), TRUE);
	gtk_window_set_position (GTK_WINDOW (dlg), GTK_WIN_POS_CENTER);
	gtk_dialog_run (GTK_DIALOG (dlg));
	gtk_widget_destroy (dlg);
}

/* wrappers around gtk_message_dialog (OBSOLETE) */
void show_info (const char *text)
{
    message_dialog (GTK_MESSAGE_INFO, text);
}

void show_warning (const char *text)
{
    message_dialog (GTK_MESSAGE_WARNING, text);
}

void show_error (const char *text)
{
    message_dialog (GTK_MESSAGE_ERROR, text);
}

/* compat stub */
gboolean confirm (const char *text, const char *stock, const char *action)
{
    return xfce_confirm (text, stock, action);
}

/**
 * xfce_confirm:
 * @text:     a question text
 * @stock_id: a stock item name
 * @action:   if non-NULL, this text is used on the confirm button together
 *            with the @stock icon.
 *
 * Runs a modal confirmation dialog, that has a 'cancel' and a 'confirm'
 * button. The 'confirm' button text can be set by @action if given.
 *
 * If @stock_id is equal to GTK_STOCK_YES, the 'cancel' button becomes a 'no' button.
 *
 * Return value: TRUE if the user confirms, else FALSE.
 **/
gboolean
xfce_confirm (const gchar *text,
              const gchar *stock_id,
              const gchar *action)
{
    GtkWidget *dialog, *button;
    int response = GTK_RESPONSE_NONE;

    dialog = gtk_message_dialog_new (NULL,
                                     GTK_DIALOG_MODAL,
                                     GTK_MESSAGE_QUESTION, GTK_BUTTONS_NONE,
                                     text);
	gtk_label_set_use_markup (GTK_LABEL (GTK_MESSAGE_DIALOG (dialog)->label), TRUE);
    if (strcmp (stock_id, GTK_STOCK_YES) == 0)
        button = gtk_button_new_from_stock (GTK_STOCK_NO);
    else
        button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);

    gtk_widget_show_all (button);
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog), button,
                                  GTK_RESPONSE_NO);

    if (action)
        button = mixed_button_new (stock_id, action);
    else
        button = gtk_button_new_from_stock (stock_id);
    gtk_widget_show_all (button);
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog), button,
                                  GTK_RESPONSE_YES);

   // gtk_dialog_set_has_separator (GTK_DIALOG (dialog), FALSE);
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);

    response = gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_hide (dialog);
    gtk_widget_destroy (dialog);

    if (response == GTK_RESPONSE_YES)
        return TRUE;
    else
        return FALSE;
}


#define DIR_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)	/* same as 0755 */
void file_check_directory(void) 
{
	gchar *rcdir = g_strconcat(g_get_home_dir(), "/.snap", NULL);
	
	if (!g_file_test(rcdir, G_FILE_TEST_IS_DIR))
		mkdir(rcdir, DIR_MODE);
	else
		DEBUG_MSG("HOME Sven DIR OK!\n");
	
	g_free(rcdir);
}


void snap_read_config(Snap *snap)
{
	DEBUG_MSG("read_config\n");
	
	snap->key=g_malloc0(sizeof (Skey));

	g_free (snap->key->root_wind);
	g_free (snap->key->id_wind);
	g_free (snap->key->x_y_wind);
	snap->key->root_wind = NULL;
	snap->key->id_wind = NULL;
	snap->key->x_y_wind = NULL;

	snap->key->t_root_wind = 0;
	snap->key->t_id_wind = 0;
	snap->key->t_x_y_wind = 0;
	

	

	DEBUG_MSG ("reading configuration data\n");
	snap->cfgfile = snap_cfg_open_file(g_strconcat(g_get_home_dir(),"/.snap/config", NULL));
	if (!snap->cfgfile)
		snap->cfgfile = snap_cfg_new();
	
	if (snap->cfgfile!= NULL)
	{
		snap_cfg_read_string (snap->cfgfile, "snap", "root_wind", &snap->key->root_wind);
		snap_cfg_read_int (snap->cfgfile, "snap", "t_root_wind", &snap->key->t_root_wind);
		
		snap_cfg_read_string (snap->cfgfile, "snap", "id_wind", &snap->key->id_wind);
		snap_cfg_read_int (snap->cfgfile, "snap", "t_id_wind", &snap->key->t_id_wind);
		
		snap_cfg_read_string (snap->cfgfile, "snap", "x_y_wind", &snap->key->x_y_wind);
		snap_cfg_read_int (snap->cfgfile, "snap", "t_x_y_wind", &snap->key->t_x_y_wind);
	}

	DEBUG_MSG ("done\n");
}


static gboolean tray_icon_release (GtkWidget *widget, GdkEventButton *event,Snap *snap)
{	

	//printf("lick\n");
	if (event->button ==3)
	{
		gtk_menu_popdown (GTK_MENU (snap->popup_menu));
		return FALSE;
	}

	return TRUE;
}
static gboolean tray_icon_press (GtkWidget *widget, GdkEventButton *event, Snap *snap)
{
	if (event->button == 1 && event->type == GDK_2BUTTON_PRESS)
	{	/* double click  */
		grab_root_wind (snap);
		return 1;
	}
	if (event->button == 3)
	{
		gtk_menu_popup (GTK_MENU (snap->popup_menu), NULL, NULL, NULL,
				NULL, event->button, event->time);
	
		return 1;
	}
	
	return 0;
}

void menu_prefswin_create(GtkMenuItem     *menuitem,Snap *snap)
{
	if(snap==NULL)
		return;
	
	prefswin_dialog_create (snap);
}

void menu_root(GtkMenuItem     *menuitem,Snap *snap)
{
	if(snap==NULL)
		return;
	
	grab_root_wind (snap);
}

void menu_id_wind(GtkMenuItem     *menuitem,Snap *snap)
{
	if(snap==NULL)
		return;
	
	grab_id_wind (snap);
}

void menu_x_y_wind(GtkMenuItem     *menuitem,Snap *snap)
{
	if(snap==NULL)
		return;
	
	grab_x_y_wind (snap);
}

void snap_exit (GtkMenuItem *menuitem,Snap *snap)
{
	int key=0;
	int mod=0;

	key=StringToModifier_Key(g_strdup(snap->key->root_wind),&mod);
	ungrab_key (snap->screen,key);
	
	key=StringToModifier_Key(g_strdup(snap->key->id_wind),&mod);
	ungrab_key (snap->screen,key);
	
	key=StringToModifier_Key(g_strdup(snap->key->x_y_wind),&mod);
	ungrab_key (snap->screen,key);
	
	gtk_main_quit();
	exit (1);
}

GtkWidget* create_menu_icon (Snap *snap)
{
	GtkWidget *menu;
	GtkWidget *menuitem,*menuaddsub,*menusub;
	GtkWidget *image;
	
	menu = gtk_menu_new ();
	
	menuitem = gtk_image_menu_item_new_from_stock ("gtk-preferences", NULL);
	gtk_widget_show_all (menuitem);
	gtk_container_add (GTK_CONTAINER (menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(menu_prefswin_create),snap);

	menuitem = gtk_separator_menu_item_new ();
	gtk_widget_show_all (menuitem);
	gtk_container_add (GTK_CONTAINER (menu), menuitem);
	gtk_widget_set_sensitive (menuitem, FALSE);
	
	menuitem = gtk_image_menu_item_new_from_stock (_("Full Screen"), NULL);
	gtk_widget_show_all (menuitem);
	gtk_container_add (GTK_CONTAINER (menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(menu_root),snap);
	
	menuitem = gtk_image_menu_item_new_from_stock (_("Window Under Cursor"), NULL);
	gtk_widget_show_all (menuitem);
	gtk_container_add (GTK_CONTAINER (menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(menu_id_wind),snap);
	
	menuitem = gtk_image_menu_item_new_from_stock (_("Region"), NULL);
	gtk_widget_show_all (menuitem);
	gtk_container_add (GTK_CONTAINER (menu), menuitem);
	g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(menu_x_y_wind),snap);
	
	menuitem = gtk_separator_menu_item_new ();
	gtk_widget_show_all (menuitem);
	gtk_container_add (GTK_CONTAINER (menu), menuitem);
	gtk_widget_set_sensitive (menuitem, FALSE);
	
	menuitem = gtk_image_menu_item_new_from_stock ("gtk-quit", NULL);
	gtk_widget_show_all (menuitem);
	gtk_container_add (GTK_CONTAINER (menu), menuitem);
	g_signal_connect ((gpointer) menuitem, "activate",G_CALLBACK (snap_exit),snap);
	
 	return menu;
}

static gboolean tray_destroyed (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	init_tray((Snap *) user_data);
	//printf("Destroy tray icon\n");
	return TRUE;
}

void init_tray (Snap *snap)
{
	GtkWidget *evbox;
      
	 /* Popup menu */
	snap->popup_menu= create_menu_icon (snap);
	
	if(snap->tray_icon==NULL)
	{
		/* Event box */
		evbox = gtk_event_box_new ();
		gtk_event_box_set_visible_window(GTK_EVENT_BOX(evbox),0);
		
		DEBUG_MSG("Start tray icon\n");
		snap->tray_icon = egg_tray_icon_new (_("Snapshot All Windows "));
			
		snap->tray_icon_tooltip = gtk_tooltips_new ();
		gtk_tooltips_set_tip (snap->tray_icon_tooltip,GTK_WIDGET (snap->tray_icon),_("Snapshot All Windows   active"),NULL);
			
			
		g_signal_connect (G_OBJECT (snap->tray_icon), "button_press_event",G_CALLBACK (tray_icon_press),snap);
		g_signal_connect (G_OBJECT (snap->tray_icon), "button_release_event",G_CALLBACK (tray_icon_release),snap);
		g_signal_connect(G_OBJECT(snap->tray_icon), "destroy",G_CALLBACK (tray_destroyed), (gpointer) snap);
			
		gtk_container_add(GTK_CONTAINER(evbox),new_pixmap_mod(1,24,24));
		gtk_container_add (GTK_CONTAINER (snap->tray_icon), evbox);
		gtk_widget_show_all (GTK_WIDGET (snap->tray_icon));
	}
}

void restart_tray (Snap *snap)
{
	init_tray (snap);
}


/*************************************************/

GdkPixbuf * grab_root_pixbuf (void)
{   
	return gdk_pixbuf_get_from_drawable (NULL,gdk_get_default_root_window(), NULL,
						 0,0,0,0,gdk_screen_width(),gdk_screen_height());
}

Window snap_window_select(void)
{
	Window w;
	XEvent event;
	int res;
	const int EVENTMASK =ButtonPressMask|ButtonReleaseMask;
	Cursor cursor_grab;
	
	

	w = GDK_ROOT_WINDOW();

	cursor_grab = XCreateFontCursor(GDK_DISPLAY(), XC_cross);
	

	res = XGrabPointer(GDK_DISPLAY(), GDK_ROOT_WINDOW(), False, EVENTMASK, GrabModeSync,
					  GrabModeAsync, GDK_ROOT_WINDOW(), cursor_grab, CurrentTime);
	if(res != GrabSuccess)
	{
		int revert;
		Window focused;
		
		fprintf (stderr, "unable to grab pointer, error %d\n", res);
		XGetInputFocus(GDK_DISPLAY(), &focused, &revert);
		return focused;
	}

	while (1)
	{
		/* allow sending events */
		XAllowEvents(GDK_DISPLAY(), SyncPointer, CurrentTime);
		XWindowEvent(GDK_DISPLAY(), GDK_ROOT_WINDOW(), EVENTMASK, &event);

		if (event.type == ButtonPress)
		{
		
			w = event.xbutton.subwindow;
			break;
		}
	}

	XUngrabPointer(GDK_DISPLAY(), CurrentTime);
	XFreeCursor(GDK_DISPLAY(), cursor_grab);
	return (w);
}

GdkPixbuf * grab_id_wind_pixbuf (void)
{   
	Window window_grab;
	XWindowAttributes attr_wg;
	

	 window_grab=snap_window_select();
	


	XGetWindowAttributes(GDK_DISPLAY(), window_grab, &attr_wg);
	
	{
		int x,y,w,h;

		x = y = 0;
		{
			Window wunused;
	
			if (!XTranslateCoordinates(GDK_DISPLAY(), window_grab, attr_wg.root,x, y, &x, &y, &wunused))
				return;
				
		/*	XTranslateCoordinates (GDK_DISPLAY(), window_grab, attr_wg.root, 
				-attr_wg.border_width,-attr_wg.border_width,
				&x, &y, &wunused);*/
		}
		//x=((attr_wg.border_width)+x);
		//y=((attr_wg.border_width)+y);
		
		return gdk_pixbuf_get_from_drawable (NULL,gdk_get_default_root_window(), NULL,x,y,0,0,attr_wg.width,attr_wg.height);
	}
}


GdkPixbuf * grab_x_y_wind_pixbuf (void)
{
	
	XEvent ev;
	int count = 0, done = 0;
	int rx = 0, ry = 0, rw = 0, rh = 0, btn_pressed = 0;
	int rect_x = 0, rect_y = 0, rect_w = 0, rect_h = 0;
	Cursor cursor;
	Window target = None;
	GC gc;
	XGCValues gcval;
	
	
	cursor = XCreateFontCursor(GDK_DISPLAY(), XC_pencil);
	
	gcval.foreground = XWhitePixel(GDK_DISPLAY(), 0);
	gcval.function = GXxor;
	gcval.background = XBlackPixel(GDK_DISPLAY(), 0);
	gcval.plane_mask = gcval.background ^ gcval.foreground;
	gcval.subwindow_mode = IncludeInferiors;
	
	gc =XCreateGC(GDK_DISPLAY(), GDK_ROOT_WINDOW(),GCFunction | GCForeground | GCBackground | GCSubwindowMode,&gcval);
	
	if ((XGrabPointer (GDK_DISPLAY(), GDK_ROOT_WINDOW(), False,
		ButtonMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync,
		GrabModeAsync, GDK_ROOT_WINDOW(), cursor, CurrentTime) != GrabSuccess))
		printf("couldn't grab pointer:\n");
		
	if ((XGrabKeyboard (GDK_DISPLAY(), GDK_ROOT_WINDOW(), False, GrabModeAsync, GrabModeAsync,
        CurrentTime) != GrabSuccess))
		printf("couldn't grab keyboard:");
	
	
	while (1) 
	{
		/* handle events here */
		while (!done && XPending(GDK_DISPLAY())) 
		{
			XNextEvent(GDK_DISPLAY(), &ev);
			switch (ev.type) 
			{
				case MotionNotify:
				if (btn_pressed) 
				{
					if (rect_w) 
					{
						/* re-draw the last rect to clear it */
						XDrawRectangle(GDK_DISPLAY(), GDK_ROOT_WINDOW(), gc, rect_x, rect_y, rect_w, rect_h);
					}
		 			else 
		 			{
						/* Change the cursor to show we're selecting a region */
						XChangeActivePointerGrab(GDK_DISPLAY(),
						ButtonMotionMask | ButtonReleaseMask,
						cursor, CurrentTime);
					}
					
					rect_x = rx;
					rect_y = ry;
					rect_w = ev.xmotion.x - rect_x;
					rect_h = ev.xmotion.y - rect_y;
	
					if (rect_w < 0) 
					{
						rect_x += rect_w;
						rect_w = 0 - rect_w;
					}
					if (rect_h < 0) 
					{
						rect_y += rect_h;
						rect_h = 0 - rect_h;
					}
					/* draw rectangle */
					XDrawRectangle(GDK_DISPLAY(), GDK_ROOT_WINDOW(), gc, rect_x, rect_y, rect_w, rect_h);
					XFlush(GDK_DISPLAY());
				}
				break;
				case ButtonPress:
					btn_pressed = 1;
					rx = ev.xbutton.x;
					ry = ev.xbutton.y;
				break;
				case ButtonRelease:
				done = 1;
				break;
				case KeyPress:
				fprintf(stderr, "Key was pressed, aborting shot\n");
				done = 2;
				break;
				case KeyRelease:
				/* ignore */
				break;
				default:
				break;
			}
		}
	if (done)
	break;

	}

	if (rect_w) 
	{
	XDrawRectangle(GDK_DISPLAY(), GDK_ROOT_WINDOW(), gc, rect_x, rect_y, rect_w, rect_h);
	XFlush(GDK_DISPLAY());
	}
	
	XUngrabPointer(GDK_DISPLAY(), CurrentTime);
	XUngrabKeyboard(GDK_DISPLAY(), CurrentTime);
	XFreeCursor(GDK_DISPLAY(), cursor);
	XFreeGC(GDK_DISPLAY(), gc);
	XSync(GDK_DISPLAY(), True);
	
	if(done==1)
		return gdk_pixbuf_get_from_drawable (NULL,gdk_get_default_root_window(), NULL,rect_x,rect_y,0,0,rect_w,rect_h);
	else
		return gdk_pixbuf_get_from_drawable (NULL,gdk_get_default_root_window(), NULL,
						 0,0,0,0,gdk_screen_width(),gdk_screen_height());
	
}


gboolean timer_snap(gpointer data)
{
	Snap *snap = (Snap *)data;
	
	if(snap->function==1)
		create_window(snap,grab_root_pixbuf());
	else if(snap->function==2)
		create_window(snap,grab_id_wind_pixbuf());
	else if(snap->function==3)
		create_window(snap,grab_x_y_wind_pixbuf());
	else
		create_window(snap,grab_root_pixbuf());
  
	return FALSE;
} 

void grab_x_y_wind(Snap *snap)
{
	if(snap->snap_wind)
		gtk_widget_hide (GTK_WIDGET(snap->snap_wind));
		
	snap->function=3;
	
	if (snap->timeId != 0)
            g_source_remove(snap->timeId);
	snap->timeId = g_timeout_add ((snap->key->t_x_y_wind*1000),timer_snap, snap);
		
	//create_window(snap,grab_x_y_wind_pixbuf());
}

void grab_id_wind(Snap *snap)
{
	if(snap->snap_wind)
		gtk_widget_hide (GTK_WIDGET(snap->snap_wind));
	
	snap->function=2;
	
	if (snap->timeId != 0)
            g_source_remove(snap->timeId);
	snap->timeId = g_timeout_add ((snap->key->t_id_wind*1000),timer_snap, snap);
	
	//create_window(snap,grab_id_wind_pixbuf());
}

void grab_root_wind(Snap *snap)
{
	if(snap->snap_wind)
		gtk_widget_hide (GTK_WIDGET(snap->snap_wind));
	
	snap->function=1;
	
	if (snap->timeId != 0)
            g_source_remove(snap->timeId);
	snap->timeId = g_timeout_add ((snap->key->t_root_wind*1000),timer_snap, snap);
		
	//create_window(snap,grab_root_pixbuf());
}

/* 'image_resize' will allow the user to enlarge */
/* or shrink the displayed image */
GdkPixbuf * image_resize (GdkPixbuf * pixbuf)
{
  gint new_gdk_width, new_gdk_height;
  gint curr_width, curr_height;
	
  curr_width = gdk_pixbuf_get_width (pixbuf);
  curr_height = gdk_pixbuf_get_height (pixbuf);


  if(curr_width>400 || curr_height>400)
  {
     new_gdk_height = ((float)curr_height / (float)curr_width) * (curr_width/4);
     new_gdk_width = ((float)curr_width / (float)curr_height) * (curr_height/4);
  }
  else
  {
     new_gdk_height = ((float)curr_height / (float)curr_width) * (curr_width/3);
     new_gdk_width = ((float)curr_width / (float)curr_height) * (curr_height/3);
  }
   return gdk_pixbuf_scale_simple (pixbuf,new_gdk_width ,new_gdk_height , GDK_INTERP_BILINEAR);
}

static	GtkWidget *spinbutton, *combobox;;
static void new_snapshot (GtkWidget *wid,Snap *snap)
{
	int i=0;

	if(snap==NULL)
		return;
	
	i=gtk_combo_box_get_active(GTK_COMBO_BOX(combobox));
		
	if(i==0)
	{
		snap->key->t_root_wind=gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinbutton));
		grab_root_wind(snap);
	}
	else if(i==1)
	{
		snap->key->t_id_wind=gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinbutton));
		grab_id_wind(snap);
	}
	else if(i==2)
	{
		snap->key->t_x_y_wind=gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinbutton));
		grab_x_y_wind(snap);
	}
	else
	{
		snap->key->t_root_wind=gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(spinbutton));
		grab_root_wind(snap);
	}
}

static void hide_win_snap (GtkWidget *wid,Snap *snap)
{

//printf("Hide Snap\n");

	if(snap==NULL)
		return;
	
	if(snap->snap_wind)
		gtk_widget_hide (GTK_WIDGET(snap->snap_wind));
	gtk_widget_destroy (snap->snap_wind);
	snap->snap_wind=NULL;
}

static GtkWidget *filedialog=NULL;

void save_pixbuf_to_file(GtkButton *button,Snap *snap)
{
	GError *err = NULL;
	
	gchar *filename=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (filedialog));
	if(filename==NULL)
		return;
		
	gchar **filter = g_strsplit(g_strdup_printf("%s",filename),".",2);
	if(filter[1]!=NULL)
	{	
		if(g_strcasecmp (filter[1],"png")==0)
		{
			if (snap->snap_pixbuf == NULL) 
			{
				printf ("PIXBUF NULL\n");
				return;
			}	

			if (!gdk_pixbuf_save (snap->snap_pixbuf,filename, "png", &err,                          "tEXt::Software", "sven-save",NULL)) 
			{
				show_error(err->message);
				printf ("%s", err->message);
				g_error_free (err);
			}
			
		//	printf("png\n");
		}
		else if(g_strcasecmp (filter[1],"jpeg")==0)
		{
			if (snap->snap_pixbuf == NULL) 
			{
				printf ("PIXBUF NULL\n");
				return;
			}	

			if (!gdk_pixbuf_save (snap->snap_pixbuf,filename, "jpeg",&err,"quality", "100",NULL)) 
			{
				show_error(err->message);
				printf ("%s", err->message);
				g_error_free (err);
			}
		//	printf("jpeg\n");
		}
		else if(g_strcasecmp (filter[1],"ico")==0)
		{
				if (snap->snap_pixbuf == NULL) 
			{
				printf ("PIXBUF NULL\n");
				return;
			}	

			if (!gdk_pixbuf_save (snap->snap_pixbuf,filename, "ico",&err,NULL)) 
			{
				show_error(err->message);
				printf ("%s", err->message);
				g_error_free (err);
			}
			else if (!gdk_pixbuf_save (snap->snap_pixbuf,filename, "png", &err,                          "tEXt::Software", "sven-save",NULL)) 
			{
				show_error(err->message);
				printf ("%s", err->message);
				g_error_free (err);
			}
		//	printf("ico\n");
		}
		else
		{
			if (snap->snap_pixbuf == NULL) 
			{
				printf ("PIXBUF NULL\n");
				return;
			}	

			if (!gdk_pixbuf_save (snap->snap_pixbuf,filename, "png", &err,                          "tEXt::Software", "sven-save",NULL)) 
			{
				show_error(err->message);
				printf ("%s", err->message);
				g_error_free (err);
			}
		}
		
	}
	else
	{
		if (snap->snap_pixbuf == NULL) 
		{
			printf ("PIXBUF NULL\n");
			return;
		}	
		
		if (!gdk_pixbuf_save (snap->snap_pixbuf,filename, "png", &err,                          "tEXt::Software", "sven-save",NULL)) 
		{
			show_error(err->message);
			printf ("%s", err->message);
			g_error_free (err);
		}
	}
	
	gtk_widget_destroy (filedialog);
	filedialog=NULL;
	//printf("File:%s\n",filename);
}

void snap_file_save_dialog (GtkWidget *wid,Snap *snap)
{
	
	GtkWidget *dialog_vbox;
	GtkWidget *dialog_action_area;
	GtkWidget *button_ch;
	GtkWidget *button_ok;
	
	filedialog = gtk_file_chooser_dialog_new (_("Save as..."), NULL, GTK_FILE_CHOOSER_ACTION_SAVE, NULL);
	gtk_window_set_type_hint (GTK_WINDOW (filedialog), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_signal_connect_object (GTK_OBJECT (filedialog), "destroy",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (filedialog));

	dialog_vbox = GTK_DIALOG (filedialog)->vbox;
	gtk_widget_show (dialog_vbox);
	
	dialog_action_area = GTK_DIALOG (filedialog)->action_area;
	gtk_widget_show (dialog_action_area);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area), GTK_BUTTONBOX_END);
	
	button_ch = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (button_ch);
	gtk_dialog_add_action_widget (GTK_DIALOG (filedialog), button_ch, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (button_ch, GTK_CAN_DEFAULT);
	
	button_ok = gtk_button_new_from_stock ("gtk-save");
	gtk_widget_show (button_ok);
	gtk_dialog_add_action_widget (GTK_DIALOG (filedialog), button_ok, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (button_ok, GTK_CAN_DEFAULT);
	
	gtk_signal_connect_object (GTK_OBJECT (button_ch), "clicked",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (filedialog));
	g_signal_connect ((gpointer) button_ok, "clicked",
                    G_CALLBACK (save_pixbuf_to_file),
                    snap);
  gtk_widget_show (filedialog);
}


void create_window (Snap *snap,GdkPixbuf *pixbuf)
{
	GtkWidget *vbox;
	GtkWidget *hseparator;
	GtkWidget *table;
	GtkWidget *label;
	GtkObject *spinbut_count;
	GtkWidget *but_new;
	GtkWidget *hbuttonbox;
	GtkWidget *but_quit;
	GtkWidget *but_pref;
	GtkWidget *but_save;

	snap->snap_pixbuf=gdk_pixbuf_copy(pixbuf);
	
	if(!snap->snap_wind)
	{
		snap->snap_wind = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title (GTK_WINDOW ( snap->snap_wind), _("Snapshot All Window"));
		g_signal_connect(snap->snap_wind, "destroy", G_CALLBACK(hide_win_snap),snap);
		
		vbox = gtk_vbox_new (FALSE, 0);
		gtk_widget_show (vbox);
		gtk_container_add (GTK_CONTAINER ( snap->snap_wind), vbox);
		
		if(pixbuf!=NULL)
		{
			snap->snap_img =gtk_image_new_from_pixbuf(image_resize(pixbuf));
			gtk_widget_show (snap->snap_img);
			gtk_box_pack_start (GTK_BOX (vbox), snap->snap_img, FALSE, FALSE, 10);
		}
		else
		{
			snap->snap_img = new_pixmap_mod(1,25,25);
			gtk_widget_show (snap->snap_img);
			gtk_box_pack_start (GTK_BOX (vbox), snap->snap_img, FALSE, FALSE, 10);
		}
		hseparator = gtk_hseparator_new ();
		gtk_widget_show (hseparator);
		gtk_box_pack_start (GTK_BOX (vbox), hseparator, FALSE, FALSE, 0);
		
		table = gtk_table_new (3, 2, FALSE);
		gtk_widget_show (table);
		gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, FALSE, 0);
		gtk_container_set_border_width (GTK_CONTAINER (table), 2);
		gtk_table_set_row_spacings (GTK_TABLE (table), 7);
		
		label = gtk_label_new (_("Capture mode:"));
		gtk_widget_show (label);
		gtk_table_attach (GTK_TABLE (table), label, 0, 1, 0, 1,
				(GtkAttachOptions) (GTK_FILL),
				(GtkAttachOptions) (0), 0, 0);
		gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
		
		label = gtk_label_new (_("Snapshot delay:"));
		gtk_widget_show (label);
		gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
				(GtkAttachOptions) (GTK_FILL),
				(GtkAttachOptions) (0), 0, 0);
		gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
		gtk_misc_set_padding (GTK_MISC (label), 15, 0);
		
		spinbut_count = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
		spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (spinbut_count), 1, 0);
		gtk_widget_show (spinbutton);
		gtk_table_attach (GTK_TABLE (table), spinbutton, 1, 2, 1, 2,
				(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
				(GtkAttachOptions) (0), 0, 0);
		
		but_new = gtk_button_new_with_mnemonic (_("New Snapshot"));
		gtk_widget_show (but_new);
		g_signal_connect(but_new, "clicked", G_CALLBACK(new_snapshot),snap);
		gtk_table_attach (GTK_TABLE (table), but_new, 1, 2, 2, 3,
				(GtkAttachOptions) (GTK_FILL),
				(GtkAttachOptions) (0), 0, 0);
		gtk_container_set_border_width (GTK_CONTAINER (but_new), 2);
		
		combobox = gtk_combo_box_new_text ();
		gtk_widget_show (combobox);
		gtk_table_attach (GTK_TABLE (table), combobox, 1, 2, 0, 1,
				(GtkAttachOptions) (GTK_FILL),
				(GtkAttachOptions) (GTK_FILL), 0, 0);
		gtk_combo_box_append_text (GTK_COMBO_BOX (combobox), _("Full Screen"));
		gtk_combo_box_append_text (GTK_COMBO_BOX (combobox), _("Window Under Cursor"));
		gtk_combo_box_append_text (GTK_COMBO_BOX (combobox), _("Region"));
		gtk_combo_box_set_active (GTK_COMBO_BOX (combobox),0);
		
		hseparator = gtk_hseparator_new ();
		gtk_widget_show (hseparator);
		gtk_box_pack_start (GTK_BOX (vbox), hseparator, FALSE, FALSE, 0);
		
		hbuttonbox = gtk_hbutton_box_new ();
		gtk_widget_show (hbuttonbox);
		gtk_box_pack_start (GTK_BOX (vbox), hbuttonbox, FALSE, FALSE, 0);
		
		but_quit = gtk_button_new_from_stock ("gtk-quit");
		gtk_widget_show (but_quit);
		g_signal_connect(but_quit, "clicked", G_CALLBACK(hide_win_snap),snap);
		gtk_container_add (GTK_CONTAINER (hbuttonbox), but_quit);
		GTK_WIDGET_SET_FLAGS (but_quit, GTK_CAN_DEFAULT);
		
		/*but_pref = gtk_button_new_from_stock ("gtk-preferences");
		gtk_widget_show (but_pref);
		gtk_container_add (GTK_CONTAINER (hbuttonbox), but_pref);
		GTK_WIDGET_SET_FLAGS (but_pref, GTK_CAN_DEFAULT);*/
		
		but_save = gtk_button_new_from_stock ("gtk-save-as");
		gtk_widget_show (but_save);
		g_signal_connect(but_save, "clicked", G_CALLBACK(snap_file_save_dialog),snap);
		gtk_container_add (GTK_CONTAINER (hbuttonbox), but_save);
		GTK_WIDGET_SET_FLAGS (but_save, GTK_CAN_DEFAULT);
		
		gtk_widget_show_all (GTK_WIDGET(snap->snap_wind));
	}
	else
	{
		if(pixbuf==NULL)
			return;
		//gtk_widget_show(GTK_WIDGET(snap->snap_wind));
		gtk_image_set_from_pixbuf(GTK_IMAGE(snap->snap_img),image_resize(pixbuf));
		gtk_window_resize (GTK_WINDOW (snap->snap_wind), 50,50);
		gtk_widget_show (GTK_WIDGET(snap->snap_wind));
	}
}

#define MAX_PROPERTY_VALUE_LEN 4096

static gchar *get_property (Display *disp, Window win, /*{{{*/
        Atom xa_prop_type, gchar *prop_name, unsigned long *size) {
    Atom xa_prop_name;
    Atom xa_ret_type;
    int ret_format;
    unsigned long ret_nitems;
    unsigned long ret_bytes_after;
    unsigned long tmp_size;
    unsigned char *ret_prop;
    gchar *ret;
    
    xa_prop_name = XInternAtom(disp, prop_name, False);
    
    /* MAX_PROPERTY_VALUE_LEN / 4 explanation (XGetWindowProperty manpage):
     *
     * long_length = Specifies the length in 32-bit multiples of the
     *               data to be retrieved.
     */
    if (XGetWindowProperty(disp, win, xa_prop_name, 0, MAX_PROPERTY_VALUE_LEN / 4, False,
            xa_prop_type, &xa_ret_type, &ret_format,     
            &ret_nitems, &ret_bytes_after, &ret_prop) != Success) {
        printf("Cannot get %s property.\n", prop_name);
        return NULL;
    }
  
    if (xa_ret_type != xa_prop_type) {
        printf("Invalid type of %s property.\n", prop_name);
        XFree(ret_prop);
        return NULL;
    }

    /* null terminate the result to make string handling easier */
    tmp_size = (ret_format / 8) * ret_nitems;
    ret = g_malloc(tmp_size + 1);
    memcpy(ret, ret_prop, tmp_size);
    ret[tmp_size] = '\0';

    if (size) {
        *size = tmp_size;
    }
    
    XFree(ret_prop);
    return ret;
}/*}}}*/

#define SELECTION_NAME "_SNAPALL_SELECTION"

static void selection_get_func (GtkClipboard *clipboard, GtkSelectionData *selection_data,
		guint info, gpointer user_data_or_owner)
{
}

static void selection_clear_func (GtkClipboard *clipboard, gpointer user_data_or_owner)
{
	return;
}

gboolean snap_get_lock (Snap *snap)
{
	gboolean result = FALSE;
	GtkClipboard *clipboard;
	Atom clipboard_atom = gdk_x11_get_xatom_by_name (SELECTION_NAME);
	static const GtkTargetEntry targets[] = 
	{
		{ SELECTION_NAME, 0, 0 }
	};

	XGrabServer (GDK_DISPLAY());

	if (XGetSelectionOwner (GDK_DISPLAY(), clipboard_atom) != None)
		goto out;

	clipboard = gtk_clipboard_get (gdk_atom_intern (SELECTION_NAME, FALSE));

	if (!gtk_clipboard_set_with_data  (clipboard, targets,
				G_N_ELEMENTS (targets),
				selection_get_func,
				selection_clear_func, NULL))
		goto out;

	result = TRUE;

out:
	XUngrabServer (GDK_DISPLAY());
	gdk_flush();

	return result;
}


int snap_kill_lock (Snap *snap)
{
	 unsigned long *pid;
	 Window wind;
	Atom clipboard_atom = gdk_x11_get_xatom_by_name (SELECTION_NAME);

	XGrabServer (GDK_DISPLAY());

	wind=XGetSelectionOwner (GDK_DISPLAY(), clipboard_atom);
	
	if (wind != None)
	{
		/* pid */
        	pid =  (unsigned long *)get_property(GDK_DISPLAY(),wind,XA_CARDINAL, "_NET_WM_PID", NULL);
	}

	if(pid>0)
	{
		gchar *output = NULL;
	
		g_spawn_command_line_sync(g_strdup_printf("kill %-6lu", pid ? *pid : 0),NULL,&output,NULL,NULL);
		if(g_strcasecmp(output,"")!=0)
			printf("Not Kill SnapAll children\n");
		else
			 printf("Kill SnapAll Pid: %-6lu \n", pid ? *pid : 0);
	}
	 
	XUngrabServer (GDK_DISPLAY());
	gdk_flush();
	
	snap_get_lock(snap);
	return 1;
}
