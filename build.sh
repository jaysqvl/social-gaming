#! /bin/sh
# Script that make it simpler to build
mkdir -p ../build;
cd ../build;
cmake ../social-gaming;
make;
echo "Finished executing build.sh"