#include "Monster.h"

Monster::Monster() : rect_{ 0, 520, 212, 209 }, currentFrame_(0), frameTime_(100), lastFrameTime_(0) {}

Monster::~Monster() {
	Free();
}

bool Monster::Load(SDL_Renderer* renderer) {
	std::string path = "IMG//Monster//walk_look_up.png";
	texture_ = IMG_LoadTexture(renderer, path.c_str());

	if (!texture_)
		return 0;

	for (int i = 0; i < 4; i++) {
		int current = (i < 3) ? 5 : 4;
		for (int j = 0; j < current; j++) {
			frames_.push_back({ j * 212, i * 209, 212, 209 });
		}
	}
	return 1;
}

void Monster::Update() {
	Uint32 currentTime = SDL_GetTicks();
	float time = fmod(currentTime / 1000.0f, 1.9f);
	if (time <= 1.0f) {
		rect_.x = (time / 1.0f) * 180;
	}
	else {
		rect_.x = 180 - ((time - 1.0f) / 0.9f) * 200;
	}
	if (currentTime - lastFrameTime_ > frameTime_) {
		currentFrame_ = (currentFrame_ + 1) % frames_.size();
		lastFrameTime_ = currentTime;
	}
}

void Monster::Render(SDL_Renderer* renderer) {
	SDL_RenderCopyEx(renderer, texture_, &frames_[currentFrame_], &rect_, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
}

void Monster::Free() {
	if (texture_ != nullptr)
		SDL_DestroyTexture(texture_);
	texture_ = nullptr;
}