echo off
setlocal enableextensions
setlocal enabledelayedexpansion

set test=false

echo Executing %0 %*

rem Default values
set platform=x86
set packageroot=.\

:processargs
if (%1)==() goto :start
call :setarg %1
shift
goto :processargs

:setarg
setlocal enabledelayedexpansion
set arg=%~1
for /f "delims== tokens=1,2" %%a in ("%arg%") do (
set key=%%a
set val=%%b
)
endlocal & set %key%=%val%
goto :eof

:start
rem Check required arguments
if ("%file%")==("") goto :usage

rem Clean up arguments ready for use
call :stripquotes file
call :prepdir packageroot
call :prepdir relativepath
call :stripquotes platform
call :stripquotes filelist

rem Set defaults if necessary
if not ("%platform%") == ("x86") if not ("%platform%") == ("x64") set platform=%defaultplatform%
set packageroot=%packageroot%%platform%\

if (%test%) == (true) (
echo file: "%file%"
echo packageroot: "%packageroot%"
echo relativepath: "%relativepath%"
echo platform: "%platform%"
echo filelist: "%filelist%"
)

rem Find Buzz
call :getbuzzroot buzzroot

if (%test%) == (true) echo buzzroot: %buzzroot%

rem Set destination folders
set packagepath=%packageroot%%relativepath%
set buzzpath=%buzzroot%%relativepath%

if (%test%) == (true) (
echo packagepath: %packagepath%
echo buzzpath: %buzzpath%
)

rem Copy file to package folder
call :makepackage
call :processfilelist packageroot

rem Copy file to buzz path if it was found
if not ("%buzzroot%") == ("") (
call :buzzcopy
call :processfilelist buzzroot
)

goto :end

:makepackage
echo Copying to "%packagepath%"
set command=if not exist "%packagepath%" mkdir "%packagepath%"
if (%test%) == (true) ( echo command: %command% ) else ( %command% )
set command=copy "%file%" "%packagepath%"
if (%test%) == (true) ( echo command: %command% ) else ( %command% )
goto :eof


:buzzcopy
echo Copying to "%buzzpath%"
set command=copy "%file%" "%buzzpath%"
if (%test%) == (true) ( echo command: %command% ) else ( %command% )
goto :eof

:getbuzzroot
setlocal enabledelayedexpansion

rem Decide which registry value we want
if ("%platform%") == ("x86") (
set value=BuzzPath32
) else if ("%platform%") == ("x64") (
set value=BuzzPath64
) else (
set v=
endlocal & set %~1=%v%
goto :eof
)

rem Test if value is present
for /f "tokens=1" %%a in ('reg query HKCU\Software\Jeskola\Buzz\Settings') do if ("%%a") == ("%value%") set found=true

rem If path value was found, retrieve the path string. If not, bail.
if (%found%)==(true) (
for /f "skip=2 tokens=*" %%a in ('reg query HKCU\Software\Jeskola\Buzz\Settings /v %value%') do ( set str=%%a )
) else (
set v=
endlocal & set %~1=%v%
goto :eof
)

rem Convert four spaces to tab. Why does reg not use tabs?
set str=%str:    =	%

rem %str% should now be this: BuzzPath	REG_SZ	C:\Program Files\Jeskola Buzz\
rem Split %str% by tabs, extracting the third token.
for /f "tokens=3 delims=	" %%x in ("%str%") do ( set v=%%x )

rem Remove trailing spaces
call :rtrim v

rem Check we've got the right path, set return value to null if not
if not exist "%v%Buzz.exe" set v=

endlocal & set %~1=%v%
goto :eof

:processfilelist
setlocal enabledelayedexpansion
if exist "%filelist%" (
set root=!%1!
call :trim root
call :prepdir root
for /f "tokens=1,2 delims=@" %%a in (%filelist%) do (
set file=%%a
set dest=%%b
call :trim file
call :trim dest
call :prepdir dest
call :copyfile file dest root
)
) else echo %filelist% not found
goto :eof

:copyfile
setlocal enabledelayedexpansion
set file=!%1!
set dest=!%2!
set root=!%3!
call :trim file
call :trim dest
call :trim root
call :prepdir root
call :prepdir dest
set command=copy "%file%" "%root%%dest%"
if (%test%) == (true) ( echo command: %command% ) else ( %command% )
goto :eof

:stripquotes
setlocal enabledelayedexpansion
set v=!%1!
for /f "useback tokens=*" %%a in ('%v%') do set v=%%~a
endlocal & set %~1=%v%
goto :eof

:ltrim
setlocal enabledelayedexpansion
set v=!%1!
if ("%v%") == ("") (
endlocal & set %~1=%v%
goto :eof
)
if ("%v:~0,1%")==(" ") set v=%v:~1%
if ("%v:~0,1%")==(" ") call :ltrim v
endlocal & set %~1=%v%
goto :eof

:rtrim
setlocal enabledelayedexpansion
set v=!%1!
if ("%v%") == ("") (
endlocal & set %~1=%v%
goto :eof
)
if ("%v:~-1%")==(" ") set v=%v:~0,-1%
if ("%v:~-1%")==(" ") call :rtrim v
endlocal & set %~1=%v%
goto :eof

:trim
setlocal enabledelayedexpansion
set v=!%1!
if not ("%v%") == ("") (
call :ltrim v
call :rtrim v
)
endlocal & set %~1=%v%
goto :eof

:prepdir
setlocal enabledelayedexpansion
set v=!%1!
call :stripquotes v
if ("%v%") == ("") (
endlocal & set %~1=%v%
goto :eof
)
if not (%v:~-1%)==(\) set v=%v%\
endlocal & set %~1=%v%
goto :eof

:usage
echo Buzz machine post-build batch file version 1.1
echo Copies files to a release package folder and to the Buzz application folder.
echo Usage: postbuild.bat file <packageroot> <relativepath> <platform> <filelist>
echo Arguments are in the form key=value and should be enclosed in double quotes ie. "file=SomeFile.dll"
echo file argument is required, all others are optional:
echo "packageroot=path\" - Root folder where package will be assembled. Trailing backslash is not required.
echo "relativepath=path" - Subfolder which file will be copied to ie. "relativepath=Gear\Effects\"
echo "platform=value" - Values are x86 or x64. If no value is set, x86 will be used.
echo "filelist=filename" - File containing a list of additional files to be copied to the output folders. One entry per line in the form SomeFile@RelativePath. 
goto :end

:end
echo Finished postbuild.bat

