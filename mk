#!/bin/bash


# Building the .pro file using qmake

rm -f qrc_resources.cpp qexpressvpn.pro Makefile

qmake -project -o qexpressvpn
cat >qexpressvpn.tmp <<EOF
QT += widgets
CONFIG += console
CONFIG += -no-pie
CODECFORSRC = UTF-8
EOF


cat qexpressvpn.pro >>qexpressvpn.tmp
rm qexpressvpn.pro
mv qexpressvpn.tmp qexpressvpn.pro


# Building the Makefile file using qmake

qmake -o Makefile



make


export DESTINATION=${1:-"."}

if [ $DESTINATION != "." ]; then
    mv qexpressvpn $DESTINATION
fi


# Get rid of the Makefile so there is no confusion.  It will be generated again the next time we build.

rm Makefile
