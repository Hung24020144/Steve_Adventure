#ifndef GAME_BASE_H
#define GAME_BASE_H

#include <Windows.h>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

static SDL_Window* g_window = nullptr;
static SDL_Renderer* g_screen = nullptr;
static SDL_Event g_event;
static TTF_Font* g_font = nullptr;
static TTF_Font* g_large_font = nullptr;
static Mix_Music* MenuMusic = nullptr;
static Mix_Music* PlayMusic = nullptr;
static Mix_Chunk* jumpEffect = nullptr;
static Mix_Chunk* deadEffect = nullptr;
static Mix_Chunk* clickEffect = nullptr;

namespace Collision {
    bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b);
}

enum GameState {
    MENU,
    PLAYING,
    HELP,
    GAMEOVER,
    PAUSE,
    REPLAY
};

class TTF{
public:
    TTF();
    ~TTF();

    void Render(SDL_Renderer* renderer, const std::string& path, TTF_Font* font, int y);
    void RenderScore(SDL_Renderer* renderer, int score, TTF_Font* font, int y);
    void Free();
private:
    SDL_Texture* texture_;
    SDL_Texture* scoreTexture_;
    SDL_Rect rect_;
    SDL_Rect scoreRect_;
    SDL_Color textColor;

    int lastScore_ = -1;
    int lastHighScore_ = -1;
    int lastScoreY_ = -1;
    int x;
    std::string lastScoreText_;
    std::string lastpath;
    bool isScore;
};

const int SCR_W = 1536;
const int SCR_H = 768;
const int SCR_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0;

const std::string 
    MENU_TEXT = "STEVE ADVENTURE !!!",
    PAUSE_TEXT = "GAME PAUSE",
    HELP_TEXT0 = "_Tutorial_",
    HELP_TEXT1 = "In his adventure, Steve is being chased by a Monster.                      ",
    HELP_TEXT2 = "Help him escape by pressing ''SPACE'' to jump over Rhinos and Bats.",
    HELP_TEXT3 = "Press ''SPACE'' twice to perform a double jump !                                ",
    HIGH_SCORE_TEXT = "HIGH SCORE !!",
    GAME_OVER_TEXT = "GAME OVER !";

extern int currentScore;
extern int highScore;

void SaveHighScore();
void LoadHighScore();

#endif 
