//
// Created by rodo on 2020-01-26.
//

#ifndef GLES3JNI_LONESCREENKEY_H
#define GLES3JNI_LONESCREENKEY_H

#include <vector>
namespace  olc {
	class PixelGameEngine;
	struct Pixel;
}

#define NO_KEYZ -1

typedef struct sScreenKey {
	int code;
	int x0,y0,x1,y1;
} ScreenKey_t;

class LoneScreenKey {
	std::vector<ScreenKey_t> vKeys;
	float fResolution = 1;
public:
	LoneScreenKey(int resolution);
	static LoneScreenKey *currentInstance;
	int input(int x, int y);
	int sync(int x0, int y0, int x1, int y1, bool keystats[], bool isUp = false);
	void add(ScreenKey_t key);
	void DrawSelf(olc::PixelGameEngine *engine, olc::Pixel color);
};

#endif //GLES3JNI_LONESCREENKEY_H
