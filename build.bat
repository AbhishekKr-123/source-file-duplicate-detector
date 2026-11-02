@echo off
gcc -Wall -Wextra -std=c11 -O2 -o duplicate_detector.exe src\main.c src\file_manager.c src\file_reader.c src\normalizer.c src\tokenizer.c src\comparator.c src\similarity.c src\report.c
if %errorlevel%==0 echo Build successful: duplicate_detector.exe
