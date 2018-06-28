#pragma once
#include "GIFData.h"


//エフェクトがGIFアニメの場合と連結画像の場合
class Strategy {
protected:
	const Point leftUpPos;
	double scale;
	double speedCoeff;

public:
	Strategy(const Point& pos, double scale, double speedRatio)
		:leftUpPos(pos),
		scale(scale),
		speedCoeff(1.0 / speedRatio)
	{}
	virtual bool update() = 0; //killするときtrueを返す
	virtual void draw() = 0;
};


//GIFアニメの場合
class AnimationGIFStrategy : public Strategy {
private:
	const AnimationGIFData data;
	size_t t;

public:
	AnimationGIFStrategy(const FilePath& path, const Point& pos, double scale, double speedRatio);
	AnimationGIFStrategy(const AnimationGIFData& data, const Point& pos, double scale, double speedRatio);
	bool update() override;
	void draw() override;
};


//連結画像の場合
class LinkedImageStrategy : public Strategy {
private:
	Texture texture;
	class Impl;
	std::shared_ptr<Impl> pImpl;

public:
	LinkedImageStrategy(const FilePath& path, const size_t cellWidth, const Point& pos, double scale, double speeRatio);
	bool update() override;
	void draw() override;
};