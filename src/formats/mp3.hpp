#ifndef DECODER_MP3_H
#define DECODER_MP3_H

#include "player.hpp"

class Mp3Decoder : public Decoder {
	public:
		Mp3Decoder(FileTransport *ftrans);
		
		~Mp3Decoder(void);
		
		void Info(metaInfo_t* Meta) override;
		
		uint32_t Position(void) override;
		
		uint32_t Length(void) override;
		
		void Seek(uint32_t location) override;
		
		uint32_t Decode(void* buffer) override;
		
		uint32_t Samplerate(void) override;
		
		uint32_t Buffsize(void) override;
		
		int Channels(void) override;
};

//bool isMp3(const std::string& filename);

#endif
