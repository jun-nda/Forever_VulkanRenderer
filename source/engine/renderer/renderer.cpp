#include "renderer.h"
#include "../engine.h"
//#include "render_scene.h"
//#include "scene_textures.h"

namespace engine
{
    RendererManager* getRenderer()
    {
        static RendererManager* renderer = Engine::get()->getRuntimeModule<RendererManager>();
        return renderer;
    }
    void RendererManager::registerCheck(Engine* engine)
    {
    }
    bool RendererManager::init()
    {
        return false;
    }
    bool RendererManager::tick(const RuntimeModuleTickData& tickData)
    {
        return false;
    }
    bool RendererManager::beforeRelease()
    {
        return false;
    }
    bool RendererManager::release()
    {
        return false;
    }
    void RendererManager::initWindowCommandContext()
    {
    }
    void RendererManager::destroyWindowCommandContext()
    {
    }
}