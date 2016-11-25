#include <iostream>

#include <portaudio.h>
#include <signal.h>
#include <cstring>
#include <deque>
#include <math.h>
#include <fstream>

#include "complex.hpp"
#include "queue.hpp"

#include "applicationmessage.hpp"
#include "filter.hpp"

Orbital::Queuing::Queue queue("application-messages");

bool running = true;

