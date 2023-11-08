# Social Gaming

Build instructions:
git clone ...
cd social-gaming
git checkout develop
git submodule init
git submodule update
mkdir build
cd build
source /usr/shared/CMPT/faculty/wsumner/base/env373/bin/activate
cmake ..
make


Run Chatserver:
bin/socialserver 8000 ../tools/static/webchat.html

How to check rpsTest:
./bin/tree-sitter-demo /home/bde4/social-gaming/tools/tree-sitter-demo/rpsTest.txt