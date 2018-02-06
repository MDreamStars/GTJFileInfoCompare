for /r "%cd%" %%i in (*.exe) do (   
E:\Tool\QT5.5.1\5.5\msvc2010\bin\windeployqt.exe "%%~nxi"
)
pause