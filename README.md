easysqlite
==========

C++ wrapper around sqlite database from https://code.google.com/p/easysqlite/

The code assume sqlite3.c sqlite3.h has been amalgamated into the ./sqlite/ path.

How to compile on iOS / Mac:

1) Clone into the same directory

git clone https://github.com/openpeer/sqlite.git
git clone https://github.com/openpeer/easysqlite.git

2) Build the amalgamation file

cd sqlite
sh sqlite.sh
cc ..

3) Copy the amalgamation file into easysql

mkdir -p "easysqlite/sqlite"
cp "sqlite/build/sqlite3.c" "easysqlite/sqlite/sqlite3.c"
cp "sqlite/build/sqlite3.h" "easysqlite/sqlite/sqlite3.h"

Compile projects/xcode/easysqlite_ios.xcodeproj for iOS
Compile projects/xcode/easysqlite.xcodeproj for Mac


NOTE: Alternatively to step 1 and 2, the amalgamation file can be downloaded directly from http://www.sqlite.org/ and placed into the easysqlite/sqlite/ path.
