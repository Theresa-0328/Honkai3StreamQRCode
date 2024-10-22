﻿#include "VideoProcessor.h"

VideoProcessor::VideoProcessor()
{
	//av_log_set_level(AV_LOG_FATAL);
	avformatContext = avformat_alloc_context();
}

VideoProcessor::~VideoProcessor()
{
	if (avformatContext != avformat_alloc_context())
	{
		avformat_free_context(avformatContext);
		avformatContext = nullptr;
		avcodec_free_context(&avCodecContext);
		avCodecContext = nullptr;
	}
}

int VideoProcessor::OpenVideo(const std::string& path)
{
	if (avformatContext == nullptr)
	{
		return -1;
	}

	if (avformat_open_input(&avformatContext, path.c_str(), NULL, NULL) != 0)
	{
		std::cerr << "Error opening the file" << std::endl;
		return -1;
	}

	if (avformat_find_stream_info(avformatContext, NULL) < 0)
	{
		std::cerr << "Error finding the stream information" << std::endl;
		return -1;
	}

	index = GetStreamIndex(AVMEDIA_TYPE_VIDEO);
	FFmpegDecoder(index);
	OpenDecoder(index);
	buffer();
	swsctx(&swsCtx);
	avstream = avformatContext->streams[index];
	fps = av_q2d(avformatContext->streams[index]->r_frame_rate);
	return 0;
}

int VideoProcessor::Close()
{
	avformat_close_input(&avformatContext);
	return 0;
}

int VideoProcessor::read()
{
	av_read_frame(avformatContext, avPacket);
	return 0;
}

int VideoProcessor::GetStreamIndex(enum AVMediaType type)
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(4500));
	return av_find_best_stream(avformatContext, type, -1, -1, nullptr, 0);
}

int VideoProcessor::FFmpegDecoder(int i) //获得视频解码器
{
	//avCodecContext = avcodec_alloc_context3(nullptr);
	//if (avcodec_parameters_to_context(avCodecContext, avCodecParameters) < 0)
	//{
	//	printf("error");
	//}
	avCodecParameters = avformatContext->streams[i]->codecpar;
	avCodec = avcodec_find_decoder(avCodecParameters->codec_id);
	return 0;
}

int VideoProcessor::OpenDecoder(int i)// 打开解码器
{
	avCodecContext = avcodec_alloc_context3(avCodec);
	avcodec_parameters_to_context(avCodecContext, avformatContext->streams[i]->codecpar);
	return avcodec_open2(avCodecContext, avCodec, nullptr);
}

int VideoProcessor::SendPacket()
{
	if (&avPacket == nullptr)
	{
		return avcodec_send_packet(avCodecContext, nullptr);
	}
	return avcodec_send_packet(avCodecContext, avPacket);
}

int VideoProcessor::ReceiveFrame()
{
	avcodec_receive_frame(avCodecContext, avframe);
	return 0;
}

int VideoProcessor::buffer()
{
	int numBytes = av_image_get_buffer_size(AV_PIX_FMT_BGR24, avCodecContext->width, avCodecContext->height, 1);
	uint8_t* buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
	return av_image_fill_arrays(pFrameBGR->data, &pFrameBGR->linesize[0], buffer,
		AV_PIX_FMT_RGB24, avCodecContext->width, avCodecContext->height, 1);
}

int VideoProcessor::swsctx(struct SwsContext** swsCtx)
{
	*swsCtx = sws_getContext(avCodecContext->width, avCodecContext->height, avCodecContext->pix_fmt,
		avCodecContext->width / 1.5, avCodecContext->height / 1.5, AV_PIX_FMT_BGR24, SWS_BILINEAR, nullptr, nullptr, nullptr);
	return 0;
}