#include "modem.hpp"

void msgApplication(const Orbital::Queuing::Message msg) {
	std::cout << "[application-messages] " << msg.Content() << std::endl;

	ApplicationMessage* applicatioMessage = (ApplicationMessage*) &msg;

	std::cout << applicatioMessage->Signal() << std::endl;
}

void signalHandler (int signal) {
	switch(signal) {
		case SIGINT:
			queue.Publish(ApplicationMessage(SIGINT));
			running = false;
			break;

		case SIGTERM:
			queue.Publish(ApplicationMessage(SIGTERM));
			running = false;
			break;

		default:
			std::cout << "Unknown Signal: " << signal << std::endl;
			running = false;
			break;
	}
}

int main(int argc, char* argv[]) {		
	if(signal(SIGINT, signalHandler) == SIG_ERR) {
		std::cout << "Unable to register SIGINT handler" << std::endl;
		return -1;
	}
	if(signal(SIGTERM, signalHandler) == SIG_ERR) {
		std::cout << "Unable to register SIGKILL handler" << std::endl;
		return -1;
	}

	queue.Subscribe(msgApplication);	

	while(running && queue.Process()) {

	}

	return 0;
}