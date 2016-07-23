#include <iostream>

#include <signal.h>

#include "socket.hpp"
#include "queue.hpp"

#include "applicationmessage.hpp"

Orbital::Queuing::Queue queue("application-messages");

bool running = true;