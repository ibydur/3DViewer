@echo off

cd ..
set PROJECTDIR=%cd%\

set TRIPLET=x64-windows

call check.bat
if "%errorlevel%" == "0" (
  goto the_end
)
set errorlevel=

  if not exist vcpkg (
    git clone https://github.com/Microsoft/vcpkg.git
    call vcpkg\bootstrap-vcpkg.bat
  ) else (
    cd vcpkg
    git pull
    call bootstrap-vcpkg.bat
    cd ..
  )
  
  set VCPKGDIR=%cd%\vcpkg
  set VCPKG="%VCPKGDIR%\vcpkg.exe"

  %VCPKG% install --triplet=%TRIPLET% --overlay-ports=../vcpkg/ports --overlay-ports=../vcpkg/ports_windows ^
    qt5 ^
    cgal ^

popd

set errorlevel=
mkdir build 2> NUL
pushd build
  cmake ..
  if "%errorlevel%" == "0" (
    cmake --build . --target ALL_BUILD --parallel 4 --config Release %*
  )
popd

:the_end
pushd ci