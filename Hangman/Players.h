#pragma once
#include <string>
using namespace std;
class Players {
private:
	int playerID;
	string name;
	int score=0;
public:
	Players() {};
	Players(int id, string n, int s) {
		playerID = id;
		name = n;
		score = s;
	}
	void setPlayerID(int playerID) {
		this->playerID = playerID;
	}
	int getPlayerID() {
		return playerID;
	}
	void setName(string name) {
		this->name = name;
	}
	string getName() {
		return name;
	}
	void setScore(int score) {
		this->score = score;
	}
	int getScore() {
		return score;
	}
};