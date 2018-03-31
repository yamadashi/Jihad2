#pragma once
# include <Siv3D.hpp>


class FileRegisterer {
public:
	FileRegisterer() = default;
	virtual ~FileRegisterer() = default;
	virtual const FileRegisterer& fileRegister(const FilePath& directory, const bool scanLowerHierarchy = true) = 0;
	virtual void preload() = 0;

private:
	Array<FilePath> filePaths;

protected:
	const std::vector<FilePath>& getPaths(const FilePath dir_name, bool scanLowerHierarchy);

};


class TextureRegisterer final : public FileRegisterer {
public:
	TextureRegisterer() = default;
	~TextureRegisterer() = default;
	FileRegisterer& fileRegister(const FilePath& directory, const bool scanLowerHierarchy = true) override;
	void preload() override { TextureAsset::PreloadAll(); }

};


class SoundRegisterer final : public FileRegisterer {
public:
	SoundRegisterer() = default;
	~SoundRegisterer() = default;
	FileRegisterer& fileRegister(const FilePath& direcotry, const bool scanLowerHierarchy = true) override;
	void preload() override { SoundAsset::PreloadAll(); }

};


class AssetRegisterer : public FileRegisterer {
public:
	AssetRegisterer() = default;
	~AssetRegisterer() = default;
	FileRegisterer& fileRegister(const FilePath& direcotry, const bool scanLowerHierarchy = true) override;
	void preload() override { TextureAsset::PreloadAll(); SoundAsset::PreloadAll();}
	
};