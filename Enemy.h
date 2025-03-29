#ifndef ENEMY_H
#define ENEMY_H

#include "GameBase.h"

enum EnemyType { RHINO, BAT };

class Enemy {
public:
    Enemy(SDL_Renderer* renderer, int screenWidth, int screenHeight, EnemyType type);
    ~Enemy();

    void Update();
    void Render();
    void Free();
    
    bool IsOffScreen() const;
    SDL_Rect GetCollider() const;
    bool IsActive() const { return isActive_; }

private:
    void Load();
    void ResetPosition();
   
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
    std::vector<SDL_Rect> frames_;
    SDL_Rect position_;
    EnemyType type_;

    int currentFrame_;
    Uint32 startTime_,
           frameTime_,
           lastFrameTime_;

    int screenWidth_;
    int screenHeight_;
    float speed_, minspeed_, maxspeed_;
    bool isActive_;
};

#endif