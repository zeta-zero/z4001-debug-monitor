@echo off

rem 要删除的目录
set "dir=build\"

rd /Q /S "build\"
del /Q /S "build\*.*"


rem 提示信息
echo "已删除目录 %dir% 中除排除文件以外的所有文件"