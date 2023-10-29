@echo off

cd ..
set everythingfine=1
set errorlevel=0

rem Git
where git > NUL
if %errorlevel% equ 0 (
  echo 1. Found Git
) else (
  echo 1. Unable to find Git!
  set everythingfine=0
)
set errorlevel=0

rem CMake
where cmake > NUL
if %errorlevel% equ 0 (
  echo 2. Found CMake
) else (
  echo 2. Unable to find CMake!
  set everythingfine=0
)

set errorlevel=%everythingfine%

cd ci/