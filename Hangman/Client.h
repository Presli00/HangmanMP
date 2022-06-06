#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
class Client {
private:
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	sockaddr_in hint;
public:
	SOCKET sock;
	void init(string ipAddress, int port) {
		int wsResult = WSAStartup(ver, &data);
		if (wsResult != 0)
		{
			cerr << "Can't start Winsock, Err #" << wsResult << endl;
			return;
		}

		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) {
			cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
			WSACleanup();
			return;
		}
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);


		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR) {
			cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
			closesocket(sock);
			WSACleanup();
			return;
		}
	}
	string Send(string s, string a) {
		char buf[6000];
		ZeroMemory(buf, 6000);
		int bytesReceived;
		stringstream ss;
		ss << s << "|" << a;
		string sendData = ss.str();
		int sendResult;
		sendResult = send(sock, sendData.c_str(), sendData.size() + 1, 0);
		if (sendResult != SOCKET_ERROR) {
			bytesReceived = recv(sock, buf, 6000, 0);
			if (bytesReceived > 0) {
				return buf;
			}
		}
		return "ERROR! Something went wrong!";
	}

	string Receive() {
		char buf[6000];
		ZeroMemory(buf, 6000);
		int bytesReceived;
		stringstream ss;
		bytesReceived = recv(sock, buf, 6000, 0);
		if (bytesReceived > 0) {
			return buf;
		}
		else {
			return "";
		}
	}


	void close() {
		closesocket(sock);
		WSACleanup();
	}
};