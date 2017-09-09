
/********************************************************************************************* 

    qexpressvpn.cpp

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


/***************************************************************************\

    Module :        qexpressvpn

    Programmer :    Jan C. Depner

    Date :          August 7, 2017

    Purpose :       Simple GUI for ExpressVPN's Linux command line client.

\***************************************************************************/

#include "qexpressvpn.hpp"

qexpressvpn::qexpressvpn (QWidget *parent, QString translatorName):
  QMainWindow (parent, 0)
{
  uint8_t envin (OPTIONS *options);
  void set_defaults (MISC *misc, OPTIONS *options);


  QResource::registerResource ("/resources.rcc");


  strcpy (misc.version, VERSION);


  //  Initialize some important variables.

  qexpressvpnProc = NULL;
  serverTable = NULL;
  normal_exit = false;
  misc.translatorName = translatorName;
  connecting = false;


  //  Trying to make QToolTips easier to read.

  qApp->setStyleSheet ("QToolTip { border: 2px solid black; padding: 2px; color: black; background: white }");


  //  Have to set the focus policy or keypress events don't work properly at first in Focus Follows Mouse mode

  setFocusPolicy (Qt::WheelFocus);
  setFocus (Qt::ActiveWindowFocusReason);


  //  Set the check mark pixmap

  checkPixmap = (QPixmap (":/resources/check20.png"));


  //  Set all of the defaults

  set_defaults (&misc, &options);


  //  Get the user's defaults if available

  envin (&options);


  //  Set the window size and location from the saved settings

  this->resize (options.window_width, options.window_height);
  this->move (options.window_x, options.window_y);


  // Set the application font

  qApp->setFont (options.font);


  //  Set the temporary file name.  We'll just re-use the same file when we need one.

  tempFileName = QDir::tempPath () + "/" + QString ("qexpressvpn_%1.tmp").arg (misc.process_id);


  //  Now let's build the GUI.

  QFrame *frame = new QFrame (this, 0);

  setCentralWidget (frame);

  QVBoxLayout *mainBox = new QVBoxLayout (frame);
  mainBox->setMargin (5);
  mainBox->setSpacing (5);


  //  Main tab widget

  qexpressvpnTab = new QTabWidget ();
  qexpressvpnTab->setTabPosition (QTabWidget::North);



  //************************************* Connection tab ***************************************************

  QGroupBox *connectionBox = new QGroupBox (this);
  QVBoxLayout *connectionBoxLayout = new QVBoxLayout;
  connectionBox->setLayout (connectionBoxLayout);


  QGroupBox *connectionBarBox = new QGroupBox (this);
  connectionBarBox->setToolTip (tr ("Connect or disconnect from ExpressVPN server"));
  connectionBarBox->setWhatsThis (tr ("This tab is where you can connect to, or disconnect from, ExpressVPN servers.  "
                                      "You can also click the status button to hide the main window and just display the "
                                      "status button in its own small window."));
  QHBoxLayout *connectionBarBoxLayout = new QHBoxLayout;
  connectionBarBox->setLayout (connectionBarBoxLayout);


  //  Icon size for the connect and disconnect buttons.

  QSize iconSize = QSize (36, 36);


  bConnect = new QPushButton (tr ("&Connect"), this);
  bConnect->setShortcut (tr ("ctrl+c", "Connect"));
  bConnect->setIcon (QIcon (":/resources/connect.png"));
  bConnect->setIconSize (iconSize);
  bConnect->setToolTip (tr ("Connect to selected ExpressVPN server [Ctrl+c]"));
  bConnect->setWhatsThis (tr ("Click this button (or press Ctrl+c) to connect to the selected ExpressVPN server.  "
                              "Select the server in the Servers table."));
  connect (bConnect, SIGNAL (clicked (bool)), this, SLOT (slotConnectClicked (bool)));
  connectionBarBoxLayout->addWidget (bConnect);

  bDisconnect = new QPushButton (tr ("&Disconnect"), this);
  bDisconnect->setShortcut (tr ("ctrl+d", "Disconnect"));
  bDisconnect->setIcon (QIcon (":/resources/disconnect.png"));
  bDisconnect->setIconSize (iconSize);
  bDisconnect->setToolTip (tr ("Disconnect from ExpressVPN server [Ctrl+d]"));
  bDisconnect->setWhatsThis (tr ("Click this button (or press Ctrl+d) to disconnect from the selected ExpressVPN server."));
  connect (bDisconnect, SIGNAL (clicked (bool)), this, SLOT (slotDisconnectClicked (bool)));
  connectionBarBoxLayout->addWidget (bDisconnect);

  statusButton = new QPushButton (this);
  statusButton->setIcon (QIcon (":/resources/status.png"));
  statusButton->setIconSize (iconSize);
  statusButton->setToolTip (tr ("ExpressVPN connection status.  Click here to display just this button. [Enter]"));
  statusButton->setWhatsThis (tr ("This is the status of the ExpressVPN connection.  It is also a button.  If you click "
                                  "this button (or press Enter), the main window will be hidden and just this status "
                                  "button will be displayed on your desktop.  This allows you to see your ExpressVPN status "
                                  "without using up valuable screen real estate.  If you want to see the main window again "
                                  "just click on the single status button."));
  connect (statusButton, SIGNAL (clicked (bool)), this, SLOT (slotStatusButtonClicked (bool)));
  connectionBarBoxLayout->addWidget (statusButton, 1);


  connectionBoxLayout->addWidget (connectionBarBox);


  serverBox = new QGroupBox (tr ("Servers"), this);
  serverBoxLayout = new QVBoxLayout;
  serverBox->setLayout (serverBoxLayout);


  //  Button group

  serverGrp = new QButtonGroup (this);
  serverGrp->setExclusive (true);
  connect (serverGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotServerClicked (int)));


  connectionBoxLayout->addWidget (serverBox, 1);


  qexpressvpnTab->addTab (connectionBox, tr ("Connection"));
  qexpressvpnTab->setTabToolTip (CONN_TAB, tr ("Set connection to ExpressVPN server"));
  qexpressvpnTab->setTabWhatsThis (CONN_TAB, tr ("This tab allows you to select an ExpressVPN server and connect to it."));


  //  ******************************************* Preferences tab *********************************************

  QGroupBox *prefBox = new QGroupBox (this);
  QVBoxLayout *prefBoxLayout = new QVBoxLayout;
  prefBox->setLayout (prefBoxLayout);


  QHBoxLayout *colorsLayout = new QHBoxLayout (0);
  prefBoxLayout->addLayout (colorsLayout);

  bServerColor = new QPushButton (tr ("Server color"), this);
  bServerColor->setToolTip (tr ("Change color used for non-selected server buttons"));
  bServerColor->setWhatsThis (tr ("Select the color that you want to use for server buttons that are not selected.<br><br>"
                                  "<b>Major hint: don't set this to be the same as the connected or disconnected color.</b>"));
  connect (bServerColor, SIGNAL (clicked ()), this, SLOT (slotServerColor ()));
  colorsLayout->addWidget (bServerColor);

  bConnectedColor = new QPushButton (tr ("Connected color"), this);
  bConnectedColor->setToolTip (tr ("Change connected color"));
  bConnectedColor->setWhatsThis (tr ("Select the color that you want to use for the connect button, the selected server "
                                     "button, and the status button when connected to a server."));
  connect (bConnectedColor, SIGNAL (clicked ()), this, SLOT (slotConnectedColor ()));
  colorsLayout->addWidget (bConnectedColor);

  bDisconnectedColor = new QPushButton (tr ("Disconnected color"), this);
  bDisconnectedColor->setToolTip (tr ("Change disconnected color"));
  bDisconnectedColor->setWhatsThis (tr ("Select the color that you want to use for the disconnect button, the selected server "
                                        "button, and the status button when not connected to a server."));
  connect (bDisconnectedColor, SIGNAL (clicked ()), this, SLOT (slotDisconnectedColor ()));
  colorsLayout->addWidget (bDisconnectedColor);


  QHBoxLayout *miscLayout = new QHBoxLayout (0);
  prefBoxLayout->addLayout (miscLayout);

  bFont = new QPushButton (this);
  bFont->setToolTip (tr ("Change font for qexpressvpn"));
  bFont->setWhatsThis (tr ("Click this button to change the font used for text in the application."));
  bFont->setText (tr ("Font: ") + options.font.toString ());
  connect (bFont, SIGNAL (clicked ()), this, SLOT (slotFont ()));
  miscLayout->addWidget (bFont);

  protocol = new QComboBox (this);
  protocol->setToolTip (tr ("Change preferred protocol for ExpressVPN Linux client"));
  protocol->setWhatsThis (tr("Select the preferred protocol.  It is probably best to just leave this set to "
                             "<b>Auto</b> unless you have a good reason for changing it.<br><br>"
                             "<b>IMPORTANT NOTE: You cannot change the protocol while connected.  If you want "
                             "to change the protocol, please disconnect first.</b>"));
  protocol->setEditable (false);
  protocol->addItem (tr ("Protocol: Auto"));
  protocol->addItem (tr ("Protocol: UDP"));
  protocol->addItem (tr ("Protocol: TCP"));
  protocol->setCurrentIndex (options.preferred_protocol);
  connect (protocol, SIGNAL (currentIndexChanged (int)), this, SLOT (slotProtocolChanged (int)));
  miscLayout->addWidget (protocol);


  QHBoxLayout *checksLayout = new QHBoxLayout (0);
  prefBoxLayout->addLayout (checksLayout);

  autoConnect = new QCheckBox (tr ("Automatic connect"), this);
  autoConnect->setToolTip (tr ("Toggle automatic connect on open"));
  autoConnect->setWhatsThis (tr ("Set this button if you would like for qexpressvpn to automatically connect to the "
                                 "last used ExpressVPN server when you start the program.  If <b>Automatic minimize </b>"
                                 "is set, and the connection is successful, the GUI will become the minimized "
                                 "status dialog window."));
  autoConnect->setCheckable (true);
  autoConnect->setChecked (options.auto_connect);
  connect (autoConnect, SIGNAL (stateChanged (int)), this, SLOT (slotAutoConnect (int)));
  checksLayout->addWidget (autoConnect);


  autoDisconnect = new QCheckBox (tr ("Automatic disconnect"), this);
  autoDisconnect->setToolTip (tr ("Toggle automatic disconnect on close"));
  autoDisconnect->setWhatsThis (tr ("Set this button if you would like for qexpressvpn to disconnect from the "
                                    "ExpressVPN server when you exit the program.  If this is unset, "
                                    "the system will remain connected to the ExpressVPN server."));
  autoDisconnect->setCheckable (true);
  autoDisconnect->setChecked (options.auto_disconnect);
  connect (autoDisconnect, SIGNAL (stateChanged (int)), this, SLOT (slotAutoDisconnect (int)));
  checksLayout->addWidget (autoDisconnect);


  autoMini = new QCheckBox (tr ("Automatic minimize"), this);
  autoMini->setToolTip (tr ("Set this to force the GUI to become the status dialog when connected"));
  autoMini->setWhatsThis (tr ("Set this button if you would like for the GUI to become the minimized status dialog window when connected to a server.<br><br>"
                              "<b>NOTE: In this context, 'minimize' does not mean to iconify the application.  It means that the GUI becomes just the status button "
                              "dialog.</b>"));
  autoMini->setCheckable (true);
  autoMini->setChecked (options.auto_mini);
  connect (autoMini, SIGNAL (stateChanged (int)), this, SLOT (slotAutoMini (int)));
  checksLayout->addWidget (autoMini);


  stayOnTop = new QCheckBox (tr ("Status dialog on top"), this);
  stayOnTop->setToolTip (tr ("Toggle status dialog on top of other windows"));
  stayOnTop->setWhatsThis (tr ("Set this button if you would like for the minimized status dialog window to stay on top of other "
                               "windows.<br><br>"
                               "<b>NOTE: This is dependent on the window manager and is only a hint to the window manager.  It may not actually happen.</b>"));
  stayOnTop->setCheckable (true);
  stayOnTop->setChecked (options.stay_on_top);
  connect (stayOnTop, SIGNAL (stateChanged (int)), this, SLOT (slotStayOnTop (int)));
  checksLayout->addWidget (stayOnTop);


  qexpressvpnTab->addTab (prefBox, tr ("Preferences"));
  qexpressvpnTab->setTabToolTip (PREF_TAB, tr ("Set qexpressvpn preferences"));
  qexpressvpnTab->setTabWhatsThis (PREF_TAB, tr ("Click this button to edit the qexpressvpn preferences."));


  //  ******************************************* Diagnostics tab ***************************************************

  QGroupBox *diagBox = new QGroupBox (this);
  QVBoxLayout *diagBoxLayout = new QVBoxLayout;
  diagBox->setLayout (diagBoxLayout);


  diagTextEdit = new QTextEdit (this);
  diagTextEdit->setReadOnly (true);


  diagBoxLayout->addWidget (diagTextEdit);

  qexpressvpnTab->addTab (diagBox, tr ("Diagnostics"));
  qexpressvpnTab->setTabToolTip (DIAG_TAB, tr ("Diagnostics for the ExpressVPN Linux client"));
  qexpressvpnTab->setTabWhatsThis (DIAG_TAB, tr ("Diagnostics for the ExpressVPN Linux client."));



  mainBox->addWidget (qexpressvpnTab);



  //  ********************************************** Actions **************************************************

  QHBoxLayout *actions = new QHBoxLayout (0);
  mainBox->addLayout (actions);

  QPushButton *bHelp = new QPushButton (this);
  bHelp->setIcon (QIcon (":/resources/contextHelp.png"));
  bHelp->setToolTip (tr ("Enter What's This mode for help"));
  connect (bHelp, SIGNAL (clicked ()), this, SLOT (slotHelp ()));
  actions->addWidget (bHelp);

  actions->addStretch (10);

  QPushButton *aboutButton = new QPushButton (tr ("About"), this);
  aboutButton->setToolTip (tr ("Information about the qexpressvpn program"));
  aboutButton->setWhatsThis (tr ("Click this button to get general information about the qexpressvpn program."));
  connect (aboutButton, SIGNAL (clicked ()), this, SLOT (slotAbout ()));
  actions->addWidget (aboutButton);

  QPushButton *quitButton = new QPushButton (tr ("&Quit"), this);
  quitButton->setToolTip (tr ("Exit from qexpressvpn [Ctrl+q]"));
  quitButton->setWhatsThis (tr ("Click this button (or press Ctrl+q) to exit from the qexpressvpn program.<br><br>"
                                "<b>IMPORTANT NOTE: If you are connected to an ExpressVPN server, closing qexpressvpn "
                                "will not disconnect from the server unless you have set <i>Auto disconnect</i> "
                                "on in the <i>Preferences</i> tab.</b>"));
  quitButton->setShortcut (tr ("ctrl+q", "Quit"));
  connect (quitButton, SIGNAL (clicked ()), this, SLOT (slotQuit ()));
  actions->addWidget (quitButton);


  //  Get the current status...

  getStatus ();


  //  Set the preferred protocol...

  slotProtocolChanged (options.preferred_protocol);


  //  Now, get the server list...

  slotCurrentTabChanged (CONN_TAB);


  show ();


  //  If the user has selected the "auto minimize" option and we are connected to a server, go ahead and minimize the GUI.

  if (options.auto_mini && misc.connected) slotStatusButtonClicked (true);


  //  Now that the GUI is set up, go ahead and connect this signal.  If it was done earlier
  //  it would trigger too soon.

  connect (qexpressvpnTab, SIGNAL (currentChanged (int)), this, SLOT (slotCurrentTabChanged (int)));


  //  If "auto connect" is set, go ahead and try to connect to the last used ExpressVPN server.

  if (options.auto_connect) slotConnectClicked (true);
}



