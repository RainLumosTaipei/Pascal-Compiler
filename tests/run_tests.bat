@echo off
setlocal enabledelayedexpansion


set PASCC=.\PASCC.exe
set TEST_DIR=test_cases
set OUTPUT_DIR=output
set LOG_FILE=test_report.log


if exist "%OUTPUT_DIR%" rmdir /s /q "%OUTPUT_DIR%"
if exist "%LOG_FILE%" del "%LOG_FILE%"
mkdir "%OUTPUT_DIR%"


for /d %%i in ("%TEST_DIR%\*") do (
    set "CASE_NAME=%%~nxi"
    set "INPUT_PAS=%%i\input.pas"
    set "EXPECTED_OUT=%%i\expected.txt"
    set "OUTPUT_C=%OUTPUT_DIR%\!CASE_NAME!.c"
    set "OUTPUT_EXE=%OUTPUT_DIR%\!CASE_NAME!"
    set "ACTUAL_OUT=%OUTPUT_DIR%\!CASE_NAME!.actual.txt"

    echo === Testing !CASE_NAME! === >> "%LOG_FILE%"


    "%PASCC%" -i "!INPUT_PAS!" -o "!OUTPUT_C!" 2>> "%LOG_FILE%"
    if !errorlevel! neq 0 (
        echo ❌ !CASE_NAME!: Pascal to C failed >> "%LOG_FILE%"
        goto :continue_case
    )


    gcc "!OUTPUT_C!" -o "!OUTPUT_EXE!" -lm 2>> "%LOG_FILE%"
    if !errorlevel! neq 0 (
        echo ❌ !CASE_NAME!: C compilation failed >> "%LOG_FILE%"
        goto :continue_case
    )


    "!OUTPUT_EXE!" > "!ACTUAL_OUT!" 2>&1


    fc /n /lb1 /w "!EXPECTED_OUT!" "!ACTUAL_OUT!" >nul 2>&1
    if !errorlevel! equ 0 (
        echo ✅ !CASE_NAME!: Passed >> "%LOG_FILE%"
    ) else (
        echo ❌ !CASE_NAME!: Output mismatch >> "%LOG_FILE%"
        echo --- Expected --- >> "%LOG_FILE%"
        type "!EXPECTED_OUT!" >> "%LOG_FILE%"
        echo --- Actual --- >> "%LOG_FILE%"
        type "!ACTUAL_OUT!" >> "%LOG_FILE%"
    )

    :continue_case
)


echo === Test Summary === >> "%LOG_FILE%"
find /c "✅" "%LOG_FILE%" | findstr /r "[0-9]" | for /f "tokens=3" %%a in ('more') do echo Passed: %%a >> "%LOG_FILE%"
find /c "❌" "%LOG_FILE%" | findstr /r "[0-9]" | for /f "tokens=3" %%a in ('more') do echo Failed: %%a >> "%LOG_FILE%"


type "%LOG_FILE%"