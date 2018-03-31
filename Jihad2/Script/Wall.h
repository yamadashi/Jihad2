#pragma once
# include "Collider.h"


enum class State {
	Upper, Middle, Lower,
	UpperLeft, MiddleLeft, LowerLeft,
	UpperMiddle, Center, LowerMiddle,
	UpperRight, MiddleRight, LowerRight,
	First
};



class Chip {
private:
	State state;
	Point pos;
	Collider collider;

public:
	Chip(const State& , const Point&);
	~Chip() = default;

	const Point& getPos() const { return pos; }
	const Collider& getCollider() const { return collider; }
	void draw() const;

	static const int size;
};



class Wall {
private:
	vector<vector<Optional<Chip>>> chips;
	const Point stdPos;
	void adjust(int row, int col);

public:
	Wall();
	~Wall() = default;
	
	void draw() const;
	vector<vector<Optional<Chip>>>& getChips() { return chips; }
	void extend(int row, int col);
};