#include <iostream>

extern "C" {
	#include "libavcodec/avcodec.h"
}

#pragma comment(lib, "avcodec.lib")

int main()
{
	std::cout << "Test FFmpeg Configure: " << std::endl;
	
	std::cout << avcodec_configuration();
	
	getchar();
	return 0;
}