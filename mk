#!/bin/bash


# Building the .pro file using qmake

rm -f qrc_resources.cpp qexpressvpn.pro Makefile

qmake -project -o qexpressvpn.tmp
cat >qexpressvpn.pro <<EOF
QT += widgets
CONFIG += console
CODECFORSRC = UTF-8
EOF


cat qexpressvpn.tmp >>qexpressvpn.pro
rm qexpressvpn.tmp


# Building the Makefile file using qmake

qmake -o Makefile



make


export DESTINATION=${1:-"."}

if [ $DESTINATION != "." ]; then
    mv qexpressvpn $DESTINATION
fi


# Get rid of the Makefile so there is no confusion.  It will be generated again the next time we build.

rm Makefile
