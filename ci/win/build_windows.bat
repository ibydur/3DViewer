@echo off

set errorlevel=0

rem Git
where git > NUL
if %errorlevel% equ 0 (
  echo 1. Found Git
) else (
  echo 1. Unable to find Git!
  goto the_end
)
rem CMake
where cmake > NUL
if %errorlevel% equ 0 (
  echo 2. Found CMake
) else (
  echo 2. Unable to find CMake!
  goto the_end
)

cd /d ../.. 
set "SCRIPTDIR=%~dp0"
set "PROJECTDIR=%cd%\"
set "TRIPLET=x64-windows"

if not exist vcpkg (
  git clone https://github.com/Microsoft/vcpkg.git "%PROJECTDIR%\"
  call vcpkg\bootstrap-vcpkg.bat
) else (
  echo 3. Found vcpkg
  pushd vcpkg
  git pull
  call bootstrap-vcpkg.bat
  popd
)

set "VCPKGDIR=%PROJECTDIR%\vcpkg"
set "VCPKG=%VCPKGDIR%\vcpkg.exe"
"%VCPKG%" install --triplet=%TRIPLET% qt5 cgal

mkdir "%PROJECTDIR%\build" 2> NUL
pushd "%PROJECTDIR%\build"
set "BUILD_DIR=%PROJECTDIR%\build"

cmake "%PROJECTDIR%"
if "%errorlevel%" == "0" (
  cmake --build . --parallel 4 --config Release %*
  if exist %PROJECTDIR%\x64 (
  rmdir /s /q "%PROJECTDIR%\x64"
  )
)
popd

:the_end
pushd %SCRIPTDIR%
