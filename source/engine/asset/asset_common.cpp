//#include "asset_common.h"
//#include <filesystem>
//
//#include "asset_manager.h"
//
//namespace engine
//{
//	AssetSaveInfo AssetSaveInfo::buildTemp(const u8str& name)
//	{
//		return AssetSaveInfo(name, kTempFolderStartChar + buildUUID());
//	}
//
//	AssetSaveInfo AssetSaveInfo::buildRelativeProject(const std::filesystem::path& savePath)
//	{
//		auto fileName = savePath.filename();
//
//		auto saveFolder = savePath;
//		saveFolder.remove_filename();
//
//		const auto relativePath = buildRelativePathUtf8(getAssetManager()->getProjectConfig().assetPath, saveFolder);
//
//		return AssetSaveInfo(utf8::utf16to8(fileName.u16string()), relativePath);
//	}
//
//	AssetSaveInfo::AssetSaveInfo(const u8str& name, const u8str& storeFolder)
//	{
//		updateStorePath( );
//	}
//
//	void AssetSaveInfo::updateStorePath( )
//	{
//		//ZoneScoped;
//		if (m_storeFolder.starts_with("\\") || m_storeFolder.starts_with("/"))
//		{
//			m_storeFolder = m_storeFolder.erase(0, 1);
//		}
//
//		const std::filesystem::path storeFolder = utf8::utf8to16(m_storeFolder);
//		const std::filesystem::path storeName = utf8::utf8to16(m_name);
//
//		m_storePath = utf8::utf16to8((storeFolder / storeName).u16string());
//	}
//
//
//	bool AssetSaveInfo::isTemp() const
//	{
//		return m_storeFolder.starts_with(kTempFolderStartChar);
//	}
//
//	bool AssetSaveInfo::isBuiltin() const
//	{
//		return isTemp() && m_name.starts_with(kBuiltinFileStartChar);
//	}
//}
//
