#pragma once
#include <unordered_set>
#include <filesystem>

#include "../utils/utils.h"
#include "asset_common.h"


namespace engine
{
    struct ProjectConfig
    {
        // Project stem name, not include period and suffix.
        std::u16string projectName;

        // Project file absolute path file in this computer, this is runtime generate value.
        std::u16string projectFilePath;

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

        void setupProjectRecursive(const std::filesystem::path& path);

        bool isProjectSetup() const { return m_bProjectSetup; }

    public:
        template<typename T>
        std::weak_ptr<T> getOrLoadAsset(const std::filesystem::path& savePath)
        {
            auto asset = tryLoadAsset(savePath);
            std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(asset);
            return result;
        }


    private:
        std::shared_ptr<AssetInterface> tryLoadAsset(const std::filesystem::path& savePath)
        {
            std::lock_guard<std::recursive_mutex> lock(m_assetManagerMutex);

            // m_assets里缓存了就不需要重新加载
            const AssetSaveInfo saveInfo = AssetSaveInfo::buildRelativeProject(savePath);
            const UUID& uuid = saveInfo.getUUID();
            CHECK(saveInfo.alreadyInDisk());

            if (!m_assets[uuid])
            {
                std::shared_ptr<AssetInterface> asset;
                CHECK(loadAsset(asset, savePath));

                insertAsset(uuid, asset, false);
            }

            return m_assets[uuid];
        }


        void insertAsset(const UUID& uuid, std::shared_ptr<AssetInterface> asset, bool bCareDirtyState);

    protected:
        bool m_bProjectSetup = false;

        ProjectConfig m_projectConfig;

        mutable std::recursive_mutex m_assetManagerMutex; // recursive_mutex 允许同一线程多次加锁

        std::unordered_set<UUID> m_dirtyAssetIds;

        // Map store all engine assets.
        std::unordered_map<UUID, std::shared_ptr<AssetInterface>> m_assets;
        std::unordered_map<std::string, std::unordered_set<UUID>> m_assetTypeMap;
    };

    extern AssetManager* getAssetManager();
}