#include <memory>

#include "sdladapters.h"
#include "scene.h"

int main()
{
    sdl::Window window("Run!", 700, 450);
    sdl::EventPump pump;
    auto renderer = std::make_shared<sdl::Renderer>(window);
    sdls::Scene scene(renderer);
    scene.background("IMG_6110.jpg");
    pump.run([&]() { scene.render(); }, 16, [](auto ev) {});
    return 0;
}