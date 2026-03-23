#pragma once
#include<Fwk/Graphics.h>
#include "Lib/Math/Vector.h"
using namespace Lib::Math;

struct Background {
	Sprite sprite;
	Vector2f position;
	float scrollRate;
	int textureIndex;
};

constexpr int NUM_BACKGROUND = 13;

enum class Images {
	BG1,
	BG2,
	BG3,
	BG4,
	BG5,
	BG6,
	BG7,
	BG8,
	BG9,
	BG10,
	BG11,
	BG12,
	BG13,
	Count
};

const string imageFiles[(int)Images::Count] = {
	"Images/2dAction/BG1.png",
	"Images/2dAction/BG2.png",
	"Images/2dAction/BG3.png",
	"Images/2dAction/BG4.png",
	"Images/2dAction/BG5.png",
	"Images/2dAction/BG6.png",
	"Images/2dAction/BG7.png",
	"Images/2dAction/BG8.png",
	"Images/2dAction/BG9.png",
	"Images/2dAction/BG10.png",
	"Images/2dAction/BG11.png",
	"Images/2dAction/BG12.png",
	"Images/2dAction/BG13.png",
};

class BG {
public:
	void Init();
	void Term();
	void Update();
	void Render();

private:

	std::vector<Background> bgList;
	Background bg[NUM_BACKGROUND];
	Texture mTextures[(int)Images::Count];
};