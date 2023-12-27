
set UE4VER=%1
set PLATFORM=%2
set BUILDTYPE=%3
set ZIPNAME=../../../../_zip/%5
::set PROJECTNAMEEDITOR="MyProjectBuildScriptEtidor"
set PROJECTNAMEEDITOR=%4

set UE4BASE=D:\Program Files\Epic Games
set UPROJECT="C:\Users\ruyo\Documents\Unreal Projects\MyProjectBuildScript\MyProjectBuildScript.uproject"
set UNREALVERSIONSELECTOR="C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe"


powershell -ExecutionPolicy RemoteSigned .\delIntermediate.ps1

::cd ../../../Plugins

git reset HEAD ./
git checkout ./

::cd VRM4U/Source/ReleaseScript 

powershell -ExecutionPolicy RemoteSigned .\version.ps1 \"%UE4VER%\"


set UE4PATH=UE_%UE4VER%

set CLEAN="%UE4BASE%\%UE4PATH%\Engine\Build\BatchFiles\Clean.bat"
set BUILD="%UE4BASE%\%UE4PATH%\Engine\Build\BatchFiles\Build.bat"
set REBUILD="%UE4BASE%\%UE4PATH%\Engine\Build\BatchFiles\Rebuild.bat"
set PROJECTNAME="../../../../MyProjectBuildScript.uproject"


if %UE4VER% == 5.0EA (
  call "D:\Program Files\Epic Games\UE_5.0EA\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project=%UPROJECT% -game -rocket -progress
) else (
  call %UNREALVERSIONSELECTOR% /projectfiles %UPROJECT%
)

if not %errorlevel% == 0 (
    echo [ERROR] :P
    goto err
)

if %PLATFORM% == Android (
    echo android
) else (
    echo not android
    call %CLEAN% %PROJECTNAMEEDITOR% %PLATFORM% %BUILDTYPE% %UPROJECT% -waitmutex
)

set tmpOldFlag=FALSE
if %UE4VER% == 4.21 set tmpOldFlag=TRUE
if %UE4VER% == 4.20 set tmpOldFlag=TRUE
if %UE4VER% == 4.19 set tmpOldFlag=TRUE

if %tmpOldFlag% == TRUE (
del "..\..\..\VRM4U\Source\VRM4U\Private\VRM4U_AnimSubsystem.cpp"
del "..\..\..\VRM4U\Source\VRM4U\Public\VRM4U_AnimSubsystem.h"
)


set /a UEVersion=%UE4VER%
for /f %%i in ('wsl echo "%UEVersion% * 100"') do set UEVersion100=%%i
for /f "tokens=1 delims=." %%a in ("%UEVersion%") do set UEMajorVersion=%%a


set isUE4=FALSE
if %UEMajorVersion% == 4 (
    set isUE4=TRUE
)

if %isUE4% == FALSE (
del "..\..\..\VRM4U\Content\Util\Actor\latest\WBP_MorphTarget.uasset"
)

set isRetargeterEnable=TRUE
if %UEMajorVersion% == 4 (
    set isRetargeterEnable=FALSE
)
if %UE4VER% == 5.0 set isRetargeterEnable=FALSE
if %UE4VER% == 5.1 set isRetargeterEnable=FALSE
if %UE4VER% == 5.2 set isRetargeterEnable=FALSE

if %isRetargeterEnable% == FALSE (
del "..\..\..\VRM4U\Source\VRM4U\Private\VrmAnimInstanceRetargetFromMannequin.cpp"
del "..\..\..\VRM4U\Source\VRM4U\Public\VrmAnimInstanceRetargetFromMannequin.h"
)



call %BUILD% %PROJECTNAMEEDITOR%       %PLATFORM% %BUILDTYPE% %UPROJECT% -waitmutex

if not %errorlevel% == 0 (
    echo [ERROR] :P
    goto err
)
:: releasetool

::cd ../Plugins

::del *.lib /s


::cd ../releasetool

powershell -ExecutionPolicy RemoteSigned .\compress.ps1 %ZIPNAME%



:finish
exit /b 0

:err
exit /b 1


