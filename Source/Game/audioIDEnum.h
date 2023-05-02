#ifndef AUDIO_ID_ENUM_H
#define AUDIO_ID_ENUM_H

namespace AudioID {
	constexpr unsigned short hit_buffer = 17;
	enum {
		theme = 0,
		player_hit = 1,
		sad_violin = 2 + hit_buffer,
		bot_hit = 3 + hit_buffer,
		click = 4 + hit_buffer*2,
		defeated = 5 + hit_buffer*2,
	};// enum AudioID

}

#endif
