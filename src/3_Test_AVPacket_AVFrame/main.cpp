#include <iostream>
#include <thread>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")

double r2d(AVRational r)
{
	return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

int main()
{
	// init
	av_register_all();
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
	rc = avformat_find_stream_info(ptrFormatContext, nullptr);
	if (rc)
	{
		char buf[1024]{ 0 };
		av_strerror(rc, buf, sizeof(buf));
		std::cout << "find stream info failed, error code: " << buf << std::endl;
	}

	//
	int videoStream = av_find_best_stream(ptrFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	int audioStream = av_find_best_stream(ptrFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	AVPacket *packet = av_packet_alloc();
	do 
	{
		int rc = av_read_frame(ptrFormatContext, packet);
		if (rc)
		{
			std::cout << "======================cycle====================" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			int seek = 10; // 10s
			long long pos = seek * r2d(ptrFormatContext->streams[packet->stream_index]->time_base);
			av_seek_frame(ptrFormatContext, videoStream, pos, AVSEEK_FLAG_BACKWARD);
			continue;
		}
		std::cout << "packet size: " << packet->size << std::endl;
		std::cout << "packet pts: " << packet->pts << std::endl;
		std::cout << "packet pts ms: " << packet->pts * r2d(ptrFormatContext->streams[packet->stream_index]->time_base) * 1000 << std::endl;
		std::cout << "packet dts: " << packet->dts << std::endl;
		if (packet->stream_index == videoStream)
		{
			std::cout << "Í¼Ïñ" << std::endl;
		}
		else if (packet->stream_index == audioStream)
		{
			std::cout << "ÒôÆµ" << std::endl;
		}
		av_packet_unref(packet);
	} while (true);
	av_packet_free(&packet);

	// release
	if (ptrFormatContext)
	{
		avformat_close_input(&ptrFormatContext);
	}
	getchar();
	return 0;
}