qexpressvpn::~qexpressvpn ()
{
}



//  Why am I doing this???  See below.

void
qexpressvpn::keyPressEvent (QKeyEvent *e)
{
  //  Set up the shortcut (Enter) for the status button.  We can't use a regular shortcut because it
  //  doesn't work.

  //  Oh, why does the shortcut not work properly?  Because it gets thrown away when you change the text.
  //  You can reset the shortcut after you change the text and you can check it and see that the button
  //  has the correct shortcut... but it will no longer trigger the button.

  //  Anyway, this way we don't have to worry about translations.

  //  We only want to use it if the current page is the connection page.

  if (qexpressvpnTab->currentIndex () == CONN_TAB && e->key () == Qt::Key_Return)
    {
      slotStatusButtonClicked (true);
      return;
    }

  e->accept ();
}



//  Get the current status

void 
qexpressvpn::getStatus ()
{
  qApp->setOverrideCursor (Qt::WaitCursor);
  qApp->processEvents ();

  if (qexpressvpnProc != NULL) delete qexpressvpnProc;

  qexpressvpnProc = new QProcess (this);

  misc.current_process = "status";


  qexpressvpnProc->setStandardOutputFile (tempFileName);


  arguments.clear ();
  arguments += misc.current_process;


  connect (qexpressvpnProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotProcessDone (int, QProcess::ExitStatus)));
  connect (qexpressvpnProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotProcessError (QProcess::ProcessError)));


  qexpressvpnProc->start (misc.progName, arguments);


  qexpressvpnProc->waitForFinished ();


  //  Even though the process is finished we still need to get the status information from the file...

  QFile temp_file (tempFileName);

  if (!temp_file.open (QIODevice::ReadOnly | QIODevice::Text))
    {
      QMessageBox::critical (this, "qexpressvpn",
                             tr ("Unable to open temporary file %1").arg (QString (tempFileName)));
      return;
    }


  QTextStream temp_str (&temp_file);

  QString txt = temp_str.readLine ();


  //  This text will be placed in the status button in setWidgetStates (when we set the statusButton color).

  misc.status = txt.remove ("\n");

  if (txt.contains ("Not connected"))
    {
      misc.connected = false;
    }
  else
    {
      misc.connected = true;
    }

  temp_file.close ();
}



