@echo off
title Campus Group Finder Server
echo Starting Campus Group Finder Web Server on http://localhost:8080 ...
powershell -ExecutionPolicy Bypass -File "%~dp0server.ps1"
pause
