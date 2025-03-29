#include "Button.h"

Button::Button() : position_({ 0, 0 }), currentState_(BUTTON_MOUSE_OUT),
textureNormal_(nullptr), textureHover_(nullptr), isClicked_(false) {
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 70;
    rect_.h = 70;
}

Button::~Button() {
    Free();
}

void Button::Free() {
    if (textureNormal_ != nullptr) {
        SDL_DestroyTexture(textureNormal_);
    }
    if (textureHover_ != nullptr) {
        SDL_DestroyTexture(textureHover_);
    }
}

void Button::SetPosition(int x, int y) {
    position_.x = x;
    position_.y = y;
}

bool Button::Load(SDL_Renderer* renderer, const std::string& normalPath, const std::string& hoverPath) {
    SDL_Surface* surface = IMG_Load(normalPath.c_str());
    if (surface == nullptr) {
        return false;
    }
    textureNormal_ = SDL_CreateTextureFromSurface(renderer, surface);
    rect_.w = surface->w;
    rect_.h = surface->h;
    SDL_FreeSurface(surface);

    surface = IMG_Load(hoverPath.c_str());
    if (surface == nullptr) {
        return false;
    }
    textureHover_ = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (clickEffect == nullptr)
        clickEffect = Mix_LoadWAV("SOUND/mouse_click.wav");

    return (textureNormal_ != nullptr && textureHover_ != nullptr);
}

void Button::Render(SDL_Renderer* renderer) {
    SDL_Rect renderQuad = { position_.x, position_.y, rect_.w, rect_.h };

    if (currentState_ == BUTTON_MOUSE_OVER) {
        SDL_RenderCopy(renderer, textureHover_, nullptr, &renderQuad);
    }
    else {
        SDL_RenderCopy(renderer, textureNormal_, nullptr, &renderQuad);
    }
}

void Button::HandleEvent(SDL_Event* e) {
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        bool inside = (x >= position_.x && x <= position_.x + rect_.w &&
            y >= position_.y && y <= position_.y + rect_.h);

        if (!inside) {
            currentState_ = BUTTON_MOUSE_OUT;
            isClicked_ = false;
        }
        else {
            currentState_ = BUTTON_MOUSE_OVER;

            if (e->type == SDL_MOUSEBUTTONDOWN) {
                isClicked_ = true;
                Mix_PlayChannel(-1, clickEffect, 0);
            }
        }
    }
}

bool Button::IsClicked() const {
    return isClicked_;
}

void Button::Reset() {
    isClicked_ = false;
    currentState_ = BUTTON_MOUSE_OUT;
}