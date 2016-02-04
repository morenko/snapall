/* Snapshot All Windows 
 * key.h
 *
 * Copyright (C) 2005 Eugene Morenko(More)
 * mailto: more@irpin.com
 * web: http://snapall.linux.kiev.ua/
 * icq:325697438
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
#ifndef __KEY_H_
#define __KEY_H_


void init_key (Snap *snap);
void grab_key (MScreen *mscreen, int key_code,int state);
void grab_key_real (int key_code,int state,GdkWindow *root);

void ungrab_key (MScreen *mscreen, int key_code);
void ungrab_key_real (int key_code, GdkWindow *root);
void do_action (Snap *snap,char *command);
GdkFilterReturn snap_filter_events (GdkXEvent *xevent, GdkEvent *event, gpointer data);

int DellModifier (unsigned int modifier);
int AddCapsModifier (unsigned int modifier);
int AddNumModifier (unsigned int modifier);
int AddCapsNumModifier (unsigned int modifier);
int ModifierToString (unsigned int modifier, char * str);
int StringToModifier_Key (char * str,unsigned int * modifier);

#endif /* __KEY_H_ */
