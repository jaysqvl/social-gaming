#! /bin/sh
# Makes an instance of socialserver on port 8000
mkdir -p ../build;
cd ../build;
cmake ../social-gaming;
make;
echo "Creating instance of socialserver on port 8000";
bin/socialserver 8000 static/html/webchat.html;