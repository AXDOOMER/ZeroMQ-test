//
//  Echo server with concatenation in C++
//  Binds ZMQ_ROUTER socket to tcp://*:5555
//  Expects a string from client, replies with the same string concatenated with the previous ones
//
#include <zmq.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace zmq;

static string s_recv (socket_t& socket)
{
	message_t message;
	socket.recv(&message);

	return string(static_cast<char*>(message.data()), message.size());
}

static bool s_send (socket_t& socket, const string& string)
{
	message_t message(string.size());
	memcpy (message.data(), string.data(), string.size());

	bool rc = socket.send (message);
	return (rc);
}

static bool s_sendmore (socket_t& socket, const string& string)
{
	message_t message(string.size());
	memcpy (message.data(), string.data(), string.size());

	bool rc = socket.send (message, ZMQ_SNDMORE);
	return (rc);
}

// Multiple sync REQ talking to async ROUTER

int main ()
{
	// Prepare our context and socket
	context_t context(1);
	socket_t socket(context, ZMQ_ROUTER);	// ZMQ_ROUTER is async reply
	socket.bind ("tcp://*:5555");

	cout << "Server started." << endl;

	string total;	// Will hold the concatenated strings

	while (true)
	{
		//  Next message gives us least recently used worker
		string identity = s_recv(socket);
		string delimiter = s_recv(socket);	// Envelope delimiter
		string message = s_recv(socket);	// Response

		total += message;
		cout << total << endl;

		// Send back the new string to the client
		s_sendmore(socket, identity);
		s_sendmore(socket, "");
		s_send(socket, total);
	}
}
