#pragma once
#include <string_view>
#include "sdladapters.h"
#include "scene.h"

namespace controls
{
    class Link
    {
    public:
        Link(std::string_view const &text_param,
             std::shared_ptr<sdl::Font> const &font,
             std::shared_ptr<sdl::Renderer> const &renderer,
             std::function<void()> action)
            : _font{font}, _renderer{renderer}, _action{action}
        {
            _hand_cursor = std::make_unique<sdl::Cursor>(SDL_SYSTEM_CURSOR_HAND);
            text(text_param);
        }

        void render()
        {
            _renderer->Copy(*_texture, nullptr, nullptr);
        }

        void text(std::string_view const &text)
        {
            auto surface = _font->RenderText_Solid(text, sdl::Color(255, 255, 255));
            _dimensions = surface->Dimensions();
            _texture = std::make_shared<sdl::Texture>(*_renderer, *surface);
            _texture->SetColorMod(0, 0, 255);
        }

        void highlight(bool value)
        {
            if (_highlight != value)
            {
                _texture->SetColorMod(value ? 255 : 0, 0, value ? 0 : 255);
                (value ? _hand_cursor.get() : &_normal_cursor)->Set();
                _highlight = value;
            }
        }

        void hook(sdls::Character &character)
        {
            character.highlight = std::bind(&Link::highlight, this, std::placeholders::_1);
            character.click = _action;
        }

        auto dimensions() const
        {
            return _dimensions;
        }

    private:
        std::shared_ptr<sdl::Font> _font;
        std::shared_ptr<sdl::Renderer> _renderer;
        std::shared_ptr<sdl::Texture> _texture;
        bool _highlight{false};
        sdl::Cursor _normal_cursor;
        std::unique_ptr<sdl::Cursor> _hand_cursor;
        std::function<void()> _action;
        SDL_Rect _dimensions;
    };
} // namespace controls
