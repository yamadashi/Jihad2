#include "EffectStrategy.h"



AnimationGIFStrategy::AnimationGIFStrategy(const FilePath& path, const Point& pos, double scale, double speedRatio)
	:Strategy(pos, scale, speedRatio),
	data(path), //転送コンストラクタ
	t(0)
{}

AnimationGIFStrategy::AnimationGIFStrategy(const AnimationGIFData& data, const Point& pos, double scale, double speedRatio)
	: Strategy(pos, scale, speedRatio),
	data(data),
	t(0)
{}

bool AnimationGIFStrategy::update() {
	if (++t / speedCoeff >= data.size) return true;
	else return false;
}

void AnimationGIFStrategy::draw() {
	data.textures[t / speedCoeff].scale(scale).draw(leftUpPos);
}



class LinkedImageStrategy::Impl {
public:
	size_t index = 0;
	size_t cellWidth;
	size_t size; //コマの枚数
};

LinkedImageStrategy::LinkedImageStrategy(const FilePath& path, const size_t cellWidth, const Point& pos, double scale, double speedRatio)
	:Strategy(pos, scale, speedRatio),
	pImpl(std::make_shared<Impl>()),
	texture(path)
{
	pImpl->cellWidth = cellWidth;
	pImpl->size = texture.width / cellWidth;
}

bool LinkedImageStrategy::update() {
	if (pImpl->index / speedCoeff >= pImpl->size) return true;
	else return false;
}

void LinkedImageStrategy::draw() {
	size_t index = pImpl->index++ / speedCoeff;
	texture(pImpl->cellWidth*index, 0, pImpl->cellWidth, texture.height).scale(scale).draw(leftUpPos);
}