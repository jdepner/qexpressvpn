
/********************************************************************************************* 

    set_defaults.cpp

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

void set_defaults (MISC *misc, OPTIONS *options)
{
  misc->process_id = getpid ();
  misc->connected = false;
  misc->status = "Unknown";
  misc->progName = "expressvpn";
  misc->recent_count = 0;


  options->preferred_protocol = 0;
  options->server_color = QColor (255, 255, 255, 255);
  options->connected_color = QColor (0, 255, 0, 255);
  options->disconnected_color = QColor (255, 0, 0, 255);
  options->preferred_protocol = 0;
  options->server_type = 0;
  options->current_server.alias = "smart";
  options->current_server.country = "Smart location";
  options->current_server.country_alias = "";
  options->current_server.location = "";
  options->current_server.recommended = true;
  options->current_server.recent = "";
  options->current_server.index = 0;
  options->auto_disconnect = false;
  options->stay_on_top = false;
  options->auto_mini = false;

  options->window_width = 645;
  options->window_height = 700;
  options->window_x = 0;
  options->window_y = 0;

  options->status_window_width = 150;
  options->status_window_height = 75;
  options->status_window_x = 0;
  options->status_window_y = 0;
}
