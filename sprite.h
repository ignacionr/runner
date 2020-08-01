#pragma once
#include <memory>
#include "scene.h"

class Sprite
{
public:
    Sprite(std::shared_ptr<sdl::Renderer> const &renderer, const char *filename, int rows, int cols)
        : _renderer{renderer}, _rows{rows}, _cols{cols}
    {
        sdl::Surface img(filename);
        _frame = img.Dimensions();
        _frame.w /= cols;
        _frame.h /= rows;
        _texture = std::make_shared<sdl::Texture>(*_renderer, img);
    }

    void render()
    {
        static int step{0};
        auto total{_rows * _cols};
        ++step %= total;
        auto const row{step / _cols};
        auto const col{step % _cols};
        _frame.x = col * _frame.w;
        _frame.y = row * _frame.h;
        _renderer->Copy(*_texture, &_frame);
    }

    void hook(sdls::Character &character) {}

private:
    std::shared_ptr<sdl::Texture> _texture;
    std::shared_ptr<sdl::Renderer> _renderer;
    SDL_Rect _frame;
    int _rows, _cols;
};
