#include "Enemy.h"

Enemy::Enemy(SDL_Renderer* renderer, int screenWidth, int screenHeight, EnemyType type)
    : renderer_(renderer), screenWidth_(screenWidth), screenHeight_(screenHeight), type_(type),
    currentFrame_(0), frameTime_(100), isActive_(true) {
    Load();
    ResetPosition();

    lastFrameTime_ = SDL_GetTicks();
}

Enemy::~Enemy() {
}

void Enemy::Free() {
    if (texture_ != nullptr) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

void Enemy::Load() {
    int frameWidth, frameHeight, rand;

    std::string path;
    if (type_ == RHINO) {
        path = "IMG//Rhino//run (52x34).png";
        minspeed_ = 1.5f;
        maxspeed_ = 3.0f;
    }
    else {
        path = "IMG//Bat//flying (46x30).png";
        minspeed_ = 1.4f;
        maxspeed_ = 2.9f;
    }

    texture_ = IMG_LoadTexture(renderer_, path.c_str());
    if (!texture_) {
        return;
    }

    if (type_ == RHINO) {
        frameWidth = 52;
        frameHeight = 34;
        position_.w = frameWidth * 2;
        position_.h = frameHeight * 2;

        for (int i = 0; i < 6; ++i) {
            frames_.push_back({ i * frameWidth, 0, frameWidth, frameHeight });
        }

        position_.y = screenHeight_ - position_.h - 50;
    }
    else {
        frameWidth = 46;
        frameHeight = 30;
        position_.w = frameWidth * 2;
        position_.h = frameHeight * 2;

        for (int i = 0; i < 7; ++i) {
            frames_.push_back({ i * frameWidth, 0, frameWidth, frameHeight });
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(150, 500);
        rand = dist(gen);
        position_.y = screenHeight_ - position_.h - rand;
    }

    float elapsedTime = (float)currentScore / 500.0f;
    speed_ = minspeed_ + elapsedTime;
    if (speed_ > maxspeed_)
        speed_ = maxspeed_;
}

void Enemy::ResetPosition() {
    position_.x = screenWidth_;
    isActive_ = true;
}

void Enemy::Update() {
    if (!isActive_) return;
    Uint32 currentTime = SDL_GetTicks();
    
    position_.x -= speed_;

    if (currentTime - lastFrameTime_ > frameTime_) {
        currentFrame_ = (currentFrame_ + 1) % frames_.size();
        lastFrameTime_ = currentTime;
    }

    if (position_.x + position_.w < 0) {
        isActive_ = false;
    }
}

void Enemy::Render() {
    if (!isActive_ || !texture_) return;
    SDL_RenderCopy(renderer_, texture_, &frames_[currentFrame_], &position_);
}

bool Enemy::IsOffScreen() const {
    return (position_.x + position_.w < 0);
}

SDL_Rect Enemy::GetCollider() const {
    SDL_Rect collider;
    if (type_ == RHINO) {
        collider.w = 70;  
        collider.h = 40;  
        collider.x = position_.x + (position_.w - collider.w) / 2;
        collider.y = position_.y + position_.h - collider.h;
    }
    else {
        collider.w = 50;  
        collider.h = 30;  
        collider.x = position_.x + (position_.w - collider.w) / 2;
        collider.y = position_.y + position_.h - collider.h - 20;
    }

    return collider;
}