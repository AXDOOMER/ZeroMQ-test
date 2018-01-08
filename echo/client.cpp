//
//  Echo client in C++
//  Connects REQ socket to tcp://localhost:5555
//  Sends a message to server, expects the same message back
//
#include <zmq.hpp>
#include <string>
#include <iostream>

int main ()
{
	const unsigned int BUFFER_SIZE = 80;

    // Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to Echo server..." << std::endl;
    socket.connect("tcp://localhost:5555");

    // Keep track of the number of requests
	int Count = 0;

    while (true) {

		std::string message;
		std::cout << "Your input: ";
		getline(std::cin, message);

		if (message.size() >= BUFFER_SIZE)
		{
			message = message.substr(0, BUFFER_SIZE - 1);
			std::cout << "Warning: Message truncated because it was larger than the buffer size." << std::endl;
		}

        zmq::message_t request(message.size() + 1);
        memcpy (request.data(), message.c_str(), message.size() + 1);

        socket.send(request);

        // Get the reply.
        zmq::message_t reply;
        socket.recv(&reply);

		char rep[BUFFER_SIZE];
		memcpy(rep, reply.data(), BUFFER_SIZE);

        std::cout << "Response " << ++Count << ": " << rep << "\t(len:" << strlen(rep) << ")" << std::endl;
    }
    return 0;
}
