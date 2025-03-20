#pragma once
#include "../utils/utils.h"

#include <cereal/access.hpp>

#define ARCHIVE_DECLARE                                                                  \
	friend class cereal::access;                                                         \
	template<class Archive>                                                              \
	void serialize(Archive& archive, std::uint32_t const version);


namespace engine
{
    class AssetSaveInfo
    {
        ARCHIVE_DECLARE;


    public:
        static const u8str kTempFolderStartChar;
        static const u8str kBuiltinFileStartChar;

        AssetSaveInfo() = default;
        explicit AssetSaveInfo(const u8str& name, const u8str& storeFolder);

        static AssetSaveInfo buildTemp(const u8str& name);
        static AssetSaveInfo buildRelativeProject(const std::filesystem::path& savePath);

    public:
        // get functions
        const UUID getUUID() const 
        {
            if (isBuiltin())
            {
                return m_name;
            }
            return m_storePath; 
        }
        const std::u16string getStorePath() const 
        { 
            return utf8::utf8to16(m_storePath); 
        }


        bool isTemp() const;
        bool isBuiltin() const;

    private:
        void updateStorePath( );

    private:

        u8str m_name = {};

        u8str m_storeFolder = {};

        u8str m_storePath = {};
    };


    class AssetInterface : public std::enable_shared_from_this<AssetInterface>
    {
        
    public:
        AssetInterface() = default;
        const AssetSaveInfo& getSaveInfo() const { return m_saveInfo; }
        virtual ~AssetInterface() = default;




        // Asset is dirty or not.
        bool isDirty() const { return m_bDirty; }

    protected:
        virtual bool saveImpl() = 0;
        virtual void unloadImpl() = 0;

    private:
        // Asset is dirty or not.
        bool m_bDirty = false;

    protected:
        AssetSaveInfo m_saveInfo = { };

        // Raw asset path relative to asset folder.
        u8str m_rawAssetPath  {};
    };



    struct AssetCompressionHelper
    {
        int orginalSize;
        int compressionSize;

        template<typename Archive>
        void serialize(Archive& archive)
        {
            archive(originalSize, compressionSize);
        }
    };



    template<typename T>
    bool loadAsset(T& out, const std::filesystem::path& savePath)
    {
        if (!std::filesystem::exists(savePath))
        {
            LOG_ERROR("Asset data {} miss!", utf8::utf16to8(savePath.u16string())); // ×ª»»×Ö·û´®¸ñÊ½
            return false;
        }

        AssetCompressionHelper sizeHelper;
        std::vector<char> compressionData;
        {
            std::ifstream is(savePath, std::ios::binary);
            cereal::BinaryInputArchive archive(is);
            archive(sizeHelper, compressionData);
        }

        std::vector<char> decompressionData()
    }
}
