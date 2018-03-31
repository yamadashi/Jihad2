#include "Wall.h"


Chip::Chip(const State& state_, const Point& pos_)
	:state(state_),
	pos(pos_),
	collider(pos)
{
	collider.add(Rect(pos, (int)size));
}

void Chip::draw() const {
	int state_val = static_cast<int>(state);
	TextureAsset(L"wall")((state_val / 3)*size, (state_val % 3)*size, size, size).draw(pos);
}


const int Chip::size = 75;


Wall::Wall()
	:stdPos(500, 100)
{
	for (int i = 0; i < 10; i++) {
		chips.emplace_back();
		
		chips[i].emplace_back(none);

		if (i == 0 || i == 9) chips[i].emplace_back(none);
		else {
			State state(State::Middle);
			if (i == 1) state = State::Upper;
			else if (i == 8) state = State::Lower;
			chips[i].emplace_back(in_place_t(), state, stdPos.movedBy(0, (i-1)*Chip::size));
		}

		for (int j = 0; j < i; j++) {
			chips[i].emplace_back(none);
		}
	}
}

void Wall::adjust(int row, int col) {
	auto& data = getChips();

	if (!data[row][col].has_value()) return;

	int state_val = 1;
	//¶’†‰E”»’è
	if (data[row][col - 1].has_value()) state_val++;
	if (!data[row][col + 1].has_value()) state_val++;
	state_val *= 3;
	//ã’†‰º”»’è
	if (data[row - 1][col].has_value()) state_val++;
	if (!data[row + 1][col].has_value()) state_val++;

	data[row][col].emplace(static_cast<State>(state_val), stdPos.movedBy((col - 1)*Chip::size, (row - 1)*Chip::size));
}

void Wall::draw() const {
	for (const auto& arr : chips) {
		for (const auto& elm : arr) {
			if (elm.has_value()) elm.value().draw();
		}
	}
}

void Wall::extend(int row, int col) {
	State temp = State::First;
	if (getChips()[row + 1][col].has_value()) temp = State::UpperRight;
	getChips()[row][col].emplace(temp, stdPos.movedBy((col-1)*Chip::size, (row-1)*Chip::size));
	adjust(row + 1, col);
	adjust(row, col - 1);
}