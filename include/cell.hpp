#pragma once

class Cell {
private:
	bool alive = false;
public:
	Cell(){}
	bool isAlive() { return alive; }
	void setAlive(bool alive) { this->alive = alive; }
};