#pragma once


class Damageable
{
protected:
	int hp;

public:
	Damageable(int hp_) : hp(hp_) {}
	virtual ~Damageable() = default;
	virtual void damage(int val) = 0;

};

