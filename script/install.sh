#!/bin/bash

if [ $# -ne 2 ]; then
    echo "[Usage] install.sh [win or mac] [0.12 or 0.14]"
    exit
fi

OS=$1
NW_VERSION=$2
FILE=volume_${OS}.node 

SRC=build/Release/$FILE
#DST1=../../skhandoff_nw_addon/handoff_pc_app/node_modules/bindings/build
DST1=../../skhandoff_nw_addon/handoff_pc_app/node_modules
DST2=../../skhandoff_nw_addon/handoff_pc_app/nodes/$NW_VERSION
DST3=../build/$NW_VERSION

mkdir ../build/$NW_VERSION

cp $SRC $DST1
cp $SRC $DST2
cp $SRC $DST3
md5sum $SRC $DST1/$FILE $DST2/$FILE $DST3/$FILE
