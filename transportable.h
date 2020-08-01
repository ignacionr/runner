#pragma once
#include "scene.h"

template <typename TRenderable>
class Transportable
{
public:
    Transportable(TRenderable &target)
        : _renderable{target}
    {
    }

    void render()
    {
        if (_position)
        {
            _position->x += (_target.x - _position->x) / 50;
            _position->y += (_target.y - _position->y) / 80;
        }
        _renderable.render();
    }

    void hook(sdls::Character &character)
    {
        _renderable.hook(character);
        _position = &character.area;
        _target = *_position;
    }

    void towards(SDL_Rect const &rc)
    {
        _target = rc;
    }

private:
    TRenderable &_renderable;
    SDL_Rect *_position{nullptr};
    SDL_Rect _target;
};
