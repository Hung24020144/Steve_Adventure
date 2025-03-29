#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameBase.h"

class Character {
public:
    Character();
    ~Character();

    bool Load(SDL_Renderer* renderer);
    void HandleEvent(SDL_Event& e);
    void Update();
    void Render(SDL_Renderer* des);
    void Free();
    void Reset();

    void StartDeathAnimation();
    bool IsDeathAnimationComplete() const { return deathAnimationComplete_; }

    SDL_Rect GetCollider() const;

private:
    SDL_Rect rect_;
    SDL_Texture* run_texture_;
    SDL_Texture* jump_texture_;
    SDL_Texture* fall_texture_;
    SDL_Texture* death_texture_;

    std::vector<SDL_Rect> run_frames_;
    std::vector<SDL_Rect> jump_frames_;
    std::vector<SDL_Rect> fall_frames_;
    std::vector<SDL_Rect> death_frames_;

    int current_run_frame_,
        current_jump_frame_,
        current_fall_frame_,
        current_death_frame_;
    Uint32 run_frame_time_,
           jump_frame_time_,
           fall_frame_time_,
           death_frame_time_;
    Uint32 last_run_frame_time_,
           last_jump_frame_time_,
           last_fall_frame_time_,
           last_death_frame_time_;

    bool isJumping_;
    bool isFalling_;
    float jumpVelocity_;
    float gravity_;
    int jumpCount_;
    const int MAX_JUMP_ = 2;
    bool isDying_;
    bool deathAnimationComplete_;

};

#endif