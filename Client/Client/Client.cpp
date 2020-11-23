#include "Winsock2.h"
#pragma comment (lib, "ws2_32.lib")
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <ctime>
#pragma warning(disable:4996)
SOCKET Connection;
void LgSent(std::string s)
{
	char time[50], date[50];
	std::ofstream log("C:\\Users\\User\\DeskTop\\LogClient.txt", std::ios_base::app);
	log << "Sent " << s << ". Date " << _strdate(date) << ", time " << _strtime(time) << "\n\n";
	log.close();
}
void LgGot(std::string s)
{
	char time[50], date[50];
	std::ofstream log("C:\\Users\\User\\DeskTop\\LogClient.txt", std::ios_base::app);
	log << "Got " << s << ". Date " << _strdate(date) << ", time " << _strtime(time) << "\n\n";
	log.close();
}
int main()
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		std::cout << "Error" << std::endl;
		exit(1);
	}
	SOCKADDR_IN addr;
	int size = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1045);
	addr.sin_family = AF_INET;
	 Connection = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(Connection, (SOCKADDR*)&addr, size) != 0)
	{
		std::cout << "Error: failed to connect the server.\n";
		return 1;
	}
	std::cout << "Connected!\n";
	char msg[256];
	recv(Connection, msg, sizeof(msg), NULL);
	std::cout << msg << std::endl;
	char msg1[256]="";
	while (true)
	{
		std::cin.getline(msg1, sizeof(msg1));
		send(Connection, msg1, sizeof(msg1), NULL);
		LgSent(msg1);
		recv(Connection, msg1, sizeof(msg1), NULL);
		LgGot(msg1);
		std::cout << msg1<<std::endl;
		if (std::strcmp(msg1, "Invalid Command")==0) continue;
		recv(Connection, msg1, sizeof(msg1), NULL);
		LgGot(msg1);
		std::cout << msg1 << std::endl;
	}
	system("pause");
	return 0;
}
