#pragma once
#include "Hangman.cpp"
#include "gameRoom.h"
#include <time.h>
#include <list>
#include "Client.h"
#include <sstream>
using namespace std;
class Game {
private:
	int tries = 0;
	bool win = false;
	string guesses;
public:
	gameRoom room;
	Draw box;
	Players me;
	Client client;
	int triesLeft(string word, string guesses) {//checks if the tries are errors or not
		int error = 0;
		for (int i = 0; i < guesses.length(); i++) {
			if (word.find(guesses[i]) == string::npos) {
				error++;
			}
		}
		return error;
	}
	void lobby() {
		system("cls");
		box.printMessage("Waiting lobby");
		stringstream ss;
		ss << room.getRoomID();
		string s = client.Send("5", ss.str());
		vector<string>v = split(s, '|');
		if (v.size() == 2 && v[0] == "5") {
			cout << v[1];
		}
		getRoomInfo(room.getRoomID());
		s = client.Send("6", ss.str());
		while (1) {
			string receive = client.Receive();
			if (receive == "") {

			}
			else {
				vector<string>v = split(receive, '|');
				system("cls");
				box.printMessage("Waiting lobby");
				if (v.size() == 2 && v[0] == "5") {
					cout << v[1];
				}
				else if (v.size() == 2 && v[0] == "8") {
					generateGame();
					return;
				}
				else if (v.size() == 3 && v[0] == "2") {
					system("cls");
					return;
					//DestroyRoom();
				}
			}
		}
		box.printMessage("", false, true);
	}
	void getPlayersInfo(int roomID) {
		stringstream ss, ss1;
		ss << roomID;
		string s = client.Send("9", ss.str());
		vector<string>i = split(s, '|');
		if (i.size() == 2) {
			vector<string>v = split(i[1], '^');
			for (auto j : v) {
				vector<string>k = split(j, ',');
				Players player;
				player.setPlayerID(stoi(k[0]));
				player.setName(k[1]);
				player.setScore(stoi(k[2]));
				room.addPlayers(player);
			}
		}
	}
	void printPlayers() {
		box.printMessage("Players in the room");
		for (auto v : room.getPlayers()) {
			stringstream ss;
			ss << "ID:" << v.getPlayerID() << ", Name:" << v.getName() << ", Score:" << v.getScore();
			box.printMessage(ss.str(), false, false);
		}
		box.printMessage("", false, true);
	}
	void getRoomInfo(int roomID) {
		stringstream ss;
		ss << roomID;
		string s = client.Send("7", ss.str());
		vector<string>i = split(s, '|');
		if (i.size() == 5) {
			if (i[0] == "7") {
				room.setOwnerName(i[2]);
				room.setMaxP(stoi(i[3]));
				room.setDifficulty(stoi(i[4]));
			}
		}
		else if (i.size() == 2) {
			cout << i[1] << endl;
			system("pause");
			system("exit");
		}
	}
	void printRoom() {
		box.printMessage("Room information");
		stringstream ss, ss1, ss2, ss3;
		ss << "Room ID: " << room.getRoomID();
		box.printMessage(ss.str(), false, false);
		ss1 << "Room owner: " << room.getOwnerName();
		box.printMessage(ss1.str(), false, false);
		ss2 << "Max players: " << room.getMaxP();
		box.printMessage(ss2.str(), false, false);
		if (room.getDifficulty() == 1) {
			ss3 << "easy";
		}
		else if (room.getDifficulty() == 0) {
			ss3 << "normal";
		}
		else if (room.getDifficulty() == 2) {
			ss3 << "hard";
		}
		box.printMessage("Difficulty: " + ss3.str(), false, false);
		box.printMessage("", false, true);
	}

	void readyToPlay() {
	
	}

