#pragma once
#include "Connection.hpp"
#include "Storage.hpp"
#include <thread>
#include<future>

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
	Storage storage("Resource");
	std::vector<uint8_t> buffer{ 1,2,3,4,5 };
	storage.AddFile("test", buffer);
	return 0;
}





