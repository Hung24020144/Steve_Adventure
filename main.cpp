#include "LBackground.h"
#include "Character.h"
#include "Enemy.h"
#include "Monster.h"
#include "Menu.h"

LBackground layer[6];
Monster monster;
Character player;
std::vector<Enemy> enemies;
Menu gameMenu;
Uint32  lastScoreUpdateTime = 0,
        startTime,
        currentTime,
        lastRhinoSpawnTime = 0,
        lastBatSpawnTime = 0,
        RhinoSpawnInterval = 2000,
        BatSpawnInterval = 1500;
GameState currentGameState = MENU,
          lastGameState = PLAYING;
TTF text; 

const int SCORE_UPDATE_INTERVAL = 100,
          MAX_ENEMIES = 4;

bool InitData() {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Steve_Adventure",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCR_W,
        SCR_H,
        SDL_WINDOW_SHOWN);

    if (g_window == nullptr)
        return false;
    else {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == nullptr)
            return false;
        else {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgflags = IMG_INIT_PNG;
            if (!(IMG_Init(imgflags) && imgflags))
                return false;
            if (TTF_Init() == -1)
                return false;
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                return false;
        }
    }

    if (!layer[0].Load(g_screen, "IMG/Background/Hills Layer 01.png")) return 0;
    if (!layer[1].Load(g_screen, "IMG/Background/Hills Layer 02.png")) return 0;
    if (!layer[2].Load(g_screen, "IMG/Background/Hills Layer 03.png")) return 0;
    if (!layer[3].Load(g_screen, "IMG/Background/Hills Layer 04.png")) return 0;
    if (!layer[4].Load(g_screen, "IMG/Background/Hills Layer 05.png")) return 0;
    if (!layer[5].Load(g_screen, "IMG/Background/Hills Layer 06.png")) return 0;
    g_font = TTF_OpenFont("TTF/DungeonFont.ttf", 50);
    g_large_font = TTF_OpenFont("TTF/DungeonFont.ttf", 100);
    PlayMusic = Mix_LoadMUS("SOUND/bkgr_audio.wav");
    MenuMusic = Mix_LoadMUS("SOUND/menu_audio.wav");
    deadEffect = Mix_LoadWAV("SOUND/lose_sound.wav");
    if (g_font == nullptr || g_large_font == nullptr || PlayMusic == nullptr || MenuMusic == nullptr || deadEffect == nullptr)
        return false;
    LoadHighScore();

    return true;
}

void HandleMusic(GameState state) {
    if (state == lastGameState || lastGameState == HELP || state == HELP) return;
    Mix_HaltMusic();
    switch (state) {
    case MENU:
        Mix_PlayMusic(MenuMusic, -1);
        break;
    case PLAYING:
        Mix_PlayMusic(PlayMusic, -1);
        break;
    case PAUSE:
    case GAMEOVER:
    case REPLAY:
        break;
    }
    lastGameState = state;
}

