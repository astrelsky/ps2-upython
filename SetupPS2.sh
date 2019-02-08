#! /bin/sh

#Copied from ps2toolchain.
##Copied from ps2sdk-ports/freetype2/SetupPS2.sh
## Check if $PS2DEV is set.
if test ! $PS2DEV; then { echo "ERROR: Set \$PS2DEV before continuing."; exit 1; } fi

 ## Check if $PS2SDK is set.
if test ! $PS2SDK; then { echo "ERROR: Set \$PS2SDK before continuing."; exit 1; } fi

## Determine GNU Make command.
if command -v gmake >/dev/null; then
	GNUMAKE=gmake
else
	GNUMAKE=make
fi

cd micropython/mpy-cross

echo "Building mpy-cross..."
$GNUMAKE > /dev/null 2>&1

## Install
echo "Installing mpy-cross..."
`cp -f mpy-cross $PS2DEV/bin` > /dev/null 2>&1

## Cleanup
echo "Cleaning up mpy-cross build..."
$GNUMAKE clean > /dev/null 2>&1

cd ../..

## Build Required Object Files
echo "Building Micropython..."
$GNUMAKE > /dev/null 2>&1

## Install Required Objects
echo "Installing Micropython..."
$GNUMAKE install > /dev/null 2>&1

## Post-installation cleanup.
echo "Cleaning up..."
$GNUMAKE fullclean > /dev/null 2>&1

echo "Micropython built and installed."

