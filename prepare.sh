#!/bin/bash

set -e

echo
echo Preparing easysqlite...
echo

preparelink()
{
	if [ ! -d "$1" ]; then
		echo ERROR: Path to link does not exist \"$1\" !
	fi
	pushd $1 > /dev/null
	if [ ! -d "$3" ]; then
		echo ERROR: Link destination is not found \"$3\" inside \"$1\" !
		popd > /dev/null
		exit -1
	fi
	if [ ! -h "$2" ]; then
		echo
		echo In path \"$1\" creating symbolic link \"$2\" pointing to \"$3\"...
		echo
		ln -s $3 $2
		if [ $? -ne 0 ]; then
			failure=$?
			echo Faield to create symbolic link
			popd > /dev/null
			exit $failure
		fi
	fi
	popd > /dev/null
}

filename=sqlite-amalgamation-3080803
download_file=http://www.sqlite.org/2015/$filename.zip
output=$filename.zip
uncompressed_path=$filename
expecting_hash=97604645c615d81194541e1398687b61



if [ ! -e $output ]; then
	echo Downloading $download_file ...

	curl -o $output $download_file
fi

if [ ! -e $output ]; then
	echo ERROR: Failed to download $download_file
	exit -1
fi

calc_hash="`md5 -q $output`"

if [ \"$calc_hash\" != \"$expecting_hash\" ]; then
	echo ERROR: Expecting $expecting_hash hash does not match calculated hash $calc_hash
	exit -1
fi

echo
echo Uncompressing $output ...
echo
unzip -n $output

preparelink "." "sqlite" "$uncompressed_path"

echo
echo easysqlite ready.
echo
