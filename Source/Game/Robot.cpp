#include "stdafx.h"
#include "Robot.h"
#include <random>

size_t std::hash<CPoint>::operator()(const CPoint& p) const {
	return hash<int>()(p.x) ^ hash<int>()(p.y);
}

bool std::equal_to<CPoint>::operator()(const CPoint& p1, const CPoint& p2) const {
	return p1.x == p2.x && p1.y == p2.y;
}

CPoint Robot::randomlyPickCoordinate() {
	CPoint pt;
	std::random_device rd;
	std::default_random_engine generator(rd());
	const std::uniform_real_distribution<float> uni(0.0, 10.0f);
	pt.x = static_cast<int>(uni(generator));
	pt.y = static_cast<int>(uni(generator));
	return pt;

}

CPoint Robot::noobModeFire() {
	CPoint pt = randomlyPickCoordinate();
	while (_map.find(pt) != _map.end()) {
		pt = randomlyPickCoordinate();
	}
	_map.insert(pt);
	_lastCoordinate = pt;
	return pt;
}

CPoint Robot::normalModeFire() {
	if (_dfsStack.empty()) {
		CPoint pt = randomlyPickCoordinate();
		_lastCoordinate = pt;
		return pt;
	} else {
		const CPoint pt = _dfsStack.top();
		_dfsStack.pop();
		_lastCoordinate = pt;
		return pt;
	}
}

CPoint Robot::hardModeFire() {
	return CPoint();
}

void Robot::ifLastFireHits(const bool& flag) {
	if (flag) {
		_dfsStack.push(_lastCoordinate);
	}
}

void Robot::getFeedback(const bool& res) {
	if (res) {
		if (_dfsStack.empty()) {
			_dfsStack.push(_lastCoordinate);
			_dfsStack.push(CPoint(_lastCoordinate.x, _lastCoordinate.y - 1));
			_lastDirection = robotEnums::up;
			_vertFlag = false;
		} else if (_lastDirection == robotEnums::up) {
			_dfsStack.push(CPoint(_lastCoordinate.x, _lastCoordinate.y - 1));
			_vertFlag = true;
		} else if (_lastDirection == robotEnums::down) {
			_dfsStack.push(CPoint(_lastCoordinate.x, _lastCoordinate.y + 1));
			_vertFlag = true;
		} else if (_lastDirection == robotEnums::left) {
			_dfsStack.push(CPoint(_lastCoordinate.x - 1, _lastCoordinate.y));
		} else if (_lastDirection == robotEnums::right) {
			_dfsStack.push(CPoint(_lastCoordinate.x + 1, _lastCoordinate.y));
		}
	}
	else {
		if (_dfsStack.empty()) {
			return;
		}
		if (_lastDirection == robotEnums::up) {
			_dfsStack.push(CPoint(_dfsStack.top().x, _dfsStack.top().y + 1));
			_lastDirection = robotEnums::down;
		} else if (_lastDirection == robotEnums::down && !_vertFlag) {
			_dfsStack.push(CPoint(_dfsStack.top().x - 1, _dfsStack.top().y));
			_lastDirection = robotEnums::left;
		} else if (_lastDirection == robotEnums::left) {
			_dfsStack.push(CPoint(_dfsStack.top().x + 1, _dfsStack.top().y));
			_lastDirection = robotEnums::right;
		} else if (_lastDirection == robotEnums::right) {
			_dfsStack.pop();
			_lastDirection = robotEnums::up;
		}
	}
}

	robotEnums::difficulty Robot::getDifficulty() const
	{
		return _difficulty;
	}
