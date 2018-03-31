#include "Fortress.h"



Fortress::Fortress()
	:pos(50, 450),
	angle(0),
	max_angle(0.2), min_angle(-0.5),
	rotation_point(80,195),
	rotation_velo(0.025),
	collider(Point(pos))
{

}

void Fortress::rotate() {


}


void Fortress::fire() {

	
}


void Fortress::update() {

	if (Input::KeyDown.pressed && angle < max_angle) angle += rotation_velo;
	if (Input::KeyUp.pressed && angle > min_angle) angle -= rotation_velo;

}


void Fortress::draw() const {

	TextureAsset(L"ashi").draw(pos);
	TextureAsset(L"close").rotateAt(rotation_point, angle).draw(pos);

}