@echo off

cd ..\webrtc

set origin=%cd%\src
set target=%cd%\..\WeiRtcWindowsNative\libwebrtc

echo Origin: %origin%
echo Target: %target%

cd %origin%
call :strlen originlength origin
call :treeProcess
cd %target%\..
goto :eof

REM ********* function *****************************
:treeProcess
set current=%cd%
echo Current: %current%
call set relative=%%current:~%originlength%%%
setlocal EnableDelayedExpansion
(
    for %%f in (*.h) do (
        set file=%%f
        if "!file:~-5!" neq ".pb.h" (
            set originpath=!origin!!relative!\!file!
            set targetpath=!target!!relative!\!file!
            echo Copying !originpath! to !targetpath!.
            echo f|xcopy /q /y !originpath! !targetpath!
        )
    )
)
(
    for %%f in (*.lib) do (
        set file=%%f
        if "!file!" equ "webrtc.lib" (
            set originpath=!origin!!relative!\!file!
            set targetpath=!target!!relative!\!file!
            echo Copying !originpath! to !targetpath!.
            echo f|xcopy /q /y !originpath! !targetpath!
        )
    )
)
endlocal
for /D %%d in (*) do (
    cd %%d
    call :treeProcess
    cd ..
)
exit /b

REM ********* function *****************************
:strlen <resultVar> <stringVar>
(
    setlocal EnableDelayedExpansion
    (set^ tmp=!%~2!)
    if defined tmp (
        set "len=1"
        for %%P in (4096 2048 1024 512 256 128 64 32 16 8 4 2 1) do (
            if "!tmp:~%%P,1!" NEQ "" ( 
                set /a "len+=%%P"
                set "tmp=!tmp:~%%P!"
            )
        )
    ) else (
        set len=0
    )
)
( 
    endlocal
    set "%~1=%len%"
    exit /b
)