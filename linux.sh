# git clone https://github.com/NVIDIA/DLSS.git

# sudo ln -s /usr/lib/gcc/x86_64-pc-linux-gnu/11.2.0/../../../../lib/libstdc++.a /usr/lib/gcc/x86_64-pc-linux-gnu/11.2.0/../../../../lib/liblibstdc++.a

cmake --build . --target clean
DLSS_SDK_PATH=/home/pew/dev/prboom-plus-rt/deps/DLSS CMAKE_INSTALL_PREFIX=../install cmake ..

make

