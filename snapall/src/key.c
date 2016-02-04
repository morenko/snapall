/* Snapshot All Windows 
 * key.c
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

static char * modifier_string [] = {"Control", "Shift", "Alt","Win"};

int DellModifier (unsigned int modifier)
{
	int mod = 0;

	if (modifier & ControlMask )
		mod += ControlMask;
  
	if (modifier & ShiftMask)
		mod += ShiftMask;

	if (modifier & Mod1Mask)
		mod += Mod1Mask;

	if (modifier & Mod4Mask)
		mod += Mod4Mask;
	
	return mod;
}
int AddCapsModifier (unsigned int modifier)
{
	int mod = DellModifier(modifier);

	mod += LockMask;
	return mod;
}
int AddNumModifier (unsigned int modifier)
{
	int mod = DellModifier(modifier);

	mod += Mod2Mask;
	return mod;
}
int AddCapsNumModifier (unsigned int modifier)
{
	int mod = DellModifier(modifier);

	mod += LockMask;
	mod += Mod2Mask;
	return mod;
}

int ModifierToString (unsigned int modifier, char * str)
{
	str[0] = '\0';
	int n=0;
	if (modifier & ControlMask )
	{
		if (str[0])
		strcat (str, "+");
		strcat (str, modifier_string[0]);
		n++;
	}
  
	if (modifier & ShiftMask)
	{
		if (str[0])
		strcat (str, "+");
		strcat (str, modifier_string[1]);
		n++;
	}

	if (modifier & Mod1Mask)
	{
		if (str[0])
		strcat (str, "+");
		strcat (str, modifier_string[2]);
		n++;
	}
	if (modifier & Mod4Mask)
	{
		if (str[0])
		strcat (str, "+");
		strcat (str, modifier_string[3]);
		n++;
	}
     return n;
}
int StringToModifier_Key (char * str,unsigned int * modifier)
{
	int key=0;
	gchar *temp;
 
	temp=strtok(str,"+");
	*modifier=0;
	
	while(temp!=NULL)
	{
		if ((strcasecmp (temp, "control") == 0)||(strcasecmp (temp, "ctrl") == 0))
		  *modifier += ControlMask;
		else if (strcasecmp (temp, "shift") == 0)
		  *modifier += ShiftMask;
		else if (strcasecmp (temp, "mod1") == 0 || strcasecmp (temp, "alt") == 0)
		  *modifier += Mod1Mask;
		else if (strcasecmp (temp, "mod4") == 0 || strcasecmp (temp, "win") == 0)
		  *modifier += Mod4Mask;
		else
		{
			if(atoi(temp)!=0)
				 key=atoi(temp);
			else
			{
				key =XKeysymToKeycode(GDK_DISPLAY(),XStringToKeysym (temp));
				if (key == 0)
					break;
				temp=strtok(NULL,"+");
				    continue;
			}
		}
		temp=strtok(NULL,"+");
	}
	g_free(temp);
	
	return key;
}


void ungrab_key_real (int key_code, GdkWindow *root)
{
	gdk_error_trap_push ();
	XUngrabKey (GDK_DISPLAY (), key_code, AnyModifier,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())));
	XUngrabKey (GDK_DISPLAY (), AnyKey, AnyModifier, (GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())));


	gdk_flush ();
	if (gdk_error_trap_pop ()) {
		char *error;

		error = g_strdup_printf
			(_("There was an error removing access to the "
			   "multimedia keys.\nKey %d couldn't be unbound."),
			 key_code);
		show_error (error);
		printf("EROR!:%s",error);
		g_free (error);
		exit (1);
	}
}

void ungrab_key (MScreen *mscreen, int key_code)
{
	GList *l;

	for (l = mscreen->screens; l != NULL; l = l->next)
	{
		GdkScreen *screen;

		screen = (GdkScreen *) l->data;
		ungrab_key_real (key_code, gdk_screen_get_root_window (screen));
	}
}


void grab_key_real (int key_code,int state,GdkWindow *root)
{
	gdk_error_trap_push ();
	DEBUG_MSG("grab_key_real:%d - state:%d\n",key_code,state);

	if(state>0)
	{
		XGrabKey (GDK_DISPLAY (), key_code,
			state,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), False,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			DellModifier(state),
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), False,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			AddCapsModifier(state),
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), False,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			AddNumModifier(state),
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), False,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			AddCapsNumModifier(state),
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), False,
			GrabModeAsync, GrabModeAsync);
	}
	else
	{
		XGrabKey (GDK_DISPLAY (), key_code,
			0,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			Mod2Mask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			Mod5Mask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			LockMask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			Mod2Mask | LockMask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			Mod5Mask | LockMask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			Mod2Mask | Mod5Mask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
		XGrabKey (GDK_DISPLAY (), key_code,
			Mod2Mask | Mod5Mask | LockMask,
			(GDK_WINDOW_XID (root) ? GDK_WINDOW_XID (root) : DefaultRootWindow (GDK_DISPLAY())), True,
			GrabModeAsync, GrabModeAsync);
	}

	gdk_flush ();
	if (gdk_error_trap_pop ())
	{
		char *error;
		char str[1000];
		char *key="";

		ModifierToString (state, str);
				key=g_strdup_printf ("%s%s%s",str,str[0] ? "+" : "",
				(XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (), key_code, 0)) != NULL)
				? XKeysymToString (XKeycodeToKeysym (GDK_DISPLAY (),key_code, 0))
				: g_strdup_printf ("%d",key_code));
		
		error = g_strdup_printf
			(_("It seems that another application already has"
			   " access to the multimedia keys.\n"
			   "Key %s couldn't be bound.\n"
			   "Is another daemon already running ?\n"),
			 key);
		show_error(error);
		printf("EROR!:%s",error);
		g_free (error);
	}
}

void grab_key (MScreen *mscreen, int key_code,int state)
{
	GList *l;

	
	
	for (l = mscreen->screens; l != NULL; l = l->next)
	{
		GdkScreen *screen;

		screen = (GdkScreen *) l->data;
		grab_key_real (key_code,state, gdk_screen_get_root_window (screen));
	}
}


void init_key (Snap *snap)
{
	int i;
	int key=0;
	int mod=0;
	
	snap->screen=g_malloc0(sizeof (MScreen));
	DEBUG_MSG("Start init_key\n");

	snap->screen->display = gdk_display_get_default ();
	snap->screen->screens = NULL;

	if (gdk_display_get_n_screens (snap->screen->display) == 1)
	{
		snap->screen->screens = g_list_append (snap->screen->screens,
				gdk_screen_get_default ());
	} else 
	{
		for (i = 0; i < gdk_display_get_n_screens (snap->screen->display); i++)
		{
			GdkScreen *screen;

			screen = gdk_display_get_screen (snap->screen->display, i);

			if (screen != NULL)
				snap->screen->screens = g_list_append (snap->screen->screens,screen);
		}
	}


	if (snap->key->root_wind!= NULL)
	{
		key=StringToModifier_Key(g_strdup(snap->key->root_wind),&mod);
		grab_key (snap->screen,key,mod);
	}
	if (snap->key->id_wind!= NULL)
	{
		key=StringToModifier_Key(g_strdup(snap->key->id_wind),&mod);
		grab_key (snap->screen,key,mod);
	}
	if (snap->key->x_y_wind!= NULL)
	{
		key=StringToModifier_Key(g_strdup(snap->key->x_y_wind),&mod);
		grab_key (snap->screen,key,mod);
	}
	
	return;
}

void do_action (Snap *snap,char *command)
{
	DEBUG_MSG("do_action, type is: %s\n", type);
	
	if(g_strcasecmp (command,"root_wind")==0)
	{
		//printf("root_wind\n");
		grab_root_wind(snap);
		return ;
	}
	if(g_strcasecmp (command,"id_wind")==0)
	{
		//printf("id_wind\n");
		grab_id_wind(snap);
		return ;
	}
	if(g_strcasecmp (command,"x_y_wind")==0)
	{
		//printf("x_y_wind\n");
		grab_x_y_wind(snap);
		//xConvert_text_case(sven);
		return ;
	}
	else
	{
		printf("Bad config line\n");
	}
}

GdkFilterReturn snap_filter_events (GdkXEvent *xevent, GdkEvent *event, gpointer data)
{
	Snap *snap = (Snap *) data;
	XEvent *xev = (XEvent *) xevent;
	XAnyEvent *xanyev = (XAnyEvent *) xevent;
	XKeyEvent *key = (XKeyEvent *) xevent;
	GdkScreen *event_screen = NULL;
	GList *l;

	DEBUG_MSG("snap_filter_events\n");


	
	/* Look for which screen we're receiving events */
	for (l = snap->screen->screens; (l != NULL) && (event_screen == NULL);
			l = l->next)
	{
		GdkWindow *window;
		GdkScreen *screen;

		screen = (GdkScreen *) l->data;
		window = gdk_screen_get_root_window (screen);

		if (GDK_WINDOW_XID (window) == xanyev->window)
		{
			event_screen = screen;
			break;
		}
	}

	/*printf("Key press !\n");
	printf("e.xkey.keycode=%d\n", xev->xkey.keycode);
	printf("e.xkey.state=%d\n", xev->xkey.state);*/


	if (xev->type == KeyPress)
	{
		int key_code=0;
		int key_mod=0;
		
		key_code=StringToModifier_Key(g_strdup(snap->key->root_wind),&key_mod);
		
		if (key_code == key->keycode && (key_mod == key->state || key_mod == DellModifier(key->state) || 	 	key_mod == AddCapsModifier(key->state) || key_mod == AddCapsNumModifier(key->state) || key_code == AddNumModifier(key->state)))
		{ 
			snap->screen->current_screen = event_screen;
			
				do_action (snap,"root_wind");
				return GDK_FILTER_REMOVE;
		}
		
		key_code=StringToModifier_Key(g_strdup(snap->key->id_wind),&key_mod);
		if (key_code == key->keycode && (key_mod == key->state || key_mod == DellModifier(key->state) || 	 	key_mod == AddCapsModifier(key->state) || key_mod == AddCapsNumModifier(key->state) || key_code == AddNumModifier(key->state)))
		{
			snap->screen->current_screen = event_screen;
			
				do_action (snap,"id_wind");
				return GDK_FILTER_REMOVE;
		}
		
		key_code=StringToModifier_Key(g_strdup(snap->key->x_y_wind),&key_mod);
		if (key_code == key->keycode && (key_mod == key->state || key_mod == DellModifier(key->state) || 	 	key_mod == AddCapsModifier(key->state) || key_mod == AddCapsNumModifier(key->state) || key_code == AddNumModifier(key->state)))
		{
			snap->screen->current_screen = event_screen;
			
				do_action (snap,"x_y_wind");
				return GDK_FILTER_REMOVE;
		}

	}
	
	return GDK_FILTER_CONTINUE;
}


