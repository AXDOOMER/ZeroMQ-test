//
//  Echo server in C++
//  Binds REP socket to tcp://*:5555
//  Expects a string from client, replies with the same string that was sent
//
#include <zmq.hpp>
#include <string>
#include <iostream>

int main ()
{
	const unsigned int BUFFER_SIZE = 80;

	// Prepare our context and socket
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind ("tcp://*:5555");

	std::cout << "Echo server started." << std::endl;

	while (true)
	{
		zmq::message_t request;

		// Wait for next request from client
		socket.recv(&request);

		char buffer[BUFFER_SIZE];	// Warn: No server-side buffer size checks done here
		memcpy(buffer, request.data(), BUFFER_SIZE);

		std::cout << "Received: '" << buffer << "'\tlen: " << strlen(buffer) << std::endl;

		//  Send reply back to client
		zmq::message_t reply(strlen(buffer) + 1);
		memcpy (reply.data(), buffer, strlen(buffer)+ 1);
		socket.send(reply);
	}
}
