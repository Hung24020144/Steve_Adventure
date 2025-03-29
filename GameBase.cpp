#include "GameBase.h"

bool Collision::CheckCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return (a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y);
}

int currentScore = 0;
int highScore = 0;

void SaveHighScore() {
    std::ofstream outFile("high_score.txt");
    if (outFile.is_open()) {
        outFile << highScore;
        outFile.close();
    }
}

void LoadHighScore() {
    std::ifstream inFile("high_score.txt");
    if (inFile.is_open()) {
        inFile >> highScore;
        inFile.close();
    }
}

TTF::TTF() : texture_(nullptr), isScore(false) {};

TTF::~TTF() {
    Free();
}

void TTF::Render(SDL_Renderer* renderer, const std::string& path, TTF_Font* font, int y) {
    if (texture_ != nullptr && lastpath == path) {
        rect_.x = x;
        rect_.y = y;
        SDL_RenderCopy(renderer, texture_, nullptr, &rect_);
        return;
    }
    Free();
    textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, path.c_str(), textColor);
    if (textSurface) {
        rect_.w = textSurface->w;
        rect_.h = textSurface->h;
        x = (SCR_W - rect_.w) / 2;
        rect_.x = x;
        rect_.y = y;
        texture_ = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        lastpath = path;
    }
    if (texture_) {
        SDL_RenderCopy(renderer, texture_, nullptr, &rect_);
    }
}

void TTF::RenderScore(SDL_Renderer* renderer, int score, TTF_Font* font, int y) {
    bool isCurrentScore = (y == 50);

    if (scoreTexture_ != nullptr &&
        ((isCurrentScore && score == lastScore_ && y == lastScoreY_) ||
            (!isCurrentScore && score == lastHighScore_ && y == lastScoreY_))) {
        scoreRect_.x = 50;
        scoreRect_.y = y;
        SDL_RenderCopy(renderer, scoreTexture_, nullptr, &scoreRect_);
        return;
    }

    std::string scoreText = isCurrentScore ?
        "Your Score: " + std::to_string(score) :
        "High Score: " + std::to_string(score);

    if (scoreTexture_ != nullptr) {
        SDL_DestroyTexture(scoreTexture_);
        scoreTexture_ = nullptr;
    }

    SDL_Color textColor = { 0, 0, 0, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (textSurface) {
        scoreRect_.w = textSurface->w;
        scoreRect_.h = textSurface->h;
        scoreRect_.x = 50;
        scoreRect_.y = y;
        scoreTexture_ = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);

        if (isCurrentScore) {
            lastScore_ = score;
        }
        else {
            lastHighScore_ = score;
        }
        lastScoreY_ = y;
        lastScoreText_ = scoreText;
    }

    if (scoreTexture_ != nullptr) {
        SDL_RenderCopy(renderer, scoreTexture_, nullptr, &scoreRect_);
    }
}

void TTF::Free() {
    if (texture_ != nullptr) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
    if (scoreTexture_ != nullptr) {
        SDL_DestroyTexture(scoreTexture_);
        scoreTexture_ = nullptr;
    }
}
