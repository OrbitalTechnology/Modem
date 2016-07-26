#include "filter.hpp"

#include <iostream>
#include <math.h>

namespace OrbitalTechnology {
	namespace DSP {
		Filter::Filter(const uint16_t poleCount, const float low, const float high) {
			this->m_PoleCount = poleCount;
			this->m_LowFrequency = low;
			this->m_HighFrequency = high;
		}

		Filter::~Filter() {

		}

		void Filter::GenerateCoefficients() {
			float g = 0.5;
			float fsr = 44100;			

			float bw = this->m_HighFrequency - this->m_LowFrequency;
			float fc = this->m_LowFrequency + (bw / 2);
			float tapCount = this->m_PoleCount;

			std::cout << "Filter:" << std::endl 
				<< "\tTaps: " << tapCount << std::endl
				<< "\tLF: " << this->m_LowFrequency << std::endl
				<< "\tHF: " << this->m_HighFrequency << std::endl
				<< "\tBW: " << bw << std::endl
				<< "\tCenter: " << fc << std::endl << std::endl;

			for (unsigned int i = 0; i < tapCount - 1; ++i) {
				float a;
				float ys;
				float yg;
				float yw;
				float yf;

				a = (i - tapCount / 2) * 2.0 * M_PI * (bw / fsr);

				if (a != 0.0f) {
					ys = sin(a) / a;
				} else {
					ys = 1.0f;
				}
				
				yg = g * (4.0 * bw / fsr);
				yw = 0.54 - 0.46 * cos(i * 2.0 * M_PI / tapCount);
				yf = cos((i - (tapCount / 2)) * 2.0 * M_PI * fc / fsr);
				
				float coeff = yf * yw * yg * ys;
				
				this->m_Coefficients.push_back(coeff);
			}
		}

		const float Filter::Process(const float input) {
			
			this->m_Samples.push_back(input);

			if(this->m_Samples.size() < this->m_PoleCount) {
				// Not enough sample data yet
				return 0.0f;
			}

			float accumulator = 0.0f;
			float tapCount = this->m_PoleCount;

			for(uint16_t n = tapCount; n > 0; --n) {
				accumulator += ((this->m_Coefficients[tapCount - 1 + n]) * (this->m_Samples[n]));
			}

			this->m_Samples.pop_front();

			return accumulator;
		}
	}
}