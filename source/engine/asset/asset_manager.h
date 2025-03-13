#pragma once
#include <unordered_set>

#include "../utils/utils.h"
#include "asset_common.h"


namespace engine
{
    struct ProjectConfig
    {
        // Project stem name, not include period and suffix.
        std::u16string projectName;

        // Project file absolute path file in this computer, this is runtime generate value.
        std::u16string projectPath;

        // Misc project folder path.
        std::u16string rootPath;
        std::u16string assetPath;
        std::u16string configPath;
        std::u16string logPath;
        std::u16string cachePath;
    };
    
    class AssetManager : public IRuntimeModule
    {
    public:
        explicit AssetManager(Engine* engine) : IRuntimeModule(engine) { }
        virtual ~AssetManager() = default;

        virtual void registerCheck(Engine* engine) override;
        virtual bool init() override;
        virtual bool tick(const RuntimeModuleTickData& tickData) override;
        virtual bool beforeRelease() override;
        virtual bool release() override;

        const ProjectConfig& getProjectConfig() const { return m_projectConfig; }
        void setupProject(const std::filesystem::path& inProjectPath);

    private:

    protected:
        bool m_bProjectLoaded = false;

        ProjectConfig m_projectConfig;

        mutable std::recursive_mutex m_mutex; // recursive_mutex 允许同一线程多次加锁

        std::unordered_set<UUID> m_dirtyAssetIds;

        // Map store all engine assets.
        std::unordered_map<UUID, std::shared_ptr<AssetInterface>> m_assets;
    };
}