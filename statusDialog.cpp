
/********************************************************************************************* 

    statusDialog.cpp

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


#include "statusDialog.hpp"

statusDialog::statusDialog (QWidget * parent, OPTIONS *op, MISC *mi):
  QDialog (parent, (Qt::WindowFlags) Qt::WA_DeleteOnClose)
{
  options = op;
  misc = mi;
  normal_exit = false;


  //  Set the "Stay on top" hint if requested.

  if (options->stay_on_top)
    {
      Qt::WindowFlags flags = windowFlags ();
      setWindowFlags (flags | Qt::WindowStaysOnTopHint);
    }


  setWindowTitle ("qexpressvpn");


  //  Set the main icon

  setWindowIcon (misc->mainIcon);


  setModal (true);


  //  Icon size for the button.

  QSize iconSize = QSize (24, 24);


  QVBoxLayout *vBox = new QVBoxLayout (this);

  QIcon logo;
  QString bc;

  if (misc->connected)
    {
      logo = QIcon (":/resources/connect.png");
      bc = misc->fontString + misc->connectedTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg
        (options->connected_color.red ()).arg (options->connected_color.green ()).arg
        (options->connected_color.blue ()).arg (options->connected_color.alpha ());
    }
  else
    {
      logo = QIcon (":/resources/disconnect.png");
      bc = misc->fontString + misc->disconnectedTextColorString + QString ("background-color:rgba(%1,%2,%3,%4)").arg
        (options->disconnected_color.red ()).arg (options->disconnected_color.green ()).arg
        (options->disconnected_color.blue ()).arg (options->disconnected_color.alpha ());
    }


  statusDialogButton = new QPushButton (logo, misc->status, this);
  statusDialogButton->setIconSize (iconSize);
  statusDialogButton->setToolTip (tr ("Click this button return to the main qexpressvpn window [Enter]"));
  connect (statusDialogButton, SIGNAL (clicked (bool)), this, SLOT (slotStatusDialogButtonClicked (bool)));
  vBox->addWidget (statusDialogButton, 1);


  statusDialogButton->setStyleSheet (bc);


  //  Set the window size and location from the saved settings

  resize (options->status_window_width, options->status_window_height);
  move (options->status_window_x, options->status_window_y);


  //  Trying to keep it as small as possible.

  QSizePolicy sizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
  setSizePolicy (sizePolicy);


  //  Hide the main window.  We're doing this before we show the dialog because it seems
  //  to force KDE to add the icon to the task manager.  If we do it the other way,
  //  sometimes it adds the icon and sometimes it doesn't - go figure.  I have no idea
  //  what GNOME or UNITY or XFCE or (fill in your favorite manager here) will do.

  parent->hide ();


  show ();
}



statusDialog::~statusDialog ()
{
}



//  Why am I doing this???  See below.

void
statusDialog::keyPressEvent (QKeyEvent *e)
{
  //  Set up the shortcut (Enter) for the status button.  We can't use a regular shortcut because it
  //  doesn't work.

  //  Oh, why does the shortcut not work properly?  Because it gets thrown away when you change the text.
  //  You can reset the shortcut after you change the text and you can check it and see that the button
  //  has the correct shortcut... but it will no longer trigger the button.

  //  Anyway, this way we don't have to worry about translations.

  if (e->key () == Qt::Key_Return)
    {
      slotStatusDialogButtonClicked (true);
      return;
    }

  e->accept ();
}



void 
statusDialog::slotStatusDialogButtonClicked (bool checked __attribute__ ((unused)))
{
  //  Use frame geometry to get the absolute x and y.

  QRect tmp = this->frameGeometry ();
  options->status_window_x = tmp.x ();
  options->status_window_y = tmp.y ();


  //  Use geometry to get the width and height.

  tmp = this->geometry ();
  options->status_window_width = tmp.width ();
  options->status_window_height = tmp.height ();


  normal_exit = true;

  emit statusDialogClosedSignal ();

  close ();
}



void
statusDialog::slotHelp ()
{
  QWhatsThis::enterWhatsThisMode ();
}



//  Someone pushed the X button

void 
statusDialog::closeEvent (QCloseEvent *event __attribute__ ((unused)))
{
  if (!normal_exit) slotStatusDialogButtonClicked (true);
}
