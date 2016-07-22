#include "modem.hpp"

void messageQueue(const Orbital::Queuing::Message msg) {

}

int main(int argc, char* argv[]) {	
	Orbital::IO::Socket 	socket;
	Orbital::Queuing::Queue queue("main_queue");

	queue.Subscribe(messageQueue);

	socket.SetAddress("0.0.0.0");
	socket.SetPort(4895);
	socket.Listen();

	return 0;
}