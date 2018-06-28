#include "GIFData.h"



AnimationGIFData::AnimationGIFData(const FilePath& path) {
	for (auto&& image : Imaging::LoadAnimatedGIF(path))
		textures.emplace_back(image);
	size = textures.size();
}

AnimationGIFData::AnimationGIFData()
{}

void AnimationGIFData::reset(const FilePath& path) {
	textures.clear();
	for (auto&& image : Imaging::LoadAnimatedGIF(path))
		textures.emplace_back(image);
	size = textures.size();
}

GifReader::GifReader(const FilePath& path)
	:data(path),
	t(0)
{}

void GifReader::draw() {
	data.textures[t].draw();
	++t %= data.size;
}