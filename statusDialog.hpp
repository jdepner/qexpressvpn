
/********************************************************************************************* 

    statusDialog.hpp

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


#ifndef STATUS_DIALOG_H
#define STATUS_DIALOG_H


#include "qexpressvpnDef.hpp"


class statusDialog:public QDialog
{
  Q_OBJECT 


    public:

  statusDialog (QWidget *parent = 0, OPTIONS *op = NULL, MISC *mi = NULL);
  ~statusDialog ();


 signals:

  void statusDialogClosedSignal ();


 protected:

  OPTIONS         *options;

  MISC            *misc;

  QPushButton     *statusDialogButton;

  uint8_t         normal_exit;


  void closeEvent (QCloseEvent *event);


  protected slots:

  void slotStatusDialogButtonClicked (bool checked);
  void slotHelp ();

 private:

  void keyPressEvent (QKeyEvent *e);

};

#endif
