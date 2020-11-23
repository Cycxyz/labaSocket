#include "Winsock2.h"
#pragma comment (lib, "ws2_32.lib")
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <ctime>
#include <tuple>
#include <fstream>
#pragma warning(disable:4996)
int genereted_number = 0;
enum class CmndType
{
	WHO, 
	GENERETE_NUMBER,
	SET_TIME, 
	GET_NUMBER
};

void LgSent(std::string s)
{
	char time[50], date[50];
	 std::ofstream log("C:\\Users\\User\\DeskTop\\LogServ.txt", std::ios_base::app);
	log << "Sent " << s <<". Date "<< _strdate(date)<<", time "<<_strtime(time)<< "\n\n";
	log.close();
}
void LgGot(std::string s)
{
	char time[50], date[50];
	std::ofstream log("C:\\Users\\User\\DeskTop\\LogServ.txt", std::ios_base::app);
	log << "Got " << s << ". Date " << _strdate(date) << ", time " << _strtime(time) << "\n\n";
	log.close();
}
std::pair<bool, CmndType> CheckIfCorrect(std::string s, int&n)
{
	std::stringstream stream(s);
	LgGot(s);
	std::string first_word, second_word;
	int number;
	std::ofstream log("logServ.txt", std::ios_base::app);
	if (!(stream >> first_word)) return std::make_pair(false, CmndType::SET_TIME);
	if (first_word == "Who") 
	{
		if (!(stream >> second_word)) 

			return std::make_pair(true, CmndType::WHO);
		
		else return std::make_pair(false, CmndType::GENERETE_NUMBER);
	}
	else if (first_word == "Generete")
	{
		if (!(stream >> second_word)) return std::make_pair(false, CmndType::SET_TIME);
		else if (second_word != "number") return std::make_pair(false, CmndType::SET_TIME);
		else if (stream >> second_word) return std::make_pair(false, CmndType::SET_TIME);
		else return std::make_pair(true, CmndType::GENERETE_NUMBER);
	}
	else if (first_word == "SetTime")
	{
		if(!(stream>>number))  return std::make_pair(false, CmndType::SET_TIME);
		else if (stream >> second_word) return std::make_pair(false, CmndType::SET_TIME);;
		n = number;
		return std::make_pair(true, CmndType::SET_TIME);
	}
	else if (first_word == "Get")
	{
		if(!(stream>>second_word)) return std::make_pair(false, CmndType::SET_TIME);
		else if (second_word != "number") return std::make_pair(false, CmndType::SET_TIME);
		else if(stream>>first_word)   return std::make_pair(false, CmndType::SET_TIME);
		else  return std::make_pair(true, CmndType::GET_NUMBER);
	}
	return 
	std::make_pair(false, CmndType::SET_TIME);
	
}

std::string FindNumber(int interval)
{
	srand(time(NULL));
	time_t tm1;
	tm1 = time(NULL);
	time_t tm2 = time(NULL);
	int difference, memory;
	int a = rand() % 20000;
	Sleep(2000);
	difference = (a - genereted_number >= 0) ? a - genereted_number : genereted_number - a;
	memory = a;
	while (tm2 - tm1 < interval)
	{
		if (difference == 0) return std::to_string(memory)+'\0';
		tm2 = time(NULL);
		int random_number;
		int dif;
		random_number = rand() % 20000;
		dif = (random_number > genereted_number) ? (random_number - genereted_number) : (genereted_number - random_number);
		if (dif < difference)
		{
			difference = dif;
			memory = random_number;
		}
		
	}
	return std::to_string(memory)+'\0';
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

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);
	SOCKET newConnection;
		newConnection = accept(sListen, (SOCKADDR*)&addr, &size);
		if (newConnection == 0)
		{
			std::cout << "Error#2\n";
		}
		else
		{
			std::cout << "Client Connected\n";
			char msg[] = "Avaible commands:\nWho\nGenerete number\nSetTime\nGet number\nGenereted number is 0 now";
			send(newConnection, msg,sizeof( msg), NULL);
		}
			char msg[256]="0";
		while (true)
		{
			int number=0;
			if(recv(newConnection, msg, sizeof(msg), NULL)==-1) continue;
			auto parse = CheckIfCorrect(msg, number);
    			if (!parse.first) 
			{
				const char msg[] = "Invalid Command";
				send(newConnection, msg, sizeof(msg), NULL);
				LgSent(msg);
				continue;
			}
			{
				const char msg[] = "Got Command";
				send(newConnection, msg, sizeof(msg), NULL);
				LgSent(msg);
			}
			if (parse.second == CmndType::GENERETE_NUMBER)
			{
				const char msg[] = "Number is genereted";
				srand(time(NULL));
				genereted_number = rand() % 20000;
				send(newConnection, msg , sizeof(msg), NULL);
				LgSent(msg);
			}
			else if (parse.second == CmndType::GET_NUMBER)
			{
				std::string msg = "Genereted number is "+std::to_string(genereted_number)+'\0';
				send(newConnection, msg.c_str(), msg.size(), NULL);
				LgSent(msg);
				std::cout << "Genereted number is sent\n";
			}
			else if (parse.second == CmndType::WHO)
			{
				const char msg[] = "Burtsevych Vladyslav\nGroup K-25\nVariant - 20";
				send(newConnection, msg, sizeof(msg), NULL);
				LgSent(msg);
				//std::cout << "Info is sent\n";
			}
			else {
				//int a = FindNumber(number);
				std::string a = FindNumber(number);
				send(newConnection,a.c_str(), sizeof(a), NULL);
				LgSent(a);

			}
	    }
	system("pause");
	return 0;
}
