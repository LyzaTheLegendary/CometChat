#include "Connection.hpp"
WSADATA Connection::m_wsaData;
SOCKET Connection::m_socket;
volatile bool Connection::m_connected;
std::queue<Message> Connection::m_outQueue;
std::mutex Connection::m_mutex;
std::condition_variable Connection::m_condition;

void(*Connection::m_onReceive)(Message);
void(*Connection::m_onConnect)();
void(*Connection::m_onDisconnect)();

void Connection::Init(void(*onReceive)(Message msg), void(*onConnect)(), void(*onDisconnect)())
{
	// Will not work on unix, as in according to the unix standards the sockets a uints, In windows they're signed
	Connection::m_socket = INVALID_SOCKET; 

	Connection::m_onReceive = onReceive;
	Connection::m_onConnect = onConnect;
	Connection::m_onDisconnect = onDisconnect;

	errno_t result = WSAStartup(MAKEWORD(2, 2), &Connection::m_wsaData);

	if (result != 0) {
		throw std::exception(std::system_category().message(result).c_str());
	}

}

void Connection::Deinit()
{
	Disconnect(false);
	WSACleanup();
}

bool Connection::Connect(const char* address, const char* port)
{
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	errno_t errcode = getaddrinfo(address, port, &hints, &result);

	if (errcode != 0) {
		// TODO:
		//	std::system_category().message(errcode).c_str() create logging
		return false;
	}

	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_socket == INVALID_SOCKET) {
		// TODO:
		//	std::system_category().message(WSAGetLastError()).c_str() create logging
	}

	errcode = connect(m_socket, result->ai_addr, static_cast<int>(result->ai_addrlen));

	if (errcode != 0) {
		// TODO:
		//	std::system_category().message(errcode).c_str() create logging // should be macro
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return false;
	}
	
	m_connected = true;
	m_onConnect();
	return true;
}

void Connection::Disconnect(const bool reconnect)
{
	if (reconnect) {
		shutdown(m_socket, 2);
		return;
	}

	m_connected = false;
	shutdown(m_socket, 2);
	closesocket(m_socket);
}

void Connection::StartOutQueue()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	while (m_connected) {
		m_condition.wait(lock);
	
		std::vector<uint8_t> bytes = m_outQueue.front().GetBytes();
		m_outQueue.pop();

		int bytesSent = send(m_socket, reinterpret_cast<const char*>(bytes.data()), bytes.size(),0);

		if (bytesSent == 0) {
			Disconnect(false);
			continue;
		}


	}

}
void Connection::Listen() {

	while (m_connected) {
		Header header(0,0);
		int32_t receivedBytes = 0;
		receivedBytes = recv(m_socket, reinterpret_cast<char*>(&header), sizeof(header), NULL);

		if (receivedBytes != sizeof(header)) {
			//TODO: logging
			Disconnect(false);
			break;
		}

		std::vector<uint8_t> buffer(header.GetSize());
		receivedBytes = recv(m_socket, reinterpret_cast<char*>(buffer.data()), buffer.size(), NULL);

		if (receivedBytes != buffer.size()) {
			//TODO: logging
			Disconnect(false);
			break;
		}

		// Have to not call it's deconstructor somehow?
		std::future<void> future = std::async(std::launch::async, m_onReceive, Message(header.GetId(), buffer));
	}
	// if we leave the loop that means something went wrong OR we called a disconnect function so it is not connected anymore!
	m_onDisconnect();
}

void Connection::PendMessage(const Message msg)
{
	m_outQueue.push(msg);
	m_condition.notify_one();
}
