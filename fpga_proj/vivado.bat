@echo off
setlocal EnableDelayedExpansion

set XILINX_VIVADO_DIR=C:\Xilinx\Vivado
set VIVADO_VERSION=2023.2
rem Scripts might work for other Vivado versions as well, but there is no guarantee.. The tcl script is only tested for Vivado 2023.2.
set VIVADO_SCRIPT_DIR=scripts\vivado\2023.2

rem add path to vivado bin dir; we use %VIVADO_VERSION% version:
if "%USERNAME%" == "JurgenGerritsen" set XILINX_VIVADO_DIR=D:\Tools\Xilinx\Vivado

REM Get the number of arguments and determine even or odd (modulo 2)
REM when even, user forgot to enter the target (only vivado_proj or all are supported
REM for now due to vivado.tcl not supporting more targets)

for %%x in (%*) do Set /A arg_count+=1

REM set /a odd_neven=%arg_count% %% 2
REM if %odd_neven% == 0 goto usage
REM Modulo operations on Windows 7 and Windows 10 do not match, use division and multiplication
REM then comparison instead.

set /a arg_count_div2=%arg_count% / 2
set /a arg_count_div2_mult2=%arg_count_div2% * 2
if "%arg_count_div2_mult2%" == "%arg_count%" goto usage

REM Parse key value input pairs

for %%a in (%*) do (
  if not defined varname (
    set varname=%%a
  ) else (
    set !varname!=%%a
    set "varname="
  )
  REM Automatically gets overwritten to the last value which is the target
  set target=%%a
)

set gendir="GEN_%board_name%"

if %target% == vivado_proj goto make_vivado_proj
if %target% == bit goto make_all
if %target% == all goto make_all
goto end

:make_vivado_proj
if exist %gendir% goto exists
if "%proj_name%" == "" goto usage
if "%board_name%" == "" goto usage
call %XILINX_VIVADO_DIR%\%VIVADO_VERSION%\bin\vivado.bat -mode batch -nojournal -nolog -source "%VIVADO_SCRIPT_DIR%\vivado.tcl" -notrace -tclargs %proj_name% %board_name% n n n n
goto end

:make_all
if exist %gendir% goto exists
if "%proj_name%" == "" goto usage
if "%board_name%" == "" goto usage
call %XILINX_VIVADO_DIR%\%VIVADO_VERSION%\bin\vivado.bat -mode batch -nojournal -source "%VIVADO_SCRIPT_DIR%\vivado.tcl" -notrace -tclargs %proj_name% %board_name% y y y y
goto end

:exists
echo ***********************************************************
echo ****                                                   ****
echo **** *****     %gendir% dir exists; cannot build ***** ****
echo ****                                                   ****
echo ***********************************************************
echo At least the %gendir% directory must be removed in order to run a new build.
echo "Removing %gendir% folders?"
@RD /S "%gendir%"
if exist %gendir% goto exists
if %target% == vivado_proj goto make_vivado_proj
if %target% == bit goto make_all
if %target% == all goto make_all
goto end

:usage
echo vivado.bat: build GEN dir with Vivado project
echo   usage:
echo vivado board_name=[board name] proj_name=[project name] [target]
echo   Possible targets are: vivado_proj, bit, all
echo   For example:
echo vivado board_name=basys3 proj_name=seven_seg_cntr vivado_proj
echo   -^> creates only the Vivado project for basys3 board, doesn't build anything
echo  or
echo vivado board_name=basys3_inv proj_name=seven_seg_cntr all
echo   -^> creates the Vivado project for basys3_inv board, and builds complete bitfile.
echo.
echo NOTE: GEN dir should not exist at start when wanting to build new bitfile!
echo.
pause

:end
echo "Finished!"
