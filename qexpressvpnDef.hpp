
/********************************************************************************************* 

    qexpressvpnDef.hpp

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


#ifndef _QEXPRESSVPN_DEF_HPP_
#define _QEXPRESSVPN_DEF_HPP_


#define __STDC_FORMAT_MACROS
#define __STDC_LIMIT_MACROS


#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif


using namespace std;


#define CONN_TAB       0
#define PREF_TAB       1
#define DIAG_TAB       2

#define ALL            0
#define RECOMMENDED    1

#define AUTO           0
#define UDP            1
#define TCP            2


typedef struct
{
  QString           alias;                      //  ExpressVPN server alias
  QString           country;                    //  Country
  QString           country_alias;              //  ExpressVPN country alias
  QString           location;                   //  Location
  uint8_t           recommended;                //  Recommended flag
  QString           recent;                     //  Recent string
  int32_t           index;                      //  Index into the misc.server vector (only used for options.current_server)
} SERVER;


//  The OPTIONS structure contains all those variables that can be saved to the users qexpressvpn QSettings.

typedef struct
{
  QFont             font;                       //  Uh... font?
  QColor            server_color;               //  Color used for all servers that aren't "recommended" or "recent"
  QColor            connected_color;            //  Connected color
  QColor            disconnected_color;         //  Disconnected color
  int32_t           preferred_protocol;         //  Preferred protocol (0 = auto, 1 = udp, 2 = tcp)
  int32_t           server_type;                //  0 = show all servers, 1 = show only recommended servers
  SERVER            current_server;             //  The current server
  uint8_t           auto_disconnect;            //  Set to true if you want to automatically disconnect from the ExpressVPN server
                                                //  when you close the program.
  uint8_t           auto_connect;               //  Set to true if you want to automatically connect to the last used ExpressVPN server
                                                //  when you open the program.
  uint8_t           stay_on_top;                //  Set to true if the user wants the statusDialog to stay on top of other windows (depending on window manager)
  uint8_t           auto_mini;                  //  Set to true if the user wants the GUI to automatically become just the status dialog on connect

  int32_t           window_width;               //  Main window width
  int32_t           window_height;              //  Main window height
  int32_t           window_x;                   //  Main window x position
  int32_t           window_y;                   //  Main window y position

  int32_t           status_window_width;        //  Status window width
  int32_t           status_window_height;       //  Status window height
  int32_t           status_window_x;            //  Status window x position
  int32_t           status_window_y;            //  Status window y position
} OPTIONS;


//  General stuff.

typedef struct
{
  int32_t           process_id;                 //  This program's process ID
  char              version[128];               //  qexpressvpn version string
  std::vector<SERVER> server;                   //  ExpressVPN server list
  SERVER            recent[3];                  //  The 3 most recent servers
  int32_t           recent_count;               //  Just in case we haven't ever conneected or we've only used 1 or 2 servers
  QString           current_process;            //  Name of the current operation being performed via QProcess
  QString           process_ext;                //  Process extension.  For second argument on expressvpn calls.
  uint8_t           connected;                  //  This is true if connected to a server
  QString           status;                     //  Status of the ExpressVPN connection
  uint32_t          major_version;              //  Major version number of expressvpn client
  uint32_t          minor_version;              //  Minor version number of expressvpn client
  uint32_t          list_all_loc[4];            //  Locations of start of string in "list all" output (for expressvpn v2.1.x and higher)
  QString           connectedTextColorString;
  QString           disconnectedTextColorString;
  QString           fontString;
  QString           progName;                   //  Program name of the ExpressVPN Linux client (just in case they change it ;-)
  QString           connectString;              /*  A place to temporarily store information from the "connect" command in case
                                                    we get an error back.  */
  QIcon             mainIcon;                   //  This is the window icon used for both the main window and the status dialog.
} MISC;


#endif
