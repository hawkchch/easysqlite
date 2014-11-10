easysqlite
==========

C++ wrapper around sqlite database from https://code.google.com/p/easysqlite/

The project files assume the amalgamation files `sqlite3.c` and `sqlite3.h` have been placed into the `easysqlite/sqlite/` path.

How to compile on iOS / Mac from scratch:

1) Clone `sqlite` and `easysqlite` into the same directory

````
git clone https://github.com/openpeer/sqlite.git -b 20140401-master
git clone https://github.com/openpeer/easysqlite.git -b 20140401-master
````

2) Build the amalgamation files

````
cd sqlite
sh sqlite.sh
cd ..
````


3) Copy the amalgamation files into easysql

````
mkdir -p "easysqlite/sqlite"
cp "sqlite/build/sqlite3.c" "easysqlite/sqlite/sqlite3.c"
cp "sqlite/build/sqlite3.h" "easysqlite/sqlite/sqlite3.h"
````

4) Compile `projects/xcode/easysqlite_ios.xcodeproj` for iOS
5) Compile `projects/xcode/easysqlite.xcodeproj` for Mac


NOTE: Alternatively to step 1, 2, and 3 the amalgamation files can be downloaded directly from http://www.sqlite.org/ and placed into a path named `easysqlite/sqlite/`.
