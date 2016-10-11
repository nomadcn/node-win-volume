# win-volume-prebuilt
win-volume module for electron
Used to control master volume on win32.

## Change Log
* 1.0.1 - 2016.09.09
  * built for electron-1.3.4-win32-ia32.
  * Fix CoInitializeEx() error. (RPC_E_CHANGED_MODE)
* 1.0.0 - 2016.09.08
  * built for electron-1.3.4-win32-ia32.
  * volume control for win7 or later.

## Dependencies
* electron-prebuilt: 1.3.4
* msvs-2013

## How to build
```
// build for electron
$ cd win-volume
$ npm install
```

## Installation
```
$ npm install win-volume
```

## How to use
```
const volume = require('win-volume')

// log file is optional.
// initialize win-volume module.
volume.init('volume.log')
volume.open()

// volume level is in the range from 0 to 100.
var micVolume = volume.getVolume('mic')
volume.setVolume('mic', 50)

// 0(not mute) 1(mute) -1(error)
var mute = volume.getMute('mic')
// mic mute
volume.setMute('mic', true)

// volume level is in the range from 0 to 100.
var speakerVolume = volume.getVolume('speaker')
volume.setVolume('speaker', 50)
// speaker not mute
volume.setMute('speaker', false)

// 0(not mute) 1(mute) -1(error)
var mute = volume.getMute('speaker')

volume.close()
volume.exit()

```
