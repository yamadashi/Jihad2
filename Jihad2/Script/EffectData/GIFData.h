#pragma once
#include <Siv3D.hpp>



//アニメーションGifのデータセット
struct AnimationGIFData {

	AnimationGIFData(const FilePath& path);
	AnimationGIFData();
	void reset(const FilePath& path);
	std::vector<Texture> textures;
	size_t size = 0;

};

class GifReader {
private:
	AnimationGIFData data;
	size_t t;

public:
	GifReader(const FilePath& path);
	void draw();

};