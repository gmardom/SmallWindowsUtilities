@echo off
setlocal EnableDelayedExpansion

set SRC_DIR=source
set BLD_DIR=build
set INS_DIR=%LOCAL_BIN%

set COMMON_FLAGS=/nologo /W3 /MD

set _subcmd=%~1
if "!_subcmd!"=="clean" goto clean
if "!_subcmd!"=="install" goto install

@rem ===========================================================================

where /q cl || (
  echo Error: "cl.exe" not found.
  exit /b 1
)

if "%Platform%" neq "x64" (
  echo Error: Platform is not "x64".
  exit /b 1
)

@rem ===========================================================================

@rem Create build folder if it does not exist.
if not exist "!BLD_DIR!" mkdir "!BLD_DIR!"

pushd build

call :f_compile wlp.c

popd
goto end

:f_compile
  set file_name=%~1
  set file_path=..\src\%file_name%
  echo Compiling: %file_name%
  cl %COMMON_FLAGS% %file_path% >nul 2>nul
  exit /b 0

:clean
  echo Cleaning !BLD_DIR!
  del /q "!BLD_DIR!\*"
  rmdir /q "!BLD_DIR!"
  goto

:install
  if not exist %INS_DIR% (
    echo ERROR: Install dir "!INS_DIR!" doesn't exist.
    exit /b 1
  )

  for /r %%f in (%BLD_DIR%\*.exe) do (
    set file_name=%%~nf
    echo Installing: !file_name!.exe
    copy /b/v/y "%%f" "!INS_DIR!\!file_name!.exe" >nul 2>nul
    if not %ERRORLEVEL%==0 (
      echo ERROR: Could not install '!file_name!.exe' to !INS_DIR!
      exit /b 1
    )
  )
  
  goto end

:end
echo.
echo Finished!
endlocal
