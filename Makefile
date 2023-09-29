AT_SDK=/opt/at-solutionpackage/cxSDK
ARCH=linux_amd64

default:
	g++ -ggdb3 -fuse-ld=gold -lcx3dLib  -lcxBaseLib -L$(AT_SDK)/lib/$(ARCH) -I$(AT_SDK)/include -Wl,-rpath,$(AT_SDK)/lib/$(ARCH) -std=c++17 `pkg-config --libs --cflags opencv4` cx-sdk-cloud.cpp -o cx-sdk-cloud
