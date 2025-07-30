#!/bin/bash
echo "Build case $1"

echo "Compile array.c";  gcc array.c -c
echo "Compile image_processing.c";  gcc image_processing.c -c 
echo "Compile main.c";  gcc main.c -c 

if [ "$1" = "skip" ] ; then
    echo "Exclude logo_image_lvds.c"
else
    echo "Compile logo_image_lvds.c";  gcc logo_image_lvds.c -c 
fi

echo "gcc *.o -o image_converter"
gcc *.o -o image_converter

echo "rm *.o"
rm *.o



# gcc main.c array.c image_processing.c -o image_converter