void 
qexpressvpn::slotCurrentTabChanged (int index)
{
  switch (index)
    {
    case CONN_TAB:

      qApp->setOverrideCursor (Qt::WaitCursor);
      qApp->processEvents ();


      //  This is a PITA but, before we get the whole list, we have to get the recently used list...

      if (qexpressvpnProc != NULL) delete qexpressvpnProc;

      qexpressvpnProc = new QProcess (this);

      misc.current_process = "list";
      misc.process_ext = "recent";


      qexpressvpnProc->setStandardOutputFile (tempFileName);


      arguments.clear ();
      arguments += misc.current_process;
      arguments += misc.process_ext;


      connect (qexpressvpnProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotProcessDone (int, QProcess::ExitStatus)));
      connect (qexpressvpnProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotProcessError (QProcess::ProcessError)));


      qexpressvpnProc->start (misc.progName, arguments);


      qexpressvpnProc->waitForFinished ();



      if (qexpressvpnProc != NULL) delete qexpressvpnProc;

      qexpressvpnProc = new QProcess (this);



      //  Now we do the full list.

      misc.current_process = "list";
      misc.process_ext = "";


      qexpressvpnProc->setStandardOutputFile (tempFileName);


      arguments.clear ();
      arguments += misc.current_process;


      connect (qexpressvpnProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotProcessDone (int, QProcess::ExitStatus)));
      connect (qexpressvpnProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotProcessError (QProcess::ProcessError)));


      qexpressvpnProc->start (misc.progName, arguments);

      qexpressvpnProc->waitForFinished ();

      break;


    case DIAG_TAB:
      qApp->setOverrideCursor (Qt::WaitCursor);
      qApp->processEvents ();

      if (qexpressvpnProc != NULL) delete qexpressvpnProc;

      qexpressvpnProc = new QProcess (this);


      misc.current_process = "diagnostics";

      qexpressvpnProc->setStandardOutputFile (tempFileName);


      arguments.clear ();
      arguments += misc.current_process;


      connect (qexpressvpnProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotProcessDone (int, QProcess::ExitStatus)));
      connect (qexpressvpnProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotProcessError (QProcess::ProcessError)));


      qexpressvpnProc->start (misc.progName, arguments);

      break;

    case PREF_TAB:
      break;
    }
}



