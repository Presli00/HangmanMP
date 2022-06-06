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
	Letters l;
	Word w;
	int triesLeft(string word, string guesses) {
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
			}
		}
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
	void readyToPlay(int roomID) {
		stringstream ss;
		ss << roomID;
		string s = client.Send("10", ss.str());
		vector<string>v = split(s, '|');
		if (v.size() == 2) {
			if (v[1] == "ok") {

			}
		}
	}
	void SendLetter(char x) {
		stringstream ss;
		int roomID = room.getRoomID();
		ss << roomID << "|" << x;
		string s = client.Send("14", ss.str());
		CheckLetter(s);
	}
	void WaitLetter() {
		string receive = client.Receive();
		CheckLetter(receive);
	}
	void CheckLetter(string s) {
		vector<string>v = split(s, '|');
		if (v.size() == 7) {
			if (v[0] == "14") {
				if (v[4] == "yes") {
					if (guesses.find(v[3]) == string::npos) {
						guesses += v[3];
					}
					room.updateScore(stoi(v[2]));
					w.setMask(v[5]);
				}
				else {
					if (guesses.find(v[3]) == string::npos) {
						guesses += v[3];
					}
					tries = triesLeft(w.getWord(), guesses);
				}
				room.setEndGameMarker(stoi(v[6]));
			}
		}
	}
	string getPlayerName(int playerID) {
		for (auto v : room.getPlayers()) {
			if (v.getPlayerID() == playerID) {
				return v.getName();
			}
		}
		return "";
	}
	void checkEndGame() {
		system("cls");
		box.printMessage("Hangman", true, true);
		box.draw(tries);
		box.printMessage("", false, false);
		printPlayers();
		box.printMessage("", false, true);
		if (room.getEndGameMarker() == 1) {
			box.printMessage("The man was hanged, no one won the game!");
			box.printMessage("The word was " + w.getWord(), false);
		}
		if (room.getEndGameMarker() == 2) {
			stringstream ss;
			ss << "The player " + room.getWinner().getName() + " has won the game!";
			box.printMessage(ss.str());
		}
		room.reset();
		guesses = "";
		tries = 0;
	}
	void drawGame() {
		do {
			system("cls");
			if (room.getEndGameMarker() != 0) {
				checkEndGame();
				system("pause");
				return;
			}
			box.printMessage("Hang man");
			box.draw(tries);
			box.printMessage("Guess the word");
			box.printMessage(w.getMask(), false, false);
			l.availableLetters(guesses);
			printPlayers();
			printRoom();
			string receive = client.Receive();
			vector<string>v = split(receive, '|');
			if (v.size() == 3) {
				if (v[0] == "13") {
					if (stoi(v[2]) == me.getPlayerID()) {
						box.printMessage("Now is your turn to enter a letter.", false, true);
						char x;
						while (true) {
							cout << ">"; cin >> x;
							if (guesses.find(x) == string::npos) {
								SendLetter(x);
								break;
							}
							else {
								cout << "The letter is already used, try another one! \n";
							}
						}
					}
					else {
						stringstream ss;
						ss << "Waiting for player " << getPlayerName(stoi(v[2])) << " to enter a letter.";
						box.printMessage(ss.str(), false, true);
						WaitLetter();
					}
				}
			}
		} while (true);
	}
	void generateGame() {
		system("cls");
		getPlayersInfo(room.getRoomID());
		readyToPlay(room.getRoomID());
		string receive = client.Receive();
		if (receive == "") {

		}
		else {
			vector<string>v = split(receive, '|');
			if (v.size() >= 2) {
				if (v[0] == "11") {
					w.setWord(v[2]);
					w.setMask(v[3]);
					drawGame();
				}
			}
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
						cout << "ERROR: " << v[1];
					}
				}
			}
		} while (true);
	}
	void addWords() {
		do {
			system("cls");
			char buf[4096];
			string word;
			cout << "Enter a word you want to add to the game: ";
			getline(cin, word);
			if (word.size() > 0) {
				string s = client.Send("102", word);
				vector<string>v = split(s, '|');
				if (v.size() == 2) {
					if (v[1] == "ok") {
						cout << "The word was succesfully added to the game.";
						system("pause");
						break;
					}
					else {
						cout << "ERROR: " << v[1];
						system("pause");
					}
				}
			}
		} while (true);
	}
	void removeWords() {
		do {
			system("cls");
			char buf[4096];
			string word;
			cout << "Enter a word you want to remove from the game: ";
			getline(cin, word);
			if (word.size() > 0) {
				string s = client.Send("103", word);
				vector<string>v = split(s, '|');
				if (v.size() == 2) {
					if (v[1] == "ok") {
						cout << "The word was succesfully removed from the game.";
						system("pause");
						break;
					}
					else {
						cout << "ERROR: " << v[1];
						system("pause");
					}
				}
			}
		} while (true);
	}
	void run() {

		string ipAddress = "192.168.0.103";//servers' ip
		int port = 50501;
		client.init(ipAddress, port);
		login();
		do {
			system("cls");
			int i = 0;
			cout << "1.Create room." << endl;
			cout << "2.Join room." << endl;
			cout << "3.Add a word to the file." << endl;
			cout << "4.Remove a word from the file" << endl;
			cout << "5.Exit." << endl;
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
				addWords();
				break;
			case 4:
				removeWords();
				break;
			case 5:
				return;
				break;
			}
		} while (true);
		client.close();
	}
};
//да се оразмери прозореца