	void generateGame() {//generates the game using the hangman.cpp file
		Letters l;
		Word w;
		string word ;
		//w.randomWord();
		getPlayersInfo(room.getRoomID());
		do {
			system("cls");
			box.printMessage("Hang man");
			box.draw(tries);
			l.availableLetters(guesses);
			box.printMessage("Guess the word");
			win = w.printWordAndCheckWin(word, guesses);
			printPlayers();
			printRoom();
			if (win) {
				box.printMessage("You won!");
				break;
			}
			char x;
			cout << ">"; cin >> x;
			if (guesses.find(x) == string::npos) {
				guesses += x;
			}
			/*if (guesses.find((int)x) == 2) {
				DestroyRoom();
			}*/
			tries = triesLeft(word, guesses);
		} while (tries < 10);
		if (!win) {
			box.printMessage("You lost!");
			box.printMessage("The word was " + word, false);
		}
	}
	vector<string> split(const string& s, char delim) {
		vector<string> result;
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			result.push_back(item);
		}

		return result;
	}
	void CreateRoom(SOCKET sock) {
		int maxP;
		int gamemode;
		cout << "Enter the max number of players for the room (2-4): ";
		cin >> maxP;
		if (maxP < 2 || maxP > 4) {
			cout << "ERROR:\n The number of players can be between 2 and 4.\n Try again: ";
			cin >> maxP;
		}
		cout << "Enter the difficulty of the game: ";
		cin >> gamemode;
		stringstream ss;
		ss << maxP << "|" << gamemode;
		string s = client.Send("1", ss.str());
		vector<string>v = split(s, '|');
		if (v.size() == 3) {
			if (v[1] == "ok") {
				room.setRoomID(stoi(v[2]));
				lobby();
			}
		}
		else {
			cout << "ERROR: Bad respond" << endl;
		}


	}
	/*void DestroyRoom(SOCKET sock) { //work in progress
		string s = client.Send("2", "");
		vector<string> v = split("2", '|');
		if (v.size() == 3) {
			if (v[1] == "ok") {
			cout<<"The room was destroyed."<<endl;
				system("pause");
				return;
			}
			else {
				cout << "You don't have permission to do that!";
			}
		}
	}*/
	void JoinRoom(SOCKET sock) {
		string s = client.Send("3", "");
		vector<string> v = split(s, '|');
		if (v.size() == 2) {
			cout << v[1] << endl;
			if (v[1] == "No rooms.\n") {
				system("pause");
				return;
			}
			cout << "Enter room ID: ";
			int ID;
			cin >> ID;
			stringstream ss;
			ss << ID;
			string s = client.Send("4", ss.str());
			vector<string> v = split(s, '|');
			if (v.size() == 2) {
				if (v[1] == "ok") {
					room.setRoomID(ID);
					lobby();
				}
				else {
					cout << v[1] << endl;
					system("pause");
					return;
				}
			}

		}
		else {
			cout << "ERROR: " << endl;
			getchar();
		}
	}
	void login() {
		do {
			system("cls");
			char buf[4096];
			string userInput;
			cout << "Enter your username: ";
			getline(cin, userInput);
			if (userInput.size() > 0) {
				string s = client.Send("0", userInput);
				vector<string>v = split(s, '|');
				if (v.size() == 3) {
					if (v[1] == "ok") {
						me.setName(userInput);
						me.setPlayerID(stoi(v[2]));
						break;
					}
					else {
						cout << "ERROR: " << buf;
						getchar();
					}
				}
			}
		} while (true);
	}
	void run() {
		//77.71.18.59
		//192.168.0.101
		//192.168.100.7



		//да се добави в менюто опция с команда 102 за добавяне на дума
		//да се добави опция в менюто с команда 103 за премахване на дума



		system("cls");
		string ipAddress = "192.168.100.7";
		int port = 50501;
		client.init(ipAddress, port);
		login();
		do {
			int i = 0;
			cout << "1.Create room." << endl;
			cout << "2.Join room." << endl;
			cout << "3.Exit." << endl;
			cout << "Enter the option that you want: ";
			cin >> i;
			switch (i) {
			case 1:
				CreateRoom(client.sock);
				break;
			case 2:
				JoinRoom(client.sock);
				break;
			case 3:
				return;
				break;
			}
		} while (true);
		client.close();
	}
};