//!  This is where we connect to the selected server.

void 
qexpressvpn::slotConnectClicked (bool checked __attribute__ ((unused)))
{
  //  This protects against accidental (or screwy mouse) double-clicks.

  if (connecting) return;
  connecting = true;
  bConnect->setEnabled (false);


  qApp->setOverrideCursor (Qt::WaitCursor);
  qApp->processEvents ();

  if (qexpressvpnProc != NULL) delete qexpressvpnProc;

  qexpressvpnProc = new QProcess (this);


  //  Merge the standard output and standard error channels so we can get errors if the connect fails.
  //  This the only time we want to process the information returning from the application as it happens.
  //  Normally we'll just read the information from a text file since it's easier to parse that way.

  qexpressvpnProc->setProcessChannelMode (QProcess::MergedChannels);


  misc.current_process = "connect";


  arguments.clear ();
  arguments += misc.current_process;
  arguments += options.current_server.alias;


  connect (qexpressvpnProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotProcessDone (int, QProcess::ExitStatus)));
  connect (qexpressvpnProc, SIGNAL (readyReadStandardOutput ()), this, SLOT (slotProcessReadyReadStandardOutput ()));
  connect (qexpressvpnProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotProcessError (QProcess::ProcessError)));


  misc.connectString = "";


  //  Before we start the process we want to disable the status button.  It will get the status information but we don't want
  //  anyone clikcing it while it is in the process of connecting.

  statusButton->setEnabled (false);


  qexpressvpnProc->start (misc.progName, arguments);


  //  Always waitForFinished on connect since it can take a while (especially if it fails).  The expressvpn program
  //  has its own timeout built in.  If it can't connect it will still return 0 and, hopefully, we will have saved
  //  the error information in misc.connectString.

  qexpressvpnProc->waitForFinished (-1);


  //  Since we waited for it to finish, we can now re-enable the status button.

  statusButton->setEnabled (true);


  getStatus ();


  //  Check for connected status.  If we're not connected, we had a problem so we want to dump
  //  misc.connectString to a message box.

  if (!misc.connected)
    {
      qApp->restoreOverrideCursor ();

      QMessageBox::critical (this, tr ("qexpressvpn connect process failed"), misc.connectString);
    }
  else
    {
      //  Now, we need to update the "recent" list...  This is a shortcut ;-)

      slotCurrentTabChanged (CONN_TAB);


      //  If the auto minimize option is set we want to minimize the GUI.

      if (options.auto_mini) slotStatusButtonClicked (true);
    }

  connecting = false;
}



//!  This is where we disconnect from the ExpressVPN server.

void 
qexpressvpn::slotDisconnectClicked (bool checked __attribute__ ((unused)))
{
  qApp->setOverrideCursor (Qt::WaitCursor);
  qApp->processEvents ();

  if (qexpressvpnProc != NULL) delete qexpressvpnProc;

  qexpressvpnProc = new QProcess (this);

  misc.current_process = "disconnect";


  qexpressvpnProc->setStandardOutputFile (tempFileName);


  arguments.clear ();
  arguments += misc.current_process;

  connect (qexpressvpnProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotProcessDone (int, QProcess::ExitStatus)));
  connect (qexpressvpnProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotProcessError (QProcess::ProcessError)));


  qexpressvpnProc->start (misc.progName, arguments);


  qexpressvpnProc->waitForFinished ();


  getStatus ();


  //  Now, we need to update the "recent" list...  This is a shortcut ;-)

  slotCurrentTabChanged (CONN_TAB);
}



