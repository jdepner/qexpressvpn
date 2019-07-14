
/********************************************************************************************* 

    env_in_out.cpp

    Copyright (c) 2017, Jan C. Depner


    This file is part of qexpressvpn.

    qexpressvpn is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qexpressvpn is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qexpressvpn.  If not, see <http://www.gnu.org/licenses/>.

*********************************************************************************************/


#include "qexpressvpn.hpp"


double settings_version = 1.00;


/*!
  These functions store and retrieve the program settings (environment) from a .ini file.  The file will be called
  .qexpressvpn.ini and will be stored in your $HOME directory.  If you make a change to the way a variable is used
  and you want to force the defaults to be restored just change the settings_version to a newer number (I've been
  using the program version number from version.hpp - which you should be updating EVERY time you make a change to
  the program!).  You don't need to change the settings_version though unless you want to force the program to go
  back to the defaults (which can annoy your users).  So, the settings_version won't always match the program version.
*/

uint8_t envin (OPTIONS *options)
{
  double saved_version = 0.0;


  //  Get the INI file name

  QString ini_file = QString (getenv ("HOME")) + "/.qexpressvpn.ini";

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("qexpressvpn");


  saved_version = settings.value (QString ("settings version"), saved_version).toDouble ();


  //  If the settings version has changed we need to leave the values at the new defaults since they may have changed.

  if (settings_version != saved_version) return (false);


  options->preferred_protocol = settings.value (QString ("preferred protocol"), options->preferred_protocol).toInt ();
  options->auto_disconnect = settings.value (QString ("auto disconnect"), options->auto_disconnect).toBool ();
  options->stay_on_top = settings.value (QString ("stay on top"), options->stay_on_top).toBool ();
  options->auto_mini = settings.value (QString ("auto mini"), options->auto_mini).toBool ();
  options->auto_connect = settings.value (QString ("auto connect"), options->auto_connect).toBool ();
  options->sort_direction = settings.value (QString ("sort direction"), options->sort_direction).toInt ();

  QString defaultFont = options->font.toString ();
  QString fontString = settings.value (QString ("font"), defaultFont).toString ();
  options->font.fromString (fontString);

  int32_t red = settings.value (QString ("server color/red"), options->server_color.red ()).toInt ();
  int32_t green = settings.value (QString ("server color/green"), options->server_color.green ()).toInt ();
  int32_t blue = settings.value (QString ("server color/blue"), options->server_color.blue ()).toInt ();
  int32_t alpha = settings.value (QString ("server color/alpha"), options->server_color.alpha ()).toInt ();
  options->server_color.setRgb (red, green, blue, alpha);

  red = settings.value (QString ("connected color/red"), options->connected_color.red ()).toInt ();
  green = settings.value (QString ("connected color/green"), options->connected_color.green ()).toInt ();
  blue = settings.value (QString ("connected color/blue"), options->connected_color.blue ()).toInt ();
  alpha = settings.value (QString ("connected color/alpha"), options->connected_color.alpha ()).toInt ();
  options->connected_color.setRgb (red, green, blue, alpha);

  red = settings.value (QString ("disconnected color/red"), options->disconnected_color.red ()).toInt ();
  green = settings.value (QString ("disconnected color/green"), options->disconnected_color.green ()).toInt ();
  blue = settings.value (QString ("disconnected color/blue"), options->disconnected_color.blue ()).toInt ();
  alpha = settings.value (QString ("disconnected color/alpha"), options->disconnected_color.alpha ()).toInt ();
  options->disconnected_color.setRgb (red, green, blue, alpha);

  options->current_server.alias = settings.value (QString ("current alias"), options->current_server.alias).toString ();
  options->current_server.country = settings.value (QString ("current country"), options->current_server.country).toString ();
  options->current_server.country_alias = settings.value (QString ("current country alias"), options->current_server.country_alias).toString ();
  options->current_server.location = settings.value (QString ("current location"), options->current_server.location).toString ();
  options->current_server.recommended = settings.value (QString ("current recommended"), options->current_server.recommended).toInt ();


  options->window_width = settings.value (QString ("width"), options->window_width).toInt ();
  options->window_height = settings.value (QString ("height"), options->window_height).toInt ();
  options->window_x = settings.value (QString ("x position"), options->window_x).toInt ();
  options->window_y = settings.value (QString ("y position"), options->window_y).toInt ();

  options->status_window_width = settings.value (QString ("status width"), options->status_window_width).toInt ();
  options->status_window_height = settings.value (QString ("status height"), options->status_window_height).toInt ();
  options->status_window_x = settings.value (QString ("status x position"), options->status_window_x).toInt ();
  options->status_window_y = settings.value (QString ("status y position"), options->status_window_y).toInt ();


  settings.endGroup ();


  return (true);
}


void envout (OPTIONS *options)
{
  //  Get the INI file name

  QString ini_file = QString (getenv ("HOME")) + "/.qexpressvpn.ini";

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("qexpressvpn");


  settings.setValue (QString ("settings version"), settings_version);

  settings.setValue (QString ("preferred protocol"), options->preferred_protocol);
  settings.setValue (QString ("auto disconnect"), options->auto_disconnect);
  settings.setValue (QString ("stay on top"), options->stay_on_top);
  settings.setValue (QString ("auto mini"), options->auto_mini);
  settings.setValue (QString ("auto connect"), options->auto_connect);
  settings.setValue (QString ("sort direction"), options->sort_direction);

  QString fontString = options->font.toString ();
  settings.setValue (QString ("font"), fontString);

  settings.setValue (QString ("server color/red"), options->server_color.red ());
  settings.setValue (QString ("server color/green"), options->server_color.green ());
  settings.setValue (QString ("server color/blue"), options->server_color.blue ());
  settings.setValue (QString ("server color/alpha"), options->server_color.alpha ());

  settings.setValue (QString ("connected color/red"), options->connected_color.red ());
  settings.setValue (QString ("connected color/green"), options->connected_color.green ());
  settings.setValue (QString ("connected color/blue"), options->connected_color.blue ());
  settings.setValue (QString ("connected color/alpha"), options->connected_color.alpha ());

  settings.setValue (QString ("disconnected color/red"), options->disconnected_color.red ());
  settings.setValue (QString ("disconnected color/green"), options->disconnected_color.green ());
  settings.setValue (QString ("disconnected color/blue"), options->disconnected_color.blue ());
  settings.setValue (QString ("disconnected color/alpha"), options->disconnected_color.alpha ());

  settings.setValue (QString ("current alias"), options->current_server.alias);
  settings.setValue (QString ("current country"), options->current_server.country);
  settings.setValue (QString ("current country alias"), options->current_server.country_alias);
  settings.setValue (QString ("current location"), options->current_server.location);
  settings.setValue (QString ("current recommended"), options->current_server.recommended);


  settings.setValue (QString ("width"), options->window_width);
  settings.setValue (QString ("height"), options->window_height);
  settings.setValue (QString ("x position"), options->window_x);
  settings.setValue (QString ("y position"), options->window_y);

  settings.setValue (QString ("status width"), options->status_window_width);
  settings.setValue (QString ("status height"), options->status_window_height);
  settings.setValue (QString ("status x position"), options->status_window_x);
  settings.setValue (QString ("status y position"), options->status_window_y);


  settings.endGroup ();
}
