#include "Collider.h"


bool Collider::intersects(const Collider& other) const
{
	for (const auto& mycollider : colliders) {
		for (const auto& collider : other.colliders) {
			if (collider.intersects(mycollider)) return true;
		}
	}
	return false;
}

void Collider::update() {

	if (pre_pos == parent_pos) return;

	for (auto&& elm : colliders) elm.moveBy(parent_pos - pre_pos);

	pre_pos = parent_pos;
}