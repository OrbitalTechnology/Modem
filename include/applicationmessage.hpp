#pragma once

#include "message.hpp"

class ApplicationMessage : public Orbital::Queuing::Message {
	public:
		ApplicationMessage(const int);
		ApplicationMessage(const ApplicationMessage&);
		~ApplicationMessage();

		const int Signal() const;

	protected:

	private:
		int 	m_Signal;
};