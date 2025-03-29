#ifndef BUTTON_H
#define BUTTON_H

#include "GameBase.h"

class Button {
public:
    Button();
    ~Button();

    void SetPosition(int x, int y);
    bool Load(SDL_Renderer* renderer, const std::string& normalPath, const std::string& hoverPath);
    void Render(SDL_Renderer* renderer);
    void HandleEvent(SDL_Event* e);
    bool IsClicked() const;
    void Reset();
    void Free();

    enum ButtonState {
        BUTTON_MOUSE_OUT = 0,
        BUTTON_MOUSE_OVER = 1,
        BUTTON_CLICKED = 2
    };

private:
    SDL_Point position_;
    ButtonState currentState_;
    SDL_Texture* textureNormal_;
    SDL_Texture* textureHover_;
    SDL_Rect rect_;
    bool isClicked_;
};

#endif