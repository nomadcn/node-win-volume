#!/bin/bash -x

OUTPUT=sound_win.node 
OPTIONS="--target=0.14.5 --arch=ia32 --msvs_version=2013"

parse_arguments() {
    case $1 in
        "debug")
            nw-gyp clean configure $OPTIONS --debug
            nw-gyp build $OPTIONS --verbose --debug
            DIR=Debug
            ;;
        *)
            nw-gyp clean configure $OPTIONS
            nw-gyp build $OPTIONS --verbose
            DIR=Release
            ;;
    esac

#    cp build/$DIR/$OUTPUT ../nw_sdk_test/build/
#    cp build/$DIR/$OUTPUT ../handoff_pc_app/build/nablesdk_addon_win.node
}

cp binding.gyp.windows binding.gyp
rm -rf build
parse_arguments $1
