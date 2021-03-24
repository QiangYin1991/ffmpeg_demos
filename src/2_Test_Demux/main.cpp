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

	//初始化封装库
	av_register_all();
	//初始化网络库（rtmp, rtsp, http流媒体格式）
	avformat_network_init();

	std::string url = "..\\..\\Titanic.ts";
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

	rc = avformat_find_stream_info(ptrFormatContext, NULL);
	if (rc)
	{
		char buf[1024]{ 0 };
		av_strerror(rc, buf, sizeof(buf));
		std::cout << "find stream info failed, error code: " << buf << std::endl;
	}
	else
	{
		int totalMs = ptrFormatContext->duration / (AV_TIME_BASE / 1000);
		std::cout << "totalMs: " << totalMs << std::endl;
	}
	av_dump_format(ptrFormatContext, 0, nullptr, 0);

	// 获取音视频流信息
	for (int i = 0; i < ptrFormatContext->nb_streams; i++)
	{
		AVStream *ptrStream = ptrFormatContext->streams[i];
		if (ptrStream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			std::cout << "第" << i << "个流是音频流" << std::endl;
		}
		else if (ptrStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			std::cout << "第" << i << "个流是视频流" << std::endl;
			std::cout << "宽：" << ptrStream->codecpar->width << std::endl;
			std::cout << "高：" << ptrStream->codecpar->height << std::endl;
		}
	}

	if (ptrFormatContext)
	{
		// 释放封装上下文，并设置为NULL
		avformat_close_input(&ptrFormatContext);
	}
	getchar();
	return 0;
}