#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <time.h>
#include <fstream>
using namespace std;
class Draw {//is responsible for drawing the box and the parts of the human for every wrong guess
private:
	string message;
	bool top, bottom;
	int guessCount;
public:
	Draw() {
		guessCount = 0;
		top = true;
		bottom = true;
	};
	Draw(string m, int g, bool t, bool b) {
		message = m;
		guessCount = g;
		top = t;
		bottom = b;
	};
	void setMessage(string message) {
		this->message = message;
	}
	string getMessage() {
		return message;
	}
	void setGuessCount(int guessCount) {
		this->guessCount = guessCount;
	}
	int getGuessCount() {
		return guessCount;
	}
	void printMessage(string message, bool top = true, bool bottom = true) { // Чертаене на кутията на играта, подравняване на текста и визуализация на човечето
		if (top) {
			cout << "+-----------------------------------+" << endl;
			cout << "|";
		}
		else {
			cout << "|";
		}
		bool front = true;
		for (int i = message.length(); i < 35; i++) {
			if (front) {
				message = " " + message;
			}
			else {
				message = message + " ";
			}
			front = !front;
		}
		cout << message;
		if (bottom) {
			cout << "|" << endl;
			cout << "+-----------------------------------+" << endl;
		}
		else {
			cout << "|" << endl;
		}
	}
	void draw(int guessCount = 0) { //Чертаене на човечето използвайки група от if-ве и else-ве и извикване на първата функция за визуализация (преработване за работа с вектори)
		vector<string>ch{ "|","O","/","\\" };
		if (guessCount >= 1) {
			printMessage(ch[0], false, false);
		}
		if (guessCount >= 2) {
			printMessage(ch[0], false, false);
		}
		if (guessCount >= 3) {
			printMessage(ch[1], false, false);
		}
		if (guessCount == 4) {
			printMessage(ch[2] + "  ", false, false);
		}
		if (guessCount == 5) {
			printMessage(ch[2] + ch[0] + " ", false, false);
		}
		if (guessCount >= 6) {
			printMessage(ch[2] + ch[0] + ch[3], false, false);
		}
		if (guessCount >= 7) {
			printMessage(ch[0], false, false);
		}
		if (guessCount == 8) {
			printMessage(ch[2] + "  ", false, false);
		}
		if (guessCount >= 9) {
			printMessage(ch[2] + " " + ch[3], false, false);
		}
	}
	friend istream& operator>>(istream& istr, Draw d) {
		istr >> d.message >> d.top >> d.bottom >> d.guessCount;
		return istr;
	}
	friend ostream& operator<<(ostream& ostr, Draw d) {
		ostr << d.message << d.top << d.bottom << d.guessCount << endl;
		return ostr;
	}
};
class Letters :public Draw {//inherits the Draw class and displayes the available letters, while removing the used ones
private:
	string input;
	string taken;
	string word;
public:
	Letters() {};
	Letters(string i, string t, string w) :Draw("", 0, false, false) {
		input = i;
		taken = t;
		word = w;
	}
	void setInput(string input) {
		this->input = input;
	}
	string getInput() {
		return input;
	}
	void setTaken(string taken) {
		this->taken = taken;
	}
	string getTaken() {
		return taken;
	}
	void setWord(string word) {
		this->word = word;
	}
	string getWord() {
		return word;
	}
	void printLetters(string input, char from, char to) { //shows the letters to be used and removes the already used ones
		string s;
		for (char i = from; i <= to; i++) {
			if (input.find(i) == string::npos) {
				s += i;
				s += " ";
			}
			else s += " ";
		}
		printMessage(s, false, false);
	}
	void availableLetters(string taken) {//Визуализация на използваемите букви използвайки горната функция.
		printMessage("Available letters");
		printLetters(taken, 'A', 'M');
		printLetters(taken, 'a', 'm');
		printLetters(taken, 'N', 'Z');
		printLetters(taken, 'n', 'z');
	}
};
class Word :public Draw {//inherits the Draw class and displayes a blank space for the word to be guessed and shows the guessed letters
private:
	string word;
	string guessed;
public:
	Word() {}
	Word(string w, string g) :Draw("", 0, false, false) {
		word = w;
		guessed = g;
	}
	void setWord(string word) {
		this->word = word;
	}
	string getWord() {
		return word;
	}
	void setGuessed(string guessed) {
		this->guessed = guessed;
	}
	string getGuessed() {
		return guessed;
	}
	bool printWordAndCheckWin(string word, string guessed) {//Визуализация на мястото за писане на думата и заместване на празните места с познатите букви(изключване на знаците различни от буквите в ASCI таблицата)
		bool won = true;
		string s;
		for (int i = 0; i < word.length(); i++) {
			if (guessed.find(word[i]) == string::npos) {
				if (word[i] == '-') {
					s += '-';
					s += " ";
				}
				else {
					won = false;
					s += "_ ";
				}
			}
			else {
				s += word[i];
				s += " ";
			}
		}
		printMessage(s, false);
		return won;
	}
	/*string randomWord() {//chosses a random word from a txt file
		srand(time(0));
		int count = 0;
		string word;
		vector<string> v;
		ifstream reader("words2.txt");
		if (reader.is_open()) {
			while (getline(reader, word)) {
				v.push_back(word);
			}
			int randLine = rand() % v.size();
			word = v.at(randLine);
		}
		return word;
		reader.close();
	}*/
};