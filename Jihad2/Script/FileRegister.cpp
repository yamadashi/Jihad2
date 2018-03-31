# include "FileRegister.h"


const std::vector<FilePath>& FileRegisterer::getPaths(const FilePath dir_name, const bool scanLowerHierarchy)
{
	const auto&& contents = FileSystem::DirectoryContents(dir_name);

	for (const auto& content : contents)
	{
		if (FileSystem::IsFile(content))
		{
			filePaths.emplace_back(content);
		}
		else if (scanLowerHierarchy && FileSystem::IsDirectory(content))
		{
			getPaths(content, scanLowerHierarchy);
		}
	}

	return filePaths;
}


FileRegisterer& TextureRegisterer::fileRegister(const FilePath& directory, const bool scanLowerHierarchy) {

	const auto& filePaths = getPaths(directory, scanLowerHierarchy);

	for (const auto& elm : filePaths) {
		String&& extension = FileSystem::Extension(elm);
		if (extension == L"png" || extension == L"jpeg" || extension == L"jpg")
			TextureAsset::Register(FileSystem::BaseName(elm), elm, TextureDesc::Mipped);
	}
	return *this;
}


FileRegisterer& SoundRegisterer::fileRegister(const FilePath& directory, const bool scanLowerHierarchy) {

	const auto& filePaths = getPaths(directory, scanLowerHierarchy);

	for (const auto& elm : filePaths) {
		String&& extension = FileSystem::Extension(elm);
		if (extension == L"mp3" || extension == L"wav")
			SoundAsset::Register(FileSystem::BaseName(elm), elm);
	}
	return *this;
}


FileRegisterer& AssetRegisterer::fileRegister(const FilePath& directory, const bool scanLowerHierarchy) {

	const auto& filePaths = getPaths(directory, scanLowerHierarchy);

	for (const auto& elm : filePaths) {
		String&& extension = FileSystem::Extension(elm);
		if (extension == L"png" || extension == L"jpeg" || extension == L"jpg") 
			TextureAsset::Register(FileSystem::BaseName(elm), elm, TextureDesc::Mipped);
		else if (extension == L"mp3" || extension == L"wav")
			SoundAsset::Register(FileSystem::BaseName(elm), elm);
	}
	return *this;
}