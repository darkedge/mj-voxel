@ECHO off

REM The tilde(~) removes quotes from parameters

REM DO NOT INSERT SPACES
SET Configuration=%1
SET Platform=%2
SET SolutionDir=%~3
SET ProjectDir=%~4
SET TargetDir=%~5

REM $(SolutionDir) and $(TargetDir) have a trailing backslash
REM and are contained in quotes

REM ~d is drive
REM ~p is the path (without drive). ~dp is drive+path.
REM ~n is the file name without the extension. 
REM ~x is the extension
REM ~t is the timestamp
REM ~z is the size

REM Copy DLL's
IF "%Configuration%" == "Debug" (
	IF "%Platform%" == "Win32" (
		XCOPY /Y "%SolutionDir%glfw\x86\glfw3.dll" %TargetDir%
		XCOPY /Y "%SolutionDir%lua\x86\lua51.dll" %TargetDir%
	) ELSE IF "%Platform%" == "x64"	(
		XCOPY /Y "%SolutionDir%glfw\x64\glfw3.dll" %TargetDir%
		XCOPY /Y "%SolutionDir%lua\x64\lua51.dll" %TargetDir%
	) ELSE (
		ECHO Unknown platform!
	)
) ELSE IF "%Configuration%" == "Release" (
	IF "%Platform%" == "Win32" (
		XCOPY /Y "%SolutionDir%glfw\x86\glfw3.dll" %TargetDir%
		XCOPY /Y "%SolutionDir%lua\x86\lua51.dll" %TargetDir%
	) ELSE IF "%Platform%" == "x64" (
		XCOPY /Y "%SolutionDir%glfw\x64\glfw3.dll" %TargetDir%
		XCOPY /Y "%SolutionDir%lua\x64\lua51.dll" %TargetDir%
	) ELSE (
		ECHO Unknown platform!
	)
) ELSE (
	ECHO Unknown configuration!
)

REM Copy Lua, shader, image files
XCOPY /Y "%ProjectDir%*.lua" %TargetDir%
XCOPY /Y "%ProjectDir%*.frag" %TargetDir%
XCOPY /Y "%ProjectDir%*.vert" %TargetDir%
XCOPY /Y "%ProjectDir%*.jpg" %TargetDir%
XCOPY /Y "%ProjectDir%*.png" %TargetDir%
