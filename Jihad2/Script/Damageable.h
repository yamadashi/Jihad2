#pragma once


class Damageable
{
protected:
	const int max_hp;
	int hp;

public:
	Damageable(int hp_) : max_hp(hp_), hp(max_hp) {}
	virtual ~Damageable() = default;
	virtual void damage(int val) = 0;

};

