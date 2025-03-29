#ifndef MONSTER_H
#define MONSTER_H

#include "GameBase.h"

class Monster {
public:
	Monster();
	~Monster();

	bool Load(SDL_Renderer* renderer);
	void Render(SDL_Renderer* renderer);
	void Update();
	void Free();

private:
	SDL_Texture* texture_;
	SDL_Rect rect_;
	std::vector<SDL_Rect> frames_;
	int currentFrame_;
	Uint32 frameTime_;
	Uint32 lastFrameTime_;
};

#endif MONSTER_H_
