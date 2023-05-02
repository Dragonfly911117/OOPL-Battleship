#ifndef AUDIO_ID_ENUM_H
#define AUDIO_ID_ENUM_H

namespace AudioID {
	// make the buffer half of the number of ships' grids
	// see if the buffer is enough to handle the hit sound
	constexpr unsigned short hit_buffer = 17 / 2;  
	enum {
		theme = 0,
		sad_violin = 1,
		player_hit = 2,
		bot_hit = 3 + hit_buffer,
		click = 4 + hit_buffer*2,
		defeated = 5 + hit_buffer*2,
		defeat_not_DSMode_Bot = 6 + hit_buffer*2,
		// defeat_normal = 7 + hit_buffer*2,
		// defeat_hard = 8 + hit_buffer*2,
		defeat_dark_soul = 7 + hit_buffer*2,
		Missed1 = 8 + hit_buffer*2,
		Missed2 = 9 + hit_buffer*2,
		Missed3 = 10 + hit_buffer*2,
		Missed4 = 11 + hit_buffer*2,
	};// enum AudioID

}

#endif
