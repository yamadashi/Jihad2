#pragma once
# include "Collider.h"


class Bullet {
private:
	
protected:
	Collider collider;

public:
	Bullet();
	virtual ~Bullet() = default;
	virtual void update() = 0;

};


class Eye : public Bullet {


};

class Sputum : public Bullet {


};