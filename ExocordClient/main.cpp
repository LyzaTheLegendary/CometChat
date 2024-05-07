#pragma once
#include "MemoryStream.hpp"
#include "FileStream.hpp"
#include "Connection.hpp"
#include <stdio.h>
#include <thread>
#include <windows.h>
#include<future>
#include<fstream>

void OnReceive(Message msg) {
	printf("msg received");
}

void OnConnect() {
	printf("works");
}

void OnDisconnect() {

}

void InitNetwork() {
	Connection::Init(OnReceive, OnConnect, OnDisconnect);
	Connection::Connect("127.0.0.1", "25565");

	std::future<void> future = std::async(std::launch::async, Connection::StartOutQueue);
	Connection::Listen();
	future.wait();
}

int main() {
	//std::fstream fs("Resource/test.txt", std::ios::in);
	//
	//if (fs.is_open()) {
	//	std::string line;
	//	while (std::getline(fs,line)) {
	//		printf(line.c_str());
	//	}
	//}
	return 0;
}





