#!/bin/sh

OPTIONS="--arch=ia32 --release --msvs_version=2013"

node-gyp clean configure $OPTIONS
node-gyp build $OPTIONS
./install.sh win 0.14

#./nw_gyp_windows.sh release
#./install.sh win 0.12