//!  Hide the main window and just show a button.

void 
qexpressvpn::slotStatusButtonClicked (bool checked __attribute__ ((unused)))
{
  statusDialogD = new statusDialog (this, &options, &misc);
  connect (statusDialogD, SIGNAL (statusDialogClosedSignal ()), this, SLOT (slotStatusDialogClosed ()));
}



//!  Get rid of the button and show the main window.

void 
qexpressvpn::slotStatusDialogClosed ()
{
  // Show the main window.

  show ();
}



//!  Select a server.

void 
qexpressvpn::slotServerClicked (int id)
{
  options.current_server = misc.server[id];

  setWidgetStates ();
}



void 
qexpressvpn::slotProcessError (QProcess::ProcessError error)
{
  switch (error)
    {
    case QProcess::FailedToStart:
      QMessageBox::critical (this, "qexpressvpn",
                             tr ("Unable to run the expressvpn program!  Make sure that the directory that contains it is in your PATH."));
      exit (-1);
      break;

    case QProcess::Crashed:
      QMessageBox::critical (this, "qexpressvpn",
                             tr ("The %1 process crashed!").arg (misc.current_process));
      break;

    case QProcess::Timedout:
      QMessageBox::critical (this, "qexpressvpn",
                             tr ("The %1 process timed out!").arg (misc.current_process));
      break;

    case QProcess::WriteError:
      QMessageBox::critical (this, "qexpressvpn",
                             tr ("There was a write error from the %1 process!").arg (misc.current_process));
      break;

    case QProcess::ReadError:
      QMessageBox::critical (this, "qexpressvpn",
                             tr ("There was a read error from the %1 process!").arg (misc.current_process));
      break;

    case QProcess::UnknownError:
      QMessageBox::critical (this, "qexpressvpn",
                             tr ("The %1 process died with an unknown error!").arg (misc.current_process));
      break;
    }
}



//  Read the information from the "connect" command.

void 
qexpressvpn::slotProcessReadyReadStandardOutput ()
{
  static QString resp_string = "";


  QByteArray response = qexpressvpnProc->readAllStandardOutput ();


  //  Parse the return response for carriage returns and line feeds

  for (int32_t i = 0 ; i < response.length () ; i++)
    {
      if (response.at (i) == '\n' || response.at (i) == '\r')
        {
          //  Get rid of the ECMA-48 Set Graphics Rendition (SGR) ESCAPE sequences.  These are used by the
          //  ExpressVPN Linux client to change the color/characteristics of the text going to the terminal.
          //  They always start with "\u001B[" and end with "m".

          int32_t start = resp_string.indexOf ("\u001B[");
          int32_t end = resp_string.indexOf ("m", start);
          if (start >= 0) resp_string.remove (start, (end - start) + 1);

          start = resp_string.indexOf ("\u001B[");
          end = resp_string.indexOf ("m", start);
          if (start >= 0) resp_string.remove (start, (end - start) + 1);


          //  Append the string to the connectString in case we have to dump an error message.

          if (response.at (i) == '\n') misc.connectString += (resp_string + "<br>");


          //  Only those messages that begin with "Connecting to" are placed in the status button.  The rest may be error messages.

          if (resp_string.contains ("Connecting to "))
            {
              //  Put the message in the status button text.

              statusButton->setText (resp_string);
              qApp->processEvents ();
            }

          resp_string = "";
        }
      else
        {
          resp_string += response.at (i);
        }
    }
}



//!  This is the return from the QProcess (when finished)

