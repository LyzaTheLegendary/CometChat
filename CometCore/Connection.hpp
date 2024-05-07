#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdint.h>
#include <queue>
#include <memory>
#include <mutex>
#include <future>
#include "Message.hpp"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
class Connection
{
public:
	//	 This function initializes the Winsock2 api and readies the socket
	static void Init(void(*onReceive)(Message msg), void(*onConnect)(), void(*onDisconnect)());
	static void Deinit();
	static bool Connect(const char* address, const char* port);
	static void Disconnect(const bool reconnect = false);
	static void StartOutQueue();
	static void Listen();
	static void PendMessage(const Message msg);
private:
	static WSADATA m_wsaData; // assign in source file
	static SOCKET m_socket; // Sockets can only be used in their own thread, So they should be mutexed and cannot be async!
	static volatile bool m_connected;

	static std::queue<Message> m_outQueue; // This queue will be mutex locked so everything stays in it's own thread.
	static std::mutex m_mutex;
	//static std::lock_guard<std::mutex> m_queueLock;
	static std::condition_variable m_condition;

	static void(*m_onReceive)(Message);
	static void(*m_onConnect)();
	static void(*m_onDisconnect)();
};

