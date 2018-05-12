//
//  Echo client with concatenation in C++
//  Connects REQ socket to tcp://localhost:5555
//  Sends a message to server, expects to received all the previous messages concatenated
//
#include <zmq.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace zmq;

static string s_recv (socket_t& socket) {

    message_t message;
    socket.recv(&message);

    return string(static_cast<char*>(message.data()), message.size());
}

// Multiple sync REQ talking to async ROUTER

int main ()
{
	// Identity must be unique between clients
	string identity;
	cout << "Enter your identity: ";
	getline(cin, identity);

	// Prepare our context and socket
	context_t context (1);
	socket_t socket (context, ZMQ_REQ);	// ZMQ_REQ is sync request
	socket.setsockopt(ZMQ_IDENTITY, identity.c_str(), identity.size());

	cout << "Connecting to server..." << endl;
	socket.connect ("tcp://localhost:5555");

	// Keep track of the number of requests
	int Count = 0;

	while (true)
	{
		string message;
		cout << "Your input: ";
		getline(cin, message);

		message_t request(message.size() + 1);
		memcpy (request.data(), message.c_str(), message.size() + 1);

		socket.send(request);

		// Wait for reply
		string rep = s_recv(socket);

		cout << "Response " << ++Count << ": " << rep << "\t(len:" << rep.size() << ")" << endl;
	}
}
