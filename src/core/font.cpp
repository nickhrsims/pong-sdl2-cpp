#include "font.h"
#include "SDL_ttf.h"

Font::Font(const std::string path, const int size) { TTF_OpenFont(path.c_str(), size); }
Font::~Font() { TTF_CloseFont(data); }

Font::Font(Font&& other) {
    data       = other.data;
    other.data = nullptr;
}

Font& Font::operator=(Font&& rhs) {
    if (data) {
        TTF_CloseFont(data);
    }
    data     = rhs.data;
    rhs.data = nullptr;
    return *this;
}

TTF_Font* Font::get() const { return data; }
