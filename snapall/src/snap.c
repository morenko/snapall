/* Snapshot All Windows 
 * snap.c
 *
 *Copyright (C) 2005 Eugene Morenko(More)
 *mailto: morenko@ukr.net
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

#include <gtk/gtk.h>
#include <glib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <X11/X.h>
#include <gdk/gdkx.h>

#include <gdk-pixbuf/gdk-pixbuf.h>

#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Xmu/WinUtil.h>

#include "snap.h"
#include "key.h"
#include "utils.h"
#include "prefswin.h"

gchar *id_wind;
	gchar *x_y_wind;

static void show_help (void)
{
	printf ("Snapshot All Windows  %s \n",VERSION);
	printf ("usage: snapall [options]\n");
	printf ("  where options are:\n");
	printf ("  -v, --version             Print version and exit\n");
	printf ("  -h, --help                This help!\n");
	printf ("  -c, --configure           Configure SnapAll\n");
	printf ("  -all, --allwind           Snapshot All(root) window\n");
	printf ("  -id, --idwind             Snapshot select window\n");
	printf ("  -xy, --xywind             Snapshot region  window\n");
}
static void get_options (int argc, char **argv,Snap *snap)
{
	int i;
	for (i = 1; i < argc; i++)
    	{
  		if (strcmp (argv[i], "-v") == 0 || strcmp (argv[i], "--version") == 0)
		{
			printf("Snapshot All Windows %s \n",VERSION);
			printf("Copyright 2005 by Eugene Morenko(More)\n");
			printf("mailto: more@irpin.com\n");
			printf("web: http://snapall.linux.kiev.ua/\n");
			printf("icq:325697438\n");
			exit (1);
		}
		else if (strcmp (argv[i], "-h") == 0 || strcmp (argv[i], "--help") == 0)
		{
			show_help ();
			exit (1);
		}
		else if (strcmp (argv[i], "-c") == 0 || strcmp (argv[i], "--configure") == 0)
		{
			printf("Configure programm parameters dialog window\n");
			if(snap==NULL)
			{
				gtk_main_quit();
				exit (1);
			}
		
			prefswin_dialog_create (snap);
		}
		else if (strcmp (argv[i], "-all") == 0 || strcmp (argv[i], "--allwind") == 0)
		{
			printf(" Snapshot All(root) window\n");
			if(snap==NULL)
			{
				gtk_main_quit();
				exit (1);
			}
		
			grab_root_wind (snap);
		}
		else if (strcmp (argv[i], "-id") == 0 || strcmp (argv[i], "--idwind") == 0)
		{
			printf("Snapshot select window\n");
			if(snap==NULL)
			{
				gtk_main_quit();
				exit (1);
			}
		
			grab_id_wind (snap);
		}
		else if (strcmp (argv[i], "-xy") == 0 || strcmp (argv[i], "--xywind") == 0)
		{
			printf("Snapshot region  window\n");
			if(snap==NULL)
			{
				gtk_main_quit();
				exit (1);
			}
		
			grab_x_y_wind (snap);
		}
		else
		{
			show_help();
				exit (1);
		}
	}
}


int main( int   argc,char *argv[] )
{
	Snap *snap;
	GList *l;

	#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
	#endif
	
	gtk_set_locale ();
	gtk_init (&argc, &argv);
	
	 snap = g_new0 (Snap, 1);
	 
	if (snap_get_lock (snap) == FALSE)
	{
		snap_kill_lock(snap);
	}
	
	/*Start*/
	file_check_directory();
	
	snap_read_config(snap);
	
	get_options (argc,argv,snap);
	
	init_key (snap);

	init_tray (snap);
	
	snap->screen->current_screen = gdk_screen_get_default ();

   	/* Start filtering the events */
	for (l = snap->screen->screens; l != NULL; l = l->next)
	{
		GdkScreen *screen;
		GdkWindow *window;

		screen = (GdkScreen *) l->data;
		window = gdk_screen_get_root_window (screen);
		gdk_window_add_filter (window,
				snap_filter_events,
				(gpointer) snap);
	}

	
	gtk_main ();
	return 0;
}


