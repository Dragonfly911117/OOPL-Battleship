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

namespace robotEnums {
	enum difficulty {
		noob, normal, hard
	};

	enum direction {
		up, down, left, right
	};
}

class Robot {
	CPoint _lastCoordinate;
	robotEnums::difficulty _difficulty = robotEnums::normal;
	robotEnums::direction _lastDirection = robotEnums::direction::up;
	bool _vertFlag = false;
	std::stack<CPoint> _dfsStack;
	std::unordered_set<CPoint> _map;
	static CPoint randomlyPickCoordinate();

public:
	CPoint noobModeFire();  // fire randomly
	CPoint normalModeFire();// if hit, fire its adjacent coordinates 'tll miss
	CPoint hardModeFire();  // have some additional strategies TODO: search for strategies
	void ifLastFireHits(const bool& flag);
	void getFeedback(const bool& res);
	robotEnums::difficulty getDifficulty() const;
};
