{
  "targets": [
    {
      "target_name": "win-volume",
      "sources": [
        "./src/volume.cc",
        "./src/VolumeControl7.cpp",
        "./src/ext/gwutils/Flags.cpp",
        "./src/ext/gwutils/Logger.cpp",
        "./src/ext/gwutils/ThreadSafe.cpp"
        ],
      "include_dirs": [ "./src", "./src/ext/gwutils" ],
      "libraries": [ "winmm.lib" ],
      'configurations' : {
        'Debug' : {
          'msvs_settings': {
            'VCCLCompilerTool': {
              'RuntimeLibrary': '3' # /MDd
            },
            'VCLinkerTool': {
              'AdditionalOptions': ['/ignore:4099'],
              'IgnoreDefaultLibraryNames': ['libcmtd.lib']
            }
          }
        },
        'Release' : {
          'msvs_settings': {
            'VCCLCompilerTool': {
              'RuntimeTypeInfo': 'true', # To disable '/GR-'
              'RuntimeLibrary': '2' # /MDd
            },
            'VCLinkerTool': {
              'AdditionalOptions': ['/ignore:4099'],
              'IgnoreDefaultLibraryNames': ['libcmt.lib']
            }
          }
        }
      }
    }
  ]
}
