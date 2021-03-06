#ifndef DECODER_MIDI_H
#define DECODER_MIDI_H

#include "player.hpp"

class MidiDecoder : public Decoder {
	public:
		MidiDecoder(const char* filename, const char* midicfg);
		
		~MidiDecoder(void);
		
		void Info(metaInfo_t* Meta) override;
		
		uint32_t Position(void) override;
		
		uint32_t Length(void) override;
		
		void Seek(uint32_t location) override;
		
		uint32_t Decode(void* buffer) override;
		
		uint32_t Samplerate(void) override;
		
		uint32_t Buffsize(void) override;
		
		int Channels(void) override;
};

#endif
