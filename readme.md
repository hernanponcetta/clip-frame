git submodule add https://github.com/monatis/clip.cpp.git
git submodule update --init --recursive

# pull latest commit for main submodule
git submodule update --remote --recursive


bear -- npx node-gyp configure build


https://dev.to/krowemoh/debugging-a-node-c-addon-3h12
