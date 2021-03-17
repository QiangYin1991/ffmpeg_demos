#include <iostream>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
int main()
{
	std::cout << "FFmpeg Configuration:" << std::endl;
	std::cout << avcodec_configuration() << std::endl;

	//��ʼ����װ��
	av_register_all();
	//��ʼ������⣨rtmp, rtsp, http��ý���ʽ��
	avformat_network_init();

	std::string url = "Titanic.ts";
	AVFormatContext *ptrFormatContext = nullptr;

	int rc = avformat_open_input(&ptrFormatContext, url.c_str(), nullptr, nullptr);
	if (rc)
	{
		char buf[1024]{ 0 };
		av_strerror(rc, buf, sizeof(buf));
		std::cout << "Demux failed, error code: " << buf << std::endl;
	}
	else
	{
		std::cout << "Demux success" << std::endl;
	}
	getchar();
	return 0;
}