void 
qexpressvpn::slotProcessDone (int exitCode __attribute__ ((unused)), QProcess::ExitStatus exitStatus __attribute__ ((unused)))
{
  if (qexpressvpnProc->exitCode ()) return;


  if (misc.current_process == "list")
    {
      misc.server.clear ();


      QFile temp_file (tempFileName);

      if (!temp_file.open (QIODevice::ReadOnly | QIODevice::Text))
        {
          QMessageBox::critical (this, "qexpressvpn",
                                 tr ("Unable to open temporary file %1").arg (QString (tempFileName)));
          return;
        }


      QTextStream temp_str (&temp_file);


      int32_t line_count = 0;
      int32_t index = 0;
      QString current_country, current_country_alias;

      if (misc.process_ext == "recent") misc.recent_count = 0;


      while (!temp_str.atEnd ())
        {
          QString txt = temp_str.readLine ();


          //  Get the first two lines and discard them (after checking for contents)

          if (!line_count)
            {
              //  Note that we're not checking for "RECOMMENDED" because this may be a "recent" list.

              if (!txt.contains ("ALIAS") || !txt.contains ("COUNTRY") || !txt.contains ("LOCATION"))
                {
                  QMessageBox::critical (this, "qexpressvpn",
                                         tr ("Temporary file %1 missing contents line").arg (QString (tempFileName)));
                  temp_file.close ();
                  return;
                }
            }
          else if (line_count == 1)
            {
              if (!txt.contains ("-----"))
                {
                  QMessageBox::critical (this, "qexpressvpn",
                                         tr ("Temporary file %1 header is incorrect").arg (QString (tempFileName)));
                  temp_file.close ();
                  return;
                }
            }
          else
            {
              //  Remove LF  (CR should have been removed by the QTextStream).

              txt.remove (QRegExp ("[\\n]"));


              //  Split the line based on TAB characters

              QStringList txtList = txt.split ('\t', QString::SkipEmptyParts);


              SERVER server;


              //  If we're getting the "recent" list, all we need is the alias and the time

              if (misc.process_ext == "recent")
                {
                  misc.recent[misc.recent_count].alias = txtList.at (0);
                  misc.recent[misc.recent_count].recent = txtList.at (3);
                  misc.recent_count++;
                }
              else
                {
                  //  There is ALWAYS an alias

                  server.alias = txtList.at (0);


                  server.recent = "";
                  server.index = index++;


                  if (server.alias == "smart")
                    {
                      server.country = txtList.at (1);
                      server.country_alias = server.alias;
                      server.location = txtList.at (2);
                      server.recommended = true;
                    }
                  else
                    {
                      //  Simplest case - all 4 fields are present.

                      if (txtList.size () == 4)
                        {
                          QRegExp exp ("\\((.*)\\)");
                          if (exp.indexIn (txtList.at (1)) >= 0) server.country_alias = current_country_alias = exp.cap (1);
                          server.country = current_country = txtList.at (1).section ('(', 0, 0).simplified ();

                          server.location = txtList.at (2);
                          server.recommended = true;
                        }
                      else
                        {
                          if (txtList.size () == 3)
                            {
                              //  Check to see if the last field is a "Y".  If so, the country field is blank and
                              //  we'll just carry over the country and country alias from the last line.

                              if (txtList.at (2) == "Y")
                                { 
                                  server.location = txtList.at (1);
                                  server.recommended = true;
                                  server.country = current_country;
                                  server.country_alias = current_country_alias;
                                }
                              else
                                {
                                  server.location = txtList.at (2);

                                  QRegExp exp ("\\((.*)\\)");
                                  if (exp.indexIn (txtList.at (1)) >= 0) server.country_alias = current_country_alias = exp.cap (1);
                                  server.country = current_country = txtList.at (1).section ('(', 0, 0).simplified ();

                                  server.recommended = false;
                                }
                            }
                          else if (txtList.size () == 2)
                            {
                              server.location = txtList.at (1);
                              server.recommended = false;
                              server.country = current_country;
                              server.country_alias = current_country_alias;
                            }
                          else
                            {
                              QMessageBox::critical (this, "qexpressvpn",
                                                     tr ("Temporary file %1 entry %2 is unknown").arg (tempFileName).arg (txt));
                              temp_file.close ();
                              return;
                            }
                        }
                    }

                  misc.server.push_back (server);
                }
            }

          line_count++;
        }

      temp_file.close ();


      if (line_count < 2 && misc.current_process != "recent")
        {
          QMessageBox::critical (this, "qexpressvpn",
                                 tr ("Temporary file %1 contains no server list").arg (QString (tempFileName)));
          return;
        }


      int32_t num_servers = misc.server.size ();


      //  Check for recent servers

      for (int32_t i = 0 ; i < num_servers ; i++)
        {
          for (int32_t j = 0 ; j < misc.recent_count ; j++)
            {
              if (misc.recent[j].alias == misc.server[i].alias)
                {
                  misc.server[i].recent = misc.recent[j].recent;
                  break;
                }
            }
        }


      if (num_servers)
        {
          if (serverTable != NULL) delete serverTable;


          serverTable = new QTableWidget (num_servers, 4, this);
          serverTable->setWhatsThis (tr ("To select a server just click on the <b>Location</b> button for the "
                                         "server you want to use.<br><br>"
                                         "<b>IMPORTANT NOTE: You cannot change servers while connected.  If you "
                                         "want to change servers, please disconnect first.</b>"));
          serverTable->setAlternatingRowColors (true);


          QTableWidgetItem *cItemH = new QTableWidgetItem (tr ("Country"));
          serverTable->setHorizontalHeaderItem (0, cItemH);
          QTableWidgetItem *lItemH = new QTableWidgetItem (tr ("Location"));
          serverTable->setHorizontalHeaderItem (1, lItemH);
          QTableWidgetItem *yItemH = new QTableWidgetItem (tr ("Recommended"));
          serverTable->setHorizontalHeaderItem (2, yItemH);
          QTableWidgetItem *rItemH = new QTableWidgetItem (tr ("Recent"));
          serverTable->setHorizontalHeaderItem (3, rItemH);

          (serverTable->verticalHeader) ()->hide ();

          QTableWidgetItem *cItem[num_servers], *rItem[num_servers];
          QPushButton *lb[num_servers];
          QLabel *cl[num_servers];

          int32_t smart = -1, found = -1;

          for (int32_t i = 0 ; i < num_servers ; i++)
            {
              cItem[i] = new QTableWidgetItem (misc.server[i].country);
              serverTable->setItem (i, 0, cItem[i]);

              lb[i] = new QPushButton (misc.server[i].location, this);
              lb[i]->setCheckable (true);
              lb[i]->setToolTip (tr ("Select server at %1").arg (misc.server[i].location));

              serverGrp->addButton (lb[i], i);

              serverTable->setCellWidget (i, 1, lb[i]);


              //  Using a check pixmap here so that we don't have to worry about trying to translate "Y" into whatever 
              //  single character you would use in some other language to indicate "yes".  I mean, do you use "S"
              //  in Spanish or "J" in German???

              cl[i] = new QLabel ();
              cl[i]->setAlignment (Qt::AlignHCenter);

              if (misc.server[i].recommended)
                {
                  cl[i]->setPixmap (checkPixmap);
                }
              else
                {
                  cl[i]->setText ("");
                }

              serverTable->setCellWidget (i, 2, cl[i]);


              rItem[i] = new QTableWidgetItem (misc.server[i].recent);
              serverTable->setItem (i, 3, rItem[i]);


              //  This is just so that we set the server button the first time through.  It's redundant later
              //  but it was easier to set up here.  We check the actual text of the selected server alias because
              //  they might have changed since the last time.  If we don't get a match we'll just set it to "smart".

              if (misc.server[i].alias == "smart") smart = i;
              if (misc.server[i].alias == options.current_server.alias) found = i;
            }

          if (found < 0)
            {
              lb[smart]->setChecked (true);
              found = smart;
            }
          else
            {
              lb[found]->setChecked (true);
            }

          options.current_server = misc.server[found];


          serverTable->resizeColumnsToContents ();
          serverTable->resizeRowsToContents ();

          serverBoxLayout->addWidget (serverTable, 1);


          //  Make sure that the selected server is in the viewport.

          serverTable->scrollToItem (cItem[found], QAbstractItemView::PositionAtCenter);
        }
    }
  else if (misc.current_process == "diagnostics")
    {
      QFile temp_file (tempFileName);

      if (!temp_file.open (QIODevice::ReadOnly | QIODevice::Text))
        {
          QMessageBox::critical (this, "qexpressvpn",
                                 tr ("Unable to open temporary file %1").arg (QString (tempFileName)));
          return;
        }


      diagTextEdit->clear ();

      QTextStream temp_str (&temp_file);


      while (!temp_str.atEnd ())
        {
          QString txt = temp_str.readLine ();

          diagTextEdit->append (txt);
        }

      temp_file.close ();
    }
  else if (misc.current_process == "connect")
    {
      //  These aren't the droids you're looking for...
    }
  else if (misc.current_process == "disconnect")
    {
      //  He can go about his business...
    }
  else if (misc.current_process == "status")
    {
      //  Move along.
    }
  else if (misc.current_process == "protocol")
    {
    }
  else
    {
    }


  misc.process_ext = "";


  qApp->restoreOverrideCursor ();


  setWidgetStates ();
}



