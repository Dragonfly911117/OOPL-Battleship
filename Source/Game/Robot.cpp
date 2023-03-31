#include "stdafx.h"
#include "Robot.h"
#include <random>

size_t std::hash<CPoint>::operator()(const CPoint& p) const noexcept {
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

CPoint Robot::infiniteMonkeyModeFire() {
	CPoint pt = randomlyPickCoordinate();
	while (_map.find(pt) != _map.end()) {
		pt = randomlyPickCoordinate();
	}
	_map.insert(pt);
	_lastCoordinate = pt;
	return pt;
}

CPoint Robot::normalModeFire() {
	CPoint pt;
	if (_dfsStack.empty()) {
		pt = randomlyPickCoordinate();
		_lastCoordinate = pt;
	} else {
		pt = _dfsStack.top();
		_lastCoordinate = pt;
		_dfsStack.pop();
	}
	return pt;
}

CPoint Robot::hardModeFire() {
	CPoint pt(-1, -1);
	if (_dfsStack.empty()) {
		if (_map.empty()) {
			pt = randomlyPickCoordinate();
		} else {
			for (auto i = _map.begin(); i != _map.end(); ++i) {
				pt = CPoint(i->x - 1, i->y - 1);// left up
				if (_map.count(pt) == 0 && pt.x >= 0 && pt.y >= 0 && pt.x < 10 && pt.y < 10) {
					break;
				}
				pt = CPoint(i->x + 1, i->y + 1);// right down
				if (_map.count(pt) == 0 && pt.x >= 0 && pt.y >= 0 && pt.x < 10 && pt.y < 10) {
					break;
				}
				pt = CPoint(i->x + 1, i->y - 1);// right up
				if (_map.count(pt) == 0 && pt.x >= 0 && pt.y >= 0 && pt.x < 10 && pt.y < 10) {
					break;
				}
				pt = CPoint(i->x - 1, i->y + 1);// left down
				if (_map.count(pt) == 0 && pt.x >= 0 && pt.y >= 0 && pt.x < 10 && pt.y < 10) {
					break;
				}
			}
		}
		_map.insert(pt);
	} else {
		pt = _dfsStack.top();
		_dfsStack.pop();
	}
	_lastCoordinate = pt;
	return pt;
}

CPoint Robot::darkSoulModeFire() {
	if (_cheatCountdown-- < 0 && !_cheatSheet.empty()) {
		const CPoint pt = _cheatSheet.front();
		_cheatSheet.pop_front();
		_lastCoordinate = pt;
		return pt;
	} else {
		return hardModeFire();
	}

}

void Robot::gatherEnemyShipCoordinates(const std::deque<CPoint>& pt) {
	_cheatSheet = pt;
}

void Robot::getFeedback(const bool& res) {
	if (res) {
		if (_dfsStack.empty()) {
			_dfsStack.push(_lastCoordinate);
			_dfsStack.push(CPoint(_lastCoordinate.x, _lastCoordinate.y - 1));
			_lastDirection = robot_enums::up;
			_vertFlag = false;
		} else if (_lastDirection == robot_enums::up) {
			_dfsStack.push(CPoint(_lastCoordinate.x, _lastCoordinate.y - 1));
			_vertFlag = true;
		} else if (_lastDirection == robot_enums::down) {
			_dfsStack.push(CPoint(_lastCoordinate.x, _lastCoordinate.y + 1));
			_vertFlag = true;
		} else if (_lastDirection == robot_enums::left) {
			_dfsStack.push(CPoint(_lastCoordinate.x - 1, _lastCoordinate.y));
		} else if (_lastDirection == robot_enums::right) {
			_dfsStack.push(CPoint(_lastCoordinate.x + 1, _lastCoordinate.y));
		}
	} else {
		if (_dfsStack.empty()) {
			return;
		}
		if (_lastDirection == robot_enums::up) {
			_dfsStack.push(CPoint(_dfsStack.top().x, _dfsStack.top().y + 1));
			_lastDirection = robot_enums::down;
		} else if (_lastDirection == robot_enums::down && !_vertFlag) {
			_dfsStack.push(CPoint(_dfsStack.top().x - 1, _dfsStack.top().y));
			_lastDirection = robot_enums::left;
		} else if (_lastDirection == robot_enums::left) {
			_dfsStack.push(CPoint(_dfsStack.top().x + 1, _dfsStack.top().y));
			_lastDirection = robot_enums::right;
		} else if (_lastDirection == robot_enums::right) {
			_dfsStack.pop();
			_lastDirection = robot_enums::up;
		}
	}
}

robot_enums::difficulty Robot::getDifficulty() const {
	return _difficulty;
}

void Robot::setDifficulty(const int& i) {
	_difficulty = static_cast<robot_enums::difficulty>(i);
}
