//
//  Chat server in C++
//  Binds ZMQ_ROUTER socket to tcp://*:5555
//  Receives messages from clients. Sends an history of messages when it receives a dot.
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <deque>
#include <utility>

using namespace std;
using namespace zmq;

const size_t MAX_STACK_SIZE = 20;

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

	deque<pair<string, string> > messages;

	while (true)
	{
		//  Next message gives us least recently used worker
		string identity = s_recv(socket);
		string delimiter = s_recv(socket);	// Envelope delimiter
		string message = s_recv(socket);	// Response

		if (message.size() == 2 && message[0] == '.')
		{
			string stack;
			for (size_t i = 0; i < messages.size(); i++)
			{
				stack += messages[i].first + ": " + messages[i].second + "\n";
			}

			// Send back the list of messages to the client
			s_sendmore(socket, identity);
			s_sendmore(socket, "");
			s_send(socket, stack);
		}
		else
		{
			messages.push_back(make_pair(identity, message));

			if (messages.size() > MAX_STACK_SIZE)
			{
				messages.pop_front();
			}

			// Send back to the client to acknowledge that a message has been received
			s_sendmore(socket, identity);
			s_sendmore(socket, "");
			s_send(socket, "$");
		}

		cout << identity << ": " << message << endl;
	}
}
