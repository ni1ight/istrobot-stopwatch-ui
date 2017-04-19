#!/bin/bash
INSTALL_DIR='/usr/local/bin/'
DESKTOP_DIR=`realpath ~/.local/share/applications/`
INSTALL_ICON=`realpath ~/.local/share/icons/`

cp StopWatch ${INSTALL_DIR}
cp ./../icons/StopWatch.png ${INSTALL_ICON}

echo "
[Desktop Entry]
Encoding=UTF-8
Version=1.0
Type=Application
Name=StopWatch
Icon=StopWatch.png
Path='${INSTALL_DIR}'
Exec='${INSTALL_DIR}/StopWatch'
StartupNotify=false
StartupWMClass=StopWatch
OnlyShowIn=Unity;
X-UnityGenerated=true
" > ${DESKTOP_DIR}/stopwatch.desktop