//  Change the server color

void
qexpressvpn::slotServerColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (options.server_color, this, tr ("qexpressvpn Server Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) options.server_color = clr;

  int32_t hue, sat, val;

  options.server_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      serverTextColorString = "color: white; ";
    }
  else
    {
      serverTextColorString = "color: black; ";
    }

  setWidgetStates ();
}



//  Change the connected color

void
qexpressvpn::slotConnectedColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (options.connected_color, this, tr ("qexpressvpn Connected Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) options.connected_color = clr;

  int32_t hue, sat, val;

  options.connected_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      misc.connectedTextColorString = "color: white; ";
    }
  else
    {
      misc.connectedTextColorString = "color: black; ";
    }

  setWidgetStates ();
}



//  Change the disconnected color

void
qexpressvpn::slotDisconnectedColor ()
{
  QColor clr;

  clr = QColorDialog::getColor (options.disconnected_color, this, tr ("qexpressvpn Disconnected Color"), QColorDialog::ShowAlphaChannel);

  if (clr.isValid ()) options.disconnected_color = clr;

  int32_t hue, sat, val;

  options.disconnected_color.getHsv (&hue, &sat, &val);
  if (val < 128)
    {
      misc.disconnectedTextColorString = "color: white; ";
    }
  else
    {
      misc.disconnectedTextColorString = "color: black; ";
    }

  setWidgetStates ();
}



//  Change the font

void
qexpressvpn::slotFont ()
{
  bool ok;
  QFont font = QFontDialog::getFont (&ok, options.font, this);
  if (ok)
    {
      options.font = font;

      setWidgetStates ();
    }
}



//  Change the protocol

void
qexpressvpn::slotProtocolChanged (int index)
{
  options.preferred_protocol = index;


  QString protocolString = "auto";

  switch (options.preferred_protocol)
    {
    case AUTO:
      protocolString = "auto";
      break;

    case UDP:
      protocolString = "udp";
      break;

    case TCP:
      protocolString = "tcp";
      break;
    }

  qApp->setOverrideCursor (Qt::WaitCursor);
  qApp->processEvents ();

  if (qexpressvpnProc != NULL) delete qexpressvpnProc;

  qexpressvpnProc = new QProcess (this);

  misc.current_process = "protocol";
  misc.process_ext = protocolString;


  qexpressvpnProc->setStandardOutputFile (tempFileName);


  arguments.clear ();
  arguments += misc.current_process;
  arguments += misc.process_ext;


  connect (qexpressvpnProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotProcessDone (int, QProcess::ExitStatus)));
  connect (qexpressvpnProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotProcessError (QProcess::ProcessError)));


  qexpressvpnProc->start (misc.progName, arguments);


  qexpressvpnProc->waitForFinished ();
}



//  Toggle the auto connect value

void
qexpressvpn::slotAutoConnect (int state)
{
  if (state == Qt::Checked)
    {
      options.auto_connect = true;
    }
  else
    {
      options.auto_connect = false;
    }
}



//  Toggle the auto disconnect value

void
qexpressvpn::slotAutoDisconnect (int state)
{
  if (state == Qt::Checked)
    {
      options.auto_disconnect = true;
    }
  else
    {
      options.auto_disconnect = false;
    }
}



//  Toggle the "Stay on top" hint value

void
qexpressvpn::slotStayOnTop (int state)
{
  if (state == Qt::Checked)
    {
      options.stay_on_top = true;
    }
  else
    {
      options.stay_on_top = false;
    }
}



//  Toggle the auto minimize value

void
qexpressvpn::slotAutoMini (int state)
{
  if (state == Qt::Checked)
    {
      options.auto_mini = true;
    }
  else
    {
      options.auto_mini = false;
    }
}



void
qexpressvpn::slotHelp ()
{
  QWhatsThis::enterWhatsThisMode ();
}



//  Somebody pressed the little X in the window decoration.  DOH!

void 
qexpressvpn::closeEvent (QCloseEvent *event __attribute__ ((unused)))
{
  //  Don't loop if this was caused by the close () call in slotQuit.

  if (!normal_exit) slotQuit ();
}



