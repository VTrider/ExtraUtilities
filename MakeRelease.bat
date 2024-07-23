@echo off

7z.exe a -tzip ".\Extra Utilities vX.X.X.zip" ".\Release\exu.dll" ".\wrapper\ExtraUtils.lua"

echo:
echo Successfuly built release!

pause