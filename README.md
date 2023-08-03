## How To Build WeiRtc Windows Native WebRtc Lib and Sample App

### Requirement for running the sample App.

1. Visual Studio 2019 Version 16.6.2 or later.
2. At least 8GB of RAM (16GB of RAM is recommended).
3. At least 15GB of disk space.

### Acquiring Winrtc and adding WebRtc Patches

1. Acqiring the WinRtc. Goto to the [Winrtc GitHub Repo](https://github.com/microsoft/winrtc).

2. Clone the Repo for winrtc, by clicking download zip or using command line
```
C:\> git clone https://github.com/microsoft/winrtc.git
```

Check out m84,


3. Goto the [Patches for WebRTC](https://github.com/microsoft/winrtc/tree/master/patches_for_WebRTC_org/m84).
Follow the steps to acqire WebRTC and add the Patches needed to build UWP ([Universal Windows Platform](https://docs.microsoft.com/en-us/windows/uwp/get-started/universal-application-platform-guide)).

4. Build webrtc using c++17

looks like that for windows runtime lib, need to compile with c++17, for “winuwp”, some generated file has flag: /std$:c++17, for “win”, 
there is not such flag, how to add it to “win“ is: file /config/win/BUILD.gn,  this section: config("runtime_library"),  add following line for none “winuwp” case:
```
 cflags_cc += [
      "/std:c++17",  # Required for having a wrapper using the WinRT/C++ projection.
    ]
```
5. Build webrtc as Dynamic lib flag "/MD", not "/MT": 

https://groups.google.com/g/discuss-webrtc/c/-fRj58xGFBg

Like, for windows, there is this file D:\git\WeiStreaming\windows\webrtc\src\build\config\win\BUILG.gn, has all the building flags like following section.  Please note, all file under src\build are having different git repo, any change made, need to go to src\build folder to make a commit. This folder might be auto updated when doing the gclient sync. 

online file at: https://source.chromium.org/chromium/chromium/src/+/main:build/config/win/BUILD.gn;l=173?q=use_large_pdbs

 ```
config("static_crt") {
  if (is_debug) {
    # This pulls in the static debug CRT and defines _DEBUG
    cflags = [ "/MTd" ]
  } else {
    cflags = [ "/MT" ]
  }
}
```
Update following section to produce the desired lib
```
config("default_crt") {
  if (is_component_build) {
    # Component mode: dynamic CRT. Since the library is shared, it requires
    # exceptions or will give errors about things not matching, so keep
    # exceptions on.
    configs = [ ":dynamic_crt" ]
  } else {
    if (current_os == "winuwp") {
      # The Great C Runtime (CRT) Refactoring - C++ Team Blog 
      # contains a details explanation of what is happening with the Windows
      # CRT in Visual Studio releases related to Windows store applications.
      configs = [ ":dynamic_crt" ]
    } else {
      # Desktop Windows: static CRT.

      # Hank: change from “:static_crt” to  ":dynamic_crt" 
      configs = [ ":dynamic_crt" ]
    }
  }
} 
```
When build sample app, will still have lots of link error, just added following libs to the link input file: Gdi32.lib, Winmm.lib, Secur32.lib  

it also cannot find where is comsuppd.lib, add following to the “AdditionalLibraryDirectories“ section: D:\Program Files\Microsoft Visual Studio\2022\VC\Tools\MSVC\14.36.32532\lib\x64


6. Build webrtc with desptop capture support

Step 4 and 5 are required, after it is done, run following build commands:

```
gn gen --ide=vs2019 out\debug\win-x64 --filters=//:webrtc --args="is_component_build=false use_lld=false is_debug=true is_clang=false rtc_build_examples=false rtc_build_tools=false rtc_include_tests=false target_cpu=\"x64\" target_os=\"win\" treat_warnings_as_errors=false use_custom_libcxx=false use_rtti=true rtc_win_video_capture_winrt=true rtc_build_examples=false rtc_win_use_mf_h264=true enable_libaom=false rtc_enable_protobuf=false"

ninja -C out\debug\win-x64

gn clean out\debug\win-x64
```


To build  release version

```
gn gen --ide=vs2019 out\release\win-x64 --filters=//:webrtc --args="is_component_build=false use_lld=false is_debug=false is_clang=false rtc_build_examples=false rtc_build_tools=false rtc_include_tests=false target_cpu=\"x64\" target_os=\"win\" treat_warnings_as_errors=false use_custom_libcxx=false use_rtti=true rtc_win_video_capture_winrt=true rtc_build_examples=false rtc_win_use_mf_h264=true enable_libaom=false rtc_enable_protobuf=false"

ninja -C out\release\win-x64

gn clean out\win\Release\x64
```
### Install Android app

adb install "D:\git\WeiStreaming\android\AppRTCDemo\app\build\outputs\apk\debug\app-debug.apk"

### Current Status

1. Able to Doing screen and camera capture
2. Handling video frame scalling faily well:  different rotate from android side are handled well.

### Next steps

1. Hangup and cleanup. remote side hangup, local hangup
2. Adding screen capture to existing call that already has camera capture.
3. When starting screen capture, should prompt dialog and ask user to pick up which desktop to share.
4. Handle more than one peer connection's case










