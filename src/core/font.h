#pragma once

#include <string>

#include "SDL_ttf.h"

class Font {
  public:
    Font(const std::string path, const int points);
    ~Font();

    Font(Font&& other);
    Font& operator=(Font&& rhs);

    TTF_Font* get() const;

  private:
    TTF_Font* data;
    Font(const Font& other);
    Font& operator=(const Font& rhs);
};
