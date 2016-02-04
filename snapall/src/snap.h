/* Snapshot All Windows 
 * snap.h
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

#ifndef __SNAP_H_
#define __SNAP_H_


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

 /*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

#ifdef DEBUG
#define DEBUG_MSG g_print
#else /* not DEBUG */
#ifdef __GNUC__
#define DEBUG_MSG(args...)
 /**/
#else/* notdef __GNUC__ */
extern void g_none(gchar *first, ...);
#define DEBUG_MSG g_none
#endif /* __GNUC__ */
#endif /* DEBUG */

#include <assert.h>
#include "configfile.h"
#include "eggtrayicon.h"

typedef struct {
	/* Multihead stuff */
	GdkDisplay *display;
	GdkScreen *current_screen;
	GList *screens;
} MScreen;

typedef struct {
	gchar *root_wind;
	gchar *id_wind;
	gchar *x_y_wind;
	
	gint t_root_wind;
	gint t_id_wind;
	gint t_x_y_wind;
} Skey;

typedef struct {
	GtkWidget *snap_wind;
	GtkWidget *snap_img;
	GdkPixbuf *snap_pixbuf;
	MScreen *screen;
	ConfigFile *cfgfile;
	Skey *key;
	int function;
	int timeId;
	/* Tray icon */
	EggTrayIcon *tray_icon;
	GtkTooltips *tray_icon_tooltip;
	GtkWidget *popup_menu;
} Snap;



#endif /* __SNAP_H_ */

