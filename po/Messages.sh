#!/bin/sh

# Extract strings from all source files
$XGETTEXT `find .. -name \*.cpp -o -name \*.h -o -name \*.qml -o -name \*.ui` -o $podir/kdevflatpakbuilder.pot