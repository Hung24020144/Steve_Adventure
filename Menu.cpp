#include "Menu.h"

Menu::Menu() : menuBackground_(nullptr), helpPanel_(nullptr) {}

Menu::~Menu() {
    Free();
    playButton_, Free();
    helpButton_.Free();
    quitButton_.Free();
    backButton_.Free();
    homeButton_.Free();
    replayButton_.Free();
    exitButton_.Free();
    pauseButton_.Free();
    continueButton_.Free();
}

void Menu::Free() {
    if (menuBackground_ != nullptr) {
        SDL_DestroyTexture(menuBackground_);
    }
    if (helpPanel_ != nullptr) {
        SDL_DestroyTexture(helpPanel_);
    }
}

bool Menu::Load(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("IMG/Home/home.png");
    if (surface == nullptr) return false;
    menuBackground_ = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    menuBackgroundRect_.x = 0;  
    menuBackgroundRect_.y = 0;  
    menuBackgroundRect_.w = SCR_W;  
    menuBackgroundRect_.h = SCR_H;

    surface = IMG_Load("IMG/Home/panel.png");
    if (surface == nullptr) return false;
    helpPanel_ = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    helpPanelRect_.x = (SCR_W - 1400) / 2;  
    helpPanelRect_.y = (SCR_H - 400) / 2;   
    helpPanelRect_.w = 1400;  
    helpPanelRect_.h = 400;

    if (!playButton_.Load(renderer, "IMG/Buttons/play_normal.png", "IMG/Buttons/play_hover.png")) return false;
    if (!helpButton_.Load(renderer, "IMG/Buttons/help_normal.png", "IMG/Buttons/help_hover.png")) return false;
    if (!quitButton_.Load(renderer, "IMG/Buttons/quit_normal.png", "IMG/Buttons/quit_hover.png")) return false;
    if (!backButton_.Load(renderer, "IMG/Buttons/quit_normal.png", "IMG/Buttons/quit_hover.png")) return false;
    if (!homeButton_.Load(renderer, "IMG/Buttons/home_normal.png", "IMG/Buttons/home_hover.png")) return false;
    if (!replayButton_.Load(renderer, "IMG/Buttons/replay_normal.png", "IMG/Buttons/replay_hover.png")) return false;
    if (!exitButton_.Load(renderer, "IMG/Buttons/quit_normal.png", "IMG/Buttons/quit_hover.png")) return false;
    if (!pauseButton_.Load(renderer, "IMG/Buttons/pause_normal.png", "IMG/Buttons/pause_hover.png")) return false;
    if (!continueButton_.Load(renderer, "IMG/Buttons/play_normal.png", "IMG/Buttons/play_hover.png")) return false;

    playButton_.SetPosition(posX - 100, posY);
    helpButton_.SetPosition(posX, posY);
    quitButton_.SetPosition(posX + 100, posY);
    backButton_.SetPosition(SCR_W - 125, 50);
    homeButton_.SetPosition(posX - 100, posY);
    replayButton_.SetPosition(posX, posY);
    exitButton_.SetPosition(posX + 100, posY);
    pauseButton_.SetPosition(SCR_W - 125, 50);
    continueButton_.SetPosition(posX + 100, posY);

    return true;
}

void Menu::HandleEvents(SDL_Event* e, GameState& currentState, bool& quitGame) {
    if (currentState == MENU) {
        playButton_.HandleEvent(e);
        helpButton_.HandleEvent(e);
        quitButton_.HandleEvent(e);

        if (playButton_.IsClicked()) {
            currentState = REPLAY;
            playButton_.Reset();
        }
        else if (helpButton_.IsClicked()) {
            currentState = HELP;
            helpButton_.Reset();
        }
        else if (quitButton_.IsClicked()) {
            quitGame = true;
        }
    }
    else if (currentState == HELP) {
        backButton_.HandleEvent(e);

        if (backButton_.IsClicked()) {
            currentState = MENU;
            backButton_.Reset();
        }
    }
    else if (currentState == GAMEOVER) {
        homeButton_.HandleEvent(e);
        replayButton_.HandleEvent(e);
        exitButton_.HandleEvent(e);

        if (homeButton_.IsClicked()) {
            currentState = MENU;
            homeButton_.Reset();
        }
        else if (replayButton_.IsClicked()) {
            currentState = REPLAY;
            replayButton_.Reset();
        }
        else if (exitButton_.IsClicked()) {
            quitGame = true;
        }
    }
    else if (currentState == PLAYING) {
        pauseButton_.HandleEvent(e);

        if (pauseButton_.IsClicked()) {
            currentState = PAUSE;
            pauseButton_.Reset();
        }
    }
    else if (currentState == PAUSE) {
        homeButton_.HandleEvent(e);
        replayButton_.HandleEvent(e);
        continueButton_.HandleEvent(e);

        if (homeButton_.IsClicked()) {
            currentState = MENU;
            homeButton_.Reset();
        }
        else if (replayButton_.IsClicked()) {
            currentState = REPLAY;
            replayButton_.Reset();
        }
        else if (continueButton_.IsClicked()) {
            currentState = PLAYING;
            continueButton_.Reset();
        }
    }
}

void Menu::Render(SDL_Renderer* renderer, GameState currentState) {
    if (currentState == MENU) {
        RenderMenu(renderer);
    }
    else if (currentState == HELP) {
        RenderHelp(renderer);
    }
    else if (currentState == GAMEOVER) {
        RenderGameover(renderer);
    }
    else if (currentState == PAUSE) {
        RenderPause(renderer);
    }
    else if (currentState == PLAYING) {
        RenderPlaying(renderer);
    }
}

void Menu::RenderMenu(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, menuBackground_, nullptr, &menuBackgroundRect_);
    playButton_.Render(renderer);
    helpButton_.Render(renderer);
    quitButton_.Render(renderer);
}

void Menu::RenderHelp(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, menuBackground_, nullptr, &menuBackgroundRect_);
    SDL_RenderCopy(renderer, helpPanel_, nullptr, &helpPanelRect_);
    backButton_.Render(renderer);
}

void Menu::RenderGameover(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect overlay = { 0, 0, SCR_W, SCR_H };
    SDL_RenderFillRect(renderer, &overlay);

    homeButton_.Render(renderer);
    replayButton_.Render(renderer);
    exitButton_.Render(renderer);
}

void Menu::RenderPause(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect overlay = { 0, 0, SCR_W, SCR_H };
    SDL_RenderFillRect(renderer, &overlay);

    homeButton_.Render(renderer);
    replayButton_.Render(renderer);
    continueButton_.Render(renderer);
}

void Menu::RenderPlaying(SDL_Renderer* renderer) {
    pauseButton_.Render(renderer);
}
