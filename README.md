# nolan_clock


dylibbundler -od -b -x ./NolanClock.app/Contents/MacOS/NolanClock -d ./NolanClock.app/Contents/libs/
codesign -f -d -s "Apple Development: anthonypate@gmail.com" NolanClock.app