void
qexpressvpn::slotAbout ()
{
  //  Because we're playing with the main window icon to give the user a better idea of what's
  //  happening when the window is minimized we have to set the main window icon back to the
  //  normal, transparent background, icon when we display the "About" text.  This way we always
  //  have the "normal" icon in the upper left of the "About" text.  Of course, afterwards we
  //  have to reset the main icon to reflect the connection state.  We do that by calling
  //  setWidgetStates.

  QIcon icon (":/resources/qexpressvpn.png");
  setWindowIcon (icon);

  QMessageBox::about (this, VERSION, tr ("<br><br><center><h1><b>qexpressvpn</b></h1></center><br>"
                                         "This program is a simple GUI driver for the ExpressVPN Linux command line program.  "
                                         "It allows you to connect to, and disconnect from, ExpressVPN servers around the world.  "
                                         "Once you have set up the preferences, selected a server, and connected using the <b>Connect</b> "
                                         "<img source=\":/resources/connect_small.png\"> button you can minimize the GUI to a single, small "
                                         "window containing just the ExpressVPN connection status by clicking the <b>ExpressVPN connection "
                                         "status</b> button <img source=\":/resources/status_small.png\">.<br><br>"
                                         "For more detailed information about each widget (e.g. button, field, label...) in <b>qexpressvpn</b> "
                                         "just click the <b>Help</b> button <img source=\":/resources/contextHelp_small.png\"> in the lower "
                                         "left corner and then click on the widget you are interested in."
                                         "<center><h2><b><a href=\"https://www.expressvpn.com/\">ExpressVPN</a></b></h2></center><br><br>"
                                         "&nbsp;&nbsp;&nbsp;&nbsp;"
                                         "ExpressVPN is a registered trademark of ExpressVPN.  Neither this application nor the author of "
                                         "this application is endorsed by or affiliated in any way (other than being a satisfied customer) "
                                         "with ExpressVPN.<br><br>"
                                         "<center><h3>Disclaimer</h3></center>"
                                         "&nbsp;&nbsp;&nbsp;&nbsp;"
                                         "The qexpressvpn program is Copyright (c) 2017 Jan C. Depner (jan@pfmabe.software) and is released under "
                                         "version 3 of the GNU General Public License (GPLv3).<br><br>"
                                         "qexpressvpn is free software; you can redistribute it and/or modify it under the terms of the GNU "
                                         "General Public License as published by the Free Software Foundation; either version 3 of the "
                                         "License, or (at your option) any later version.<br><br>"
                                         "qexpressvpn is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even "
                                         "the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General "
                                         "Public License for more details.<br><br>"
                                         "You should have received a copy of the GNU General Public License along with this program; if not, "
                                         "write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA "
                                         "02110-1301  USA<br><br>"));

  setWidgetStates ();
}



void 
qexpressvpn::slotQuit ()
{
  void envout (OPTIONS *options);


  //  If connected and auto disconnect is on, disconnect.

  if (misc.connected && options.auto_disconnect) slotDisconnectClicked (true);


  //  Use frame geometry to get the absolute x and y.

  QRect tmp = this->frameGeometry ();
  options.window_x = tmp.x ();
  options.window_y = tmp.y ();


  //  Use geometry to get the width and height.

  tmp = this->geometry ();
  options.window_width = tmp.width ();
  options.window_height = tmp.height ();


  //  Save the user's defaults

  envout (&options);


  //  Get rid of the temp file (we don't care if this fails).

  QFile::remove (tempFileName);


  //  Set this flag so that slotCloseEvent won't loop back on this function.

  normal_exit = true;
  close ();
}



//  Set all the widget states that might need to be changed due to option/state changes.

void
qexpressvpn::setWidgetStates ()
{
  qApp->setFont (options.font);

  bFont->setText (tr ("Font: ") + options.font.toString ());

  misc.fontString = QString ("font: ") + options.font.toString () + QString ("; ");


  QString bc = misc.fontString + serverTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg (options.server_color.red ()).arg
    (options.server_color.green ()).arg (options.server_color.blue ()).arg (options.server_color.alpha ());
  bServerColor->setStyleSheet (bc);

  for (uint32_t i = 0 ; i < misc.server.size () ; i++) serverTable->cellWidget (i, 1)->setStyleSheet (bc);

  bc = misc.fontString + misc.connectedTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg (options.connected_color.red ()).arg
    (options.connected_color.green ()).arg (options.connected_color.blue ()).arg (options.connected_color.alpha ());
  bConnectedColor->setStyleSheet (bc);

  bc = misc.fontString + misc.disconnectedTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg
    (options.disconnected_color.red ()).arg (options.disconnected_color.green ()).arg
    (options.disconnected_color.blue ()).arg (options.disconnected_color.alpha ());
  bDisconnectedColor->setStyleSheet (bc);


  if (misc.connected)
    {
      bConnect->setEnabled (false);
      bDisconnect->setEnabled (true);
      protocol->setEnabled (false);
      for (uint32_t i = 0 ; i < misc.server.size () ; i++) serverTable->cellWidget (i, 1)->setEnabled (false);

      bc = misc.fontString + misc.connectedTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg (options.connected_color.red ()).arg
        (options.connected_color.green ()).arg (options.connected_color.blue ()).arg (options.connected_color.alpha ());
    }
  else
    {
      bConnect->setEnabled (true);
      bDisconnect->setEnabled (false);
      protocol->setEnabled (true);
      for (uint32_t i = 0 ; i < misc.server.size () ; i++) serverTable->cellWidget (i, 1)->setEnabled (true);

      bc = misc.fontString + misc.disconnectedTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg
        (options.disconnected_color.red ()).arg (options.disconnected_color.green ()).arg 
        (options.disconnected_color.blue ()).arg (options.disconnected_color.alpha ());
    }

  statusButton->setStyleSheet (bc);
  statusButton->setText (misc.status);

  if (misc.server.size ()) serverTable->cellWidget (options.current_server.index, 1)->setStyleSheet (bc);


  //  This is a bit tricky.  We want the window icon for this and the statusDialog to have a
  //  background color that is either the "connected" color or the "disconnected" color and we
  //  also want it to use the correct image (connected or disconnected).  That way, if we
  //  minimize either window we can still see the state of the connection just by glancing at
  //  the icon in whatever task manager bar we have on the desktop (we hope).

  QPixmap modIcon;
  if (misc.connected)
    {
      QPixmap icon (":/resources/connect.png");
      modIcon = QPixmap (icon.width (), icon.height ());
      modIcon.fill (options.connected_color);
      QPainter painter (&modIcon);
      painter.drawPixmap (0, 0, icon);
    }
  else
    {
      QPixmap icon (":/resources/disconnect.png");
      modIcon = QPixmap (icon.width (), icon.height ());
      modIcon.fill (options.disconnected_color);
      QPainter painter (&modIcon);
      painter.drawPixmap (0, 0, icon);
    }

  misc.mainIcon = QIcon (modIcon);
  setWindowIcon (misc.mainIcon);
}
