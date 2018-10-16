#!/bin/bash

# use this file to replace SDKROOT on all the Xcode projects. Rationale: from
# macosx10.12 onwards, QtKit is no longer supported, and #include
# <QuickTime/QuickTime.h> will fail all over the place
# This allows you to quickly set the SDK for all the projects in here.

[ -z $SDKROOT ] && SDKROOT=$1 # e.g.: macosx10.11 or earlier, download from https://github.com/phracker/MacOSX-SDKs/releases, unzip and place in /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs
[ -z $SET ] && SED=gsed # GNU sed, install with macports

for p in *;
do
	FILE=$p/*.xcodeproj/project.pbxproj
	[ -f $FILE ] || continue
	grep -q SDKROOT $FILE &&\
		$SED -i "s/\(.*SDKROOT=\).*/\1$SDKROOT;/" $FILE||\
		$SED -i "s/\(.*\)\(OTHER_LDFLAGS.*\)/\1\2\n\1SDKROOT=$SDKROOT;/" $FILE
done


