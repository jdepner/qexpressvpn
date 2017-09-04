
/********************************************************************************************* 

    qexpressvpn.hpp

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


/*  qexpressvpn class definitions.  */

#ifndef _QEXPRESSVPN_HPP_
#define _QEXPRESSVPN_HPP_

#include <cmath>
#include <cerrno>

#include "qexpressvpnDef.hpp"
#include "statusDialog.hpp"
#include "version.hpp"


using namespace std;


class qexpressvpn:public QMainWindow

{
  Q_OBJECT 


public:

  qexpressvpn (QWidget *parent = 0, QString translatorName = "");
  ~qexpressvpn ();


protected:

  FILE            *qexpressvpn_tmp_fp;

  QProcess        *qexpressvpnProc;

  OPTIONS         options;

  MISC            misc;

  QTabWidget      *qexpressvpnTab, *serverTab;

  QTextEdit       *diagTextEdit;

  QTimer          *qexpressvpnTimer;

  QGroupBox       *serverBox;

  QVBoxLayout     *serverBoxLayout;

  QButtonGroup    *serverGrp;

  QTableWidget    *serverTable;

  QAction         *bHelp;

  QDialog         *statusDialogButtonD;

  QPushButton     *bConnect, *bDisconnect, *statusButton, *statusDialogButton, *bServerColor, *bConnectedColor, *bDisconnectedColor, *bFont;

  QComboBox       *protocol;

  QCheckBox       *autoDisconnect, *stayOnTop, *autoMini;

  QString         serverTextColorString, fontString, arg, tempFileName;

  QStringList     arguments;

  QPixmap         checkPixmap;

  statusDialog    *statusDialogD;

  uint8_t         normal_exit, connecting;


  void getStatus ();
  void setWidgetStates ();
  void closeEvent (QCloseEvent *event);


protected slots:

  void slotCurrentTabChanged (int index);

  void slotConnectClicked (bool checked);
  void slotDisconnectClicked (bool checked);
  void slotStatusButtonClicked (bool checked);
  void slotStatusDialogClosed ();

  void slotServerClicked (int id);

  void slotProcessError (QProcess::ProcessError error);
  void slotProcessReadyReadStandardOutput ();
  void slotProcessDone (int exitCode, QProcess::ExitStatus exitStatus);

  void slotServerColor ();
  void slotConnectedColor ();
  void slotDisconnectedColor ();
  void slotFont ();
  void slotProtocolChanged (int index);
  void slotAutoDisconnect (int state);
  void slotStayOnTop (int state);
  void slotAutoMini (int state);


  void slotHelp ();
  void slotAbout ();
  void slotQuit ();


private:

  void keyPressEvent (QKeyEvent *e);

};

#endif
