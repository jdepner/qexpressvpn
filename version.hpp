
/********************************************************************************************* 

    version.hpp

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


#ifndef VERSION

#define     VERSION     "PFM Software - qexpressvpn V1.04 - 09/04/17"

#endif

/*! <pre>


    Version 1.00
    Jan C. Depner (PFM Software)
    08/24/17

    First working version.


    Version 1.01
    Jan C. Depner (PFM Software)
    08/29/17

    Fixed bug caused by running with no "recent" servers.  In other words, no connections have ever been made to
    ExpressVPN servers.


    Version 1.02
    Jan C. Depner (PFM Software)
    08/31/17

    Disabled the status button while trying to connect to a server.


    Version 1.03
    Jan C. Depner (PFM Software)
    09/03/17

    Added Preferences option for "Stay on top" hint to be set for status dialog.


    Version 1.04
    Jan C. Depner (PFM Software)
    09/04/17

    - Added Preferences option to automatically minimize (to the status button) on connect.
    - Rearranged the Preferences so they weren't just stacked up.
    - Made getStatus wait until the status file was read prior to returning.

</pre>*/
