#include "Character.h"

Character::Character() : rect_{ 380, 550, 300, 300 },
run_texture_(nullptr), jump_texture_(nullptr), fall_texture_(nullptr), death_texture_(nullptr),
isJumping_(false), isFalling_(false), isDying_(false), deathAnimationComplete_(false),
jumpVelocity_(0.0f), gravity_(0.18f), jumpCount_(0),
current_run_frame_(0), current_jump_frame_(0), current_fall_frame_(0), current_death_frame_(0),
run_frame_time_(100), jump_frame_time_(200), fall_frame_time_(400), death_frame_time_(100),
last_run_frame_time_(0), last_jump_frame_time_(0), last_fall_frame_time_(0), last_death_frame_time_(0) {}

Character::~Character() {
    Free();
}

void Character::Free() {
    if (run_texture_) SDL_DestroyTexture(run_texture_);
    if (jump_texture_) SDL_DestroyTexture(jump_texture_);
    if (fall_texture_) SDL_DestroyTexture(fall_texture_);
    if (death_texture_) SDL_DestroyTexture(death_texture_);
    run_texture_ = jump_texture_ = fall_texture_ = death_texture_ = nullptr;
}

bool Character::Load(SDL_Renderer* renderer) {
    run_texture_ = IMG_LoadTexture(renderer, "IMG//Steve//Run.png");
    jump_texture_ = IMG_LoadTexture(renderer, "IMG//Steve//Jump.png");
    fall_texture_ = IMG_LoadTexture(renderer, "IMG//Steve//Fall.png");
    death_texture_ = IMG_LoadTexture(renderer, "IMG/Steve/Death.png");

    if (!run_texture_ || !jump_texture_ || !fall_texture_ || !death_texture_) {
        return false;
    }

    for (int i = 0; i < 8; i++) run_frames_.push_back({ i * 128, 0, 128, 128 });
    for (int i = 0; i < 4; i++) jump_frames_.push_back({ i * 128, 0, 128, 128 });
    for (int i = 0; i < 2; i++) fall_frames_.push_back({ i * 128, 0, 128, 128 });
    for (int i = 0; i < 24; i++) death_frames_.push_back({ i * 128, 0, 128, 128 });

    jumpEffect = Mix_LoadWAV("SOUND/jump_sound.wav");
    
    return true;
}

void Character::HandleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_SPACE) {
            if (!isJumping_ || (isJumping_ && jumpCount_ < MAX_JUMP_)) {
                isJumping_ = true;
                jumpVelocity_ = -10.0f;
                jumpCount_++;
                current_jump_frame_ = 0;
                last_jump_frame_time_ = SDL_GetTicks();
                Mix_PlayChannel(-1, jumpEffect, 0);
            }
        }
    }
}

void Character::Update() {
    Uint32 current_time = SDL_GetTicks();
    if (!isJumping_ && !isFalling_ && !isDying_ && current_time - last_run_frame_time_ > run_frame_time_) {
        current_run_frame_ = (current_run_frame_ + 1) % run_frames_.size();
        last_run_frame_time_ = current_time;
    }
    if (isJumping_ && !isFalling_ && !isDying_ && current_time - last_jump_frame_time_ > jump_frame_time_) {
        current_jump_frame_ = (current_jump_frame_ + 1) % jump_frames_.size();
        last_jump_frame_time_ = current_time;
    }
    if (isFalling_ && !isDying_ && current_time - last_fall_frame_time_ > fall_frame_time_) {
        current_fall_frame_ = (current_fall_frame_ + 1) % fall_frames_.size();
        last_fall_frame_time_ = current_time;
    }
    if (isDying_ && current_time - last_death_frame_time_ > death_frame_time_) {
        current_death_frame_ ++;
        last_death_frame_time_ = current_time;
        if (current_death_frame_ >= death_frames_.size()) {
            deathAnimationComplete_ = true;
            current_death_frame_ = death_frames_.size() - 1;
        }
    }

    if (isJumping_) {
        rect_.y += jumpVelocity_;
        jumpVelocity_ += gravity_;
        if (jumpVelocity_ > 0) {
            isFalling_ = true;
            current_fall_frame_ = 0;
            last_fall_frame_time_ = SDL_GetTicks();
        }
        if (rect_.y >= 550) {
            rect_.y = 550;
            isJumping_ = false;
            isFalling_ = false;
            jumpCount_ = 0;
            current_run_frame_ = 0;
            last_run_frame_time_ = SDL_GetTicks();
        }
    }
}

void Character::Reset() {
    rect_ = { 380, 550, 300, 300 };
    isJumping_ = false;
    isFalling_ = false;
    isDying_ = false;
    deathAnimationComplete_ = false;
    jumpCount_ = 0;
    current_run_frame_ = 0;
    current_death_frame_ = 0;
}

void Character::Render(SDL_Renderer* des) {
    SDL_Texture* current_texture = run_texture_;
    SDL_Rect* current_frame = &run_frames_[current_run_frame_];

    if (isJumping_ && !isFalling_ && !isDying_) {
        current_texture = jump_texture_;
        current_frame = &jump_frames_[current_jump_frame_];
    }
    else if (isFalling_ && !isDying_) {
        current_texture = fall_texture_;
        current_frame = &fall_frames_[current_fall_frame_];
    }
    else if (isDying_) {
        current_texture = death_texture_;
        current_frame = &death_frames_[current_death_frame_];
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    if (current_texture != nullptr) {
        SDL_RenderCopy(des, current_texture, current_frame, &rect_);
    }
    else {
        SDL_SetRenderDrawColor(des, 255, 0, 0, 255);
        SDL_RenderFillRect(des, &rect_);
    }
}

void Character::StartDeathAnimation() {
    isDying_ = true;
}

SDL_Rect Character::GetCollider() const {
    SDL_Rect collider;
    collider.w = 60;  
    collider.h = 80; 
    collider.x = rect_.x + (rect_.w - collider.w) / 2; 
    collider.y = rect_.y + rect_.h - collider.h - 120;

    return collider;
}