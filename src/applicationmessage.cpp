#include "applicationmessage.hpp"

ApplicationMessage::ApplicationMessage(const int signal) {
	this->m_Signal = signal;
}

ApplicationMessage::ApplicationMessage(const ApplicationMessage& msg) {
	this->m_Signal = msg.m_Signal;
}

ApplicationMessage::~ApplicationMessage() {
	
}

const int ApplicationMessage::Signal() const {
	return this->m_Signal;
}