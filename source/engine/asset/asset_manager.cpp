#include "asset_manager.h"


namespace engine
{
	AssetManager* getAssetManager( )
	{
		return nullptr;
	}

	void AssetManager::setupProject(const std::filesystem::path& inProjectPath)
	{
		//ZoneScopedN("vAssetManager::setupProject(const std::filesystem::path&)");

		m_bProjectSetup = true;

		m_projectConfig.rootPath        = inProjectPath.parent_path().u16string();
		m_projectConfig.projectFilePath = inProjectPath.u16string();
		m_projectConfig.projectName     = inProjectPath.filename().replace_extension().u16string();
		m_projectConfig.assetPath       = (inProjectPath.parent_path() / "asset" ).u16string();
		m_projectConfig.cachePath       = (inProjectPath.parent_path() / "cache" ).u16string();
		m_projectConfig.configPath      = (inProjectPath.parent_path() / "config").u16string();
		m_projectConfig.logPath         = (inProjectPath.parent_path() / "log"   ).u16string();

		// Scan whole asset folder to setup asset uuid map.
		setupProjectRecursive(m_projectConfig.assetPath);
	}

	void AssetManager::insertAsset(const UUID& uuid, std::shared_ptr<AssetInterface> asset, bool bCareDirtyState)
	{
		std::filesystem::path savePath = asset->getSaveInfo().getStorePath();

		m_assets[uuid] = asset;
		m_assetTypeMap[savePath.extension().string()].insert(uuid);

		if (asset->isDirty() && bCareDirtyState)
		{
			m_dirtyAssetIds.insert(uuid);
		}
	}

	void AssetManager::setupProjectRecursive(const std::filesystem::path& path)
	{
		//ZoneScopedN("AssetManager::setupProjectRecursive(const std::filesystem::path&)");
		const bool bFolder = std::filesystem::is_directory(path);
		if (bFolder)
		{
			for (const auto& entry : std::filesystem::directory_iterator(path))
			{
				setupProjectRecursive(entry);
			}
		}
		else
		{
			const auto extension = path.extension().string();
			if (extension.starts_with(".dark"))
			{
				tryLoadAsset(path);
			}
		}
	}
}


