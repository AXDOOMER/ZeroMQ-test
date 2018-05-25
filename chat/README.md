# Basic chat

## How to use

Start a server and open the client. The server IP must be hard-coded into the client.

A client can send a dot (`.`) to receive the last 20 messages.

## How to run

Compile and run the server:

`g++ server.cpp -Wall -lzmq -o server && ./server`

Compile and run the client:

`g++ client.cpp -Wall -lzmq -o client && ./client`

Install the ZeroMQ library:

`sudo apt-get install libzmq3-dev`
