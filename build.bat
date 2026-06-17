@echo off
for /f "usebackq tokens=*" %%i in (`pkg-config --cflags --libs gtk+-3.0`) do gcc main.c commands.c -o termulate.exe %%i
echo Build complete.