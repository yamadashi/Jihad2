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

void Collider::add(const Rect& arg) {
	colliders.emplace_back(arg);
	if (!collider.has_value()) collider.emplace(colliders.at(0));
}

void Collider::update() {

	if (pre_pos == parent_pos) return;

	delta.set(parent_pos - pre_pos);
	for (auto&& elm : colliders) elm.moveBy(delta);

	pre_pos = parent_pos;
}