void close() {
    gameMenu.Free();
    for (int i = 0; i < 6; i++)
        layer[i].Free();
    player.Free();
    for (auto& enemy : enemies)
        enemy.Free();
    enemies.clear();
    monster.Free();
    text.Free();

    SaveHighScore();
    TTF_CloseFont(g_font);
    TTF_CloseFont(g_large_font);
    SDL_DestroyRenderer(g_screen);
    g_screen = nullptr;
    SDL_DestroyWindow(g_window);
    g_window = nullptr;

    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    Mix_FreeMusic(MenuMusic);
    MenuMusic = nullptr;
    Mix_FreeMusic(PlayMusic);
    PlayMusic = nullptr;
    Mix_FreeChunk(clickEffect);
    clickEffect = nullptr;
    Mix_FreeChunk(deadEffect);
    deadEffect = nullptr;
    Mix_FreeChunk(jumpEffect);
    jumpEffect = nullptr;
    
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!InitData())
        return -1;

    if (!player.Load(g_screen))
        return -2;

    if (!monster.Load(g_screen))
        return -3;

    if (!gameMenu.Load(g_screen)) 
        return -4;

    bool IsQuit = 0;
    while (!IsQuit) {
        while (SDL_PollEvent(&g_event) != 0) {
            if (g_event.type == SDL_QUIT) {
                IsQuit = true;
            }
            if (currentGameState == MENU || currentGameState == HELP || currentGameState == GAMEOVER || currentGameState == PAUSE) {
                gameMenu.HandleEvents(&g_event, currentGameState, IsQuit);
            }
            else if (currentGameState == PLAYING) {
                player.HandleEvent(g_event);
                gameMenu.HandleEvents(&g_event, currentGameState, IsQuit);
            }
        }

        HandleMusic(currentGameState);

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        if (currentGameState == MENU) {
            gameMenu.Render(g_screen, currentGameState);
            text.Render(g_screen, MENU_TEXT,g_large_font, 250);
        }
        else if (currentGameState == HELP) {
            gameMenu.Render(g_screen, currentGameState);
            text.Render(g_screen, HELP_TEXT0, g_large_font, 290);
            text.Render(g_screen, HELP_TEXT1, g_font, 400);
            text.Render(g_screen, HELP_TEXT2, g_font, 450);
            text.Render(g_screen, HELP_TEXT3, g_font, 500);
        }
        else if (currentGameState == PLAYING) {
            player.Update();
            monster.Update();
            currentTime = SDL_GetTicks();
            if (currentTime - lastScoreUpdateTime > SCORE_UPDATE_INTERVAL) {
                currentScore += 1; 
                lastScoreUpdateTime = currentTime;
                if (currentScore > highScore) {
                    highScore = currentScore;
                }
            }
            if (currentTime - lastRhinoSpawnTime > RhinoSpawnInterval && enemies.size() < MAX_ENEMIES) {
                enemies.emplace_back(g_screen, SCR_W, SCR_H, RHINO);
                lastRhinoSpawnTime = currentTime;
                RhinoSpawnInterval = 1000 + (rand() % 2000);
            }

            if (currentTime - lastBatSpawnTime > BatSpawnInterval && enemies.size() < MAX_ENEMIES) {
                enemies.emplace_back(g_screen, SCR_W, SCR_H, BAT);
                lastBatSpawnTime = currentTime;
                BatSpawnInterval = 800 + (rand() % 1700);
            }
            for (auto& enemy : enemies) {
                enemy.Update();
                if (enemy.IsActive()) {  
                    SDL_Rect playerCollider = player.GetCollider();
                    SDL_Rect enemyCollider = enemy.GetCollider();

                    if (Collision::CheckCollision(playerCollider, enemyCollider)) {
                        currentGameState = GAMEOVER;
                        player.StartDeathAnimation();
                        Mix_PlayChannel(-1, deadEffect, 0);
                        break; 
                    }
                }
            }
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& e) { return e.IsOffScreen(); }), enemies.end());

            for (int i = 0; i < 5; i++)
                layer[i].Render(g_screen, i / 3.0);
            for (auto& enemy : enemies) 
                enemy.Render();
            monster.Render(g_screen);
            player.Render(g_screen);
            layer[5].Render(g_screen, 1.8);
            text.RenderScore(g_screen, currentScore, g_font, 50);
            text.RenderScore(g_screen, highScore, g_font, 120);
            gameMenu.Render(g_screen, currentGameState);
        }
        else if (currentGameState == PAUSE) {
            for (int i = 0; i < 5; i++)
                layer[i].Render(g_screen, 0.0);
            for (auto& enemy : enemies)
                enemy.Render();
            monster.Render(g_screen);
            player.Render(g_screen);
            layer[5].Render(g_screen, 0.0);
            text.RenderScore(g_screen, currentScore, g_font, 50);
            text.RenderScore(g_screen, highScore, g_font, 120);
            gameMenu.Render(g_screen, currentGameState);
            text.Render(g_screen, PAUSE_TEXT, g_large_font, 250);
       }
        else if (currentGameState == GAMEOVER) {
            player.Update();

            for (int i = 0; i < 5; i++)
                layer[i].Render(g_screen, 0.0);
            for (auto& enemy : enemies)
                enemy.Render();
            monster.Render(g_screen);
            player.Render(g_screen);
            layer[5].Render(g_screen, 0.0);
            if (player.IsDeathAnimationComplete()) {
                gameMenu.Render(g_screen, currentGameState);
                if (currentScore >= highScore) {
                    text.Render(g_screen, HIGH_SCORE_TEXT, g_large_font, 250);
                    text.Render(g_screen, std::to_string(currentScore), g_large_font, 350);
                }
                else {
                    text.Render(g_screen, GAME_OVER_TEXT, g_large_font, 250);
                    text.Render(g_screen, "YOUR SCCORE: " + std::to_string(currentScore), g_font,370);
                }
            }
        }
        else if (currentGameState == REPLAY) {
            player.Reset();
            enemies.clear();
            lastRhinoSpawnTime = 0;
            lastBatSpawnTime = 0;
            startTime = SDL_GetTicks();
            currentScore = 0;
            currentGameState = PLAYING;
        }

        SDL_RenderPresent(g_screen);

    }

    close();
    return 0;
}