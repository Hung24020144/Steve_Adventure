#ifndef MENU_H
#define MENU_H

#include "GameBase.h"
#include "Button.h"

class Menu {
public:
    Menu();
    ~Menu();

    bool Load(SDL_Renderer* renderer);
    void HandleEvents(SDL_Event* e, GameState& currentState, bool& quitGame);
    void Render(SDL_Renderer* renderer, GameState currentState);
    void Free();

private:
    int posX = (SCR_W - 70) / 2;
    int posY = SCR_H / 2 + 80;

    SDL_Texture* menuBackground_;
    SDL_Texture* helpPanel_;
    Button playButton_;
    Button helpButton_;
    Button quitButton_;
    Button backButton_;
    Button pauseButton_;
    Button replayButton_;
    Button homeButton_;
    Button exitButton_;
    Button continueButton_;
    SDL_Rect menuBackgroundRect_;
    SDL_Rect helpPanelRect_;

    void RenderMenu(SDL_Renderer* renderer);
    void RenderHelp(SDL_Renderer* renderer);
    void RenderPlaying(SDL_Renderer* renderer);
    void RenderPause(SDL_Renderer* renderer);
    void RenderGameover(SDL_Renderer* renderer);
};

#endif
