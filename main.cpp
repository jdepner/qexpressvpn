
/********************************************************************************************* 

    main.cpp

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


/*  qexpressvpn  */

#include "qexpressvpn.hpp"


int
main (int argc, char **argv)
{
    QApplication a (argc, argv);


    QTranslator translator;
    QString translatorName = QString ("qexpressvpn_") + QLocale::system ().name ();
    //QString translatorName = "qexpressvpn_es_MX.utf8";
    translator.load (translatorName, ":/resources");
    a.installTranslator (&translator);


    a.setStyle (QStyleFactory::create ("Plastique"));


    qexpressvpn *ac = new qexpressvpn (0, translatorName);


    ac->setWindowTitle (VERSION);


    return a.exec ();
}
