#pragma once
#include <map>
#include <unordered_set>
#include <queue>
#include <stack>

template<>
struct std::hash<CPoint> {
	size_t operator()(const CPoint& p) const;
};

template<>
struct std::equal_to<CPoint> {
	bool operator()(const CPoint& p1, const CPoint& p2) const;
};

namespace robot_enums {
	enum difficulty {
		infinite_monkey, normal, hard
	};

	enum direction {
		up, down, left, right
	};
}

class Robot {
	CPoint _lastCoordinate;
	robot_enums::difficulty _difficulty = robot_enums::hard;
	robot_enums::direction _lastDirection = robot_enums::direction::up;
	bool _vertFlag = false;
	std::stack<CPoint> _dfsStack;
	std::unordered_set<CPoint> _map;
	static CPoint randomlyPickCoordinate();

public:
	CPoint infiniteMonkeyModeFire(); // fire randomly Reference: https://en.wikipedia.org/wiki/Infinite_monkey_theorem
	CPoint normalModeFire(); // if hit, fire its adjacent coordinates 'tll miss
	CPoint hardModeFire(); // have some additional strategies 
	void getFeedback(const bool& res);
	robot_enums::difficulty getDifficulty() const;
};
