#pragma once
#include <memory>
#include <functional>
#include <list>
#include <iostream>
#include "sdladapters.h"

namespace sdls
{
    struct Character
    {
        SDL_Rect area;
        std::function<void()> render;
        std::function<void(bool)> highlight;
        std::function<void()> click;
    };

    class Scene
    {
    public:
        Scene(std::shared_ptr<sdl::Renderer> renderer) : _renderer{renderer} { ; }

        auto &background(const sdl::Color &color)
        {
            _paint_background = [&]() { _renderer->Clear(color); };
            return *this;
        }

        auto &background(const std::string_view &filename)
        {
            sdl::Surface surface(std::string(filename).c_str());
            _background_texture = std::make_shared<sdl::Texture>(*_renderer, surface);
            _paint_background = [&]() {
                _renderer->SetViewPort(nullptr);
                _renderer->Copy(*_background_texture);
            };
            return *this;
        }

        auto &background(const std::string_view &filename, SDL_Rect const &initialSource, std::function<void(SDL_Rect &)> move)
        {
            sdl::Surface surface(std::string(filename).c_str());
            _background_texture = std::make_shared<sdl::Texture>(*_renderer, surface);
            _background_source = initialSource;
            _paint_background = [&]() {
                _renderer->SetViewPort(nullptr);
                _renderer->Copy(*_background_texture, &_background_source);
                move(_background_source);
            };
            return *this;
        }

        template <typename T>
        auto &add(T &renderable, SDL_Rect const &rc)
        {
            auto character = std::make_unique<Character>();
            character->area = rc;
            character->render = std::bind(&T::render, &renderable);
            renderable.hook(*character);
            _characters.push_back(std::move(character));
            return _characters.back();
        }

        auto &render()
        {
            _paint_background();
            for (auto &c : _characters)
            {
                _renderer->SetViewPort(&c->area);
                c->render();
            }
            _renderer->Present();
            return *this;
        }

        auto &interact(std::function<void(int, int)> moves)
        {
            _interactive = moves;
            return *this;
        }

        auto hit_test(int x, int y)
        {
            auto pos = std::find_if(
                _characters.rbegin(),
                _characters.rend(),
                [x, y](auto const &ch) {
                    return x >= ch->area.x && y >= ch->area.y &&
                           x <= (ch->area.x + ch->area.w) && y <= (ch->area.y + ch->area.h);
                });
            std::unique_ptr<Character> *found =
                pos == _characters.rend() ? nullptr : &(*pos);
            return found;
        }

        bool handle_event(SDL_Event *ev)
        {
            if (ev->type == SDL_MOUSEMOTION)
            {
                const auto x = ev->motion.x;
                const auto y = ev->motion.y;
                auto newlyHighLighted = hit_test(x, y);
                if (newlyHighLighted != _highlighted)
                {
                    if (_highlighted && _highlighted->get()->highlight)
                        _highlighted->get()->highlight(false);
                    if (newlyHighLighted && newlyHighLighted->get()->highlight)
                        newlyHighLighted->get()->highlight(true);
                    _highlighted = newlyHighLighted;
                }
                return true;
            }
            else if (ev->type == SDL_MOUSEBUTTONDOWN)
            {
                const auto x = ev->motion.x;
                const auto y = ev->motion.y;
                auto pressed = hit_test(x, y);
                if (pressed && pressed->get()->click)
                {
                    pressed->get()->click();
                }
            }
            else if (ev->type == SDL_KEYDOWN && _interactive)
            {
                if (ev->key.keysym.scancode == SDL_SCANCODE_RIGHT)
                {
                    _interactive(ev->key.repeat + 1, 0);
                }
                else if (ev->key.keysym.scancode == SDL_SCANCODE_LEFT)
                {
                    _interactive(-ev->key.repeat - 1, 0);
                }
                else if (ev->key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    _interactive(0, -ev->key.repeat - 1);
                }
                else if (ev->key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    _interactive(0, ev->key.repeat + 1);
                }
            }
            return false;
        }

    private:
        std::shared_ptr<sdl::Renderer> _renderer;
        std::function<void()> _paint_background;
        std::list<std::unique_ptr<Character>> _characters;
        std::unique_ptr<Character> *_highlighted{nullptr};
        std::shared_ptr<sdl::Texture> _background_texture;
        SDL_Rect _background_source;
        std::function<void(int, int)> _interactive;
    };

    class Chapter
    {
    };

    class Play
    {
    };
} // namespace sdls
