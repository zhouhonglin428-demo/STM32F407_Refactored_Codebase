@echo off
chcp 65001 >nul
set /p msg="请输入本次提交说明: "
git add .
git commit -m "%msg%"
git push
exit