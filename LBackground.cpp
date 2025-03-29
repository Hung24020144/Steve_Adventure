#include "LBackground.h";

LBackground::LBackground() : bgRect1{ 0, 0, 1536, 768 },
bgRect2{ 1536, 0, 1536, 768 }, texture_(nullptr){}

LBackground::~LBackground() {
	Free();
}

bool LBackground::Load(SDL_Renderer* renderer, const std::string& path) {
	SDL_Texture* new_texture = nullptr;
	SDL_Surface* load_surface = IMG_Load(path.c_str());
	if (load_surface != nullptr) {
		SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
		new_texture = SDL_CreateTextureFromSurface(renderer, load_surface);
		if (new_texture != nullptr) {
			rect_.w = load_surface->w * 3;
			rect_.h = load_surface->h * 3;
		}
		SDL_FreeSurface(load_surface);
	}

	texture_ = new_texture;
	return texture_ != nullptr;
}

void LBackground::Render(SDL_Renderer* renderer,double lSpeed) {
	xOffset1 -= lSpeed;
	xOffset2 -= lSpeed;

	if (xOffset1 + bgRect1.w <= 0) xOffset1 = xOffset2 + bgRect2.w;
	if (xOffset2 + bgRect2.w <= 0) xOffset2 = xOffset1 + bgRect1.w;

	bgRect1.x = static_cast<int>(xOffset1);
	bgRect2.x = static_cast<int>(xOffset2);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	SDL_RenderCopy(renderer, texture_, nullptr, &bgRect1);
	SDL_RenderCopy(renderer, texture_, nullptr, &bgRect2);
}

void LBackground::Free() {
	if (texture_ != nullptr)
		SDL_DestroyTexture(texture_);
	texture_ = nullptr;
}