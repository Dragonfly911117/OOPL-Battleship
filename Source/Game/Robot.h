#pragma once
#include <map>
#include <unordered_set>
#include <queue>
#include <stack>

template<>
struct std::hash<CPoint> {
	size_t operator()(const CPoint& p) const noexcept;
};

template<>
struct std::equal_to<CPoint> {
	bool operator()(const CPoint& p1, const CPoint& p2) const;
};

namespace robot_enums {
	enum difficulty {
		infinite_monkey, normal, hard, dark_soul
	};

	enum direction {
		up, down, left, right
	};
}

class Robot {
	const int default_cheat_count = 10;
	CPoint _lastCoordinate;
	robot_enums::difficulty _difficulty = robot_enums::hard;
	robot_enums::direction _lastDirection = robot_enums::direction::up;
	bool _vertFlag = false;
	std::stack<CPoint> _dfsStack;
	std::unordered_set<CPoint> _map;
	int _cheatCount = default_cheat_count;
	std::deque<CPoint> _cheatSheet;

	static CPoint randomlyPickCoordinate();

public:
	CPoint infiniteMonkeyModeFire();// fire randomly Reference: https://en.wikipedia.org/wiki/Infinite_monkey_theorem
	CPoint normalModeFire();        // If hit, fire its adjacent coordinates 'tll miss.
	CPoint hardModeFire();          // have some additional strategies
	CPoint darkSoulModeFire();      // hard mode 'till the _cheatCount is 0, then cheat
	void gatherEnemyShipCoordinates(const std::deque<CPoint>& pt);
	void getFeedback(const bool& res);
	robot_enums::difficulty getDifficulty() const;
	void setDifficulty(const int& i);
	void reset();
};
