
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

#define     VERSION     "PFM Software - qexpressvpn V1.11 - 10/12/18"

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
    - Throws away any accidental mouse double-clicks on the connect button.


    Version 1.05
    Jan C. Depner (PFM Software)
    09/05/17

    Moved statusButton text setting to setWidgetStates so that changes to the text and color occur 
    more smoothly.


    Version 1.06
    Jan C. Depner (PFM Software)
    09/09/17

    Added "Automatic connect" on open option.


    Version 1.07
    Jan C. Depner (PFM Software)
    09/11/17

    Forced the status button to change color before exit if "Automatic disconnect" is selected.


    Version 1.08
    Jan C. Depner (PFM Software)
    09/18/17

    - Added a delay to exit if "Automatic disconnect" is selected.
    - Added a lock file so that only one instance of the program can run.
    - Removed an unused argument to qexpressvpn.


    Version 1.09
    Jan C. Depner (PFM Software)
    11/28/17

    - Handle 'A new version is available' message from ExpressVPN server.



    Version 1.10
    Jan C. Depner (PFM Software)
    06/13/18

    - Added a utility timer so that the GUI will appear prior to trying to connect (I hope).


    Version 1.11
    Jan C. Depner (PFM Software)
    10/12/18

    - Remove ECMA text formatting junk from status information so it doesn't show up in the 
      statusButton.  As of expressvpn 1.5, ECMA stuff has been added to some of the status 
      messages.

</pre>*/
