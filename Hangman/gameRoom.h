#pragma once
#include "Players.h"
#include <list>
using namespace std;
class gameRoom {
private:
	int roomID;
	list<Players>players;
	bool joined;
	int maxP;
	int difficulty;
	string ownerName;
public:
	gameRoom() {
		joined = false;
	};
	void setRoomID(int roomID) {
		this->roomID = roomID;
	}
	int getRoomID() {
		return roomID;
	}
	void setMaxP(int maxP) {
		this->maxP = maxP;
	}
	int getMaxP() {
		return maxP;
	}
	void setDifficulty(int difficulty) {
		this->difficulty = difficulty;
	}
	int getDifficulty() {
		return difficulty;
	}
	void setOwnerName(string ownerName) {
		this->ownerName = ownerName;
	}
	string getOwnerName() {
		return ownerName;
	}
	void setPlayers(list<Players> players) {
		this->players = players;
	}
	list<Players> getPlayers() {
		return players;
	}
	int getPlayersNumbers() {
		return players.size();
	}
	string addPlayers(Players p) {
		players.push_back(p);
		return "ok";
	}
	void removePlayers(int playerID) {
		list<Players>::iterator i = this->players.begin();
		while (i != this->players.end()) {
			if (i->getPlayerID() == playerID) {
				players.erase(i);
				return;
			}
			i++;
		}
	}
};