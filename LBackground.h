#ifndef LOAD_BACKGROUND_H
#define LOAD_BACKGROUND_H

#include "GameBase.h"

class LBackground {
public:
	LBackground();
	~LBackground();

	bool Load(SDL_Renderer* renderer, const std::string& path);
	void Render(SDL_Renderer* renderer, double lSpeed); 
	void Update(float deltaTime);
	void Free();
	
private:
	SDL_Texture* texture_;
	SDL_Rect bgRect1, bgRect2, rect_;
	double xOffset1 = 0.0;  
	double xOffset2 = 1536.0;
};

#endif
