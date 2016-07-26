#include "filter.hpp"

#include <iostream>
#include <math.h>

namespace OrbitalTechnology {
	namespace DSP {
		Filter::Filter() {

		}

		Filter::Filter(const uint16_t poleCount, const float low, const float high) {
			this->m_PoleCount = poleCount;
			this->m_LowFrequency = low;
			this->m_HighFrequency = high;
		}

		Filter::~Filter() {

		}

		void Filter::GenerateCoefficients() {
			// fsr = 44100 - sample rate
			// fc = 0 - lower frequency
			// bw = 3000 - bandwidth (at least fsr/n AND < fsr/2)
			// nt = 128 - number taps
			// g = 1 - gain

			float a;
			float g = 0.5;
			float ys;
			float yg;
			float yw;
			float yf;

			float bw = this->m_HighFrequency - this->m_LowFrequency;
			float fsr = 44100;
			float fc = this->m_LowFrequency + (bw / 2);

			for(unsigned int i = 0; i < this->m_PoleCount-1; ++i) {
				a = (i - this->m_PoleCount/2) * 2.0 * M_PI * (bw/fsr);
				if (a != 0.0f) {
					ys = sin(a) / a;
				} else {
					ys = 1.0f;
				}
				yg = g * (4.0 * bw/fsr);
				yw = 0.54 - 0.46 * cos(i * 2.0 * M_PI/this->m_PoleCount);
				yf = cos((i - (this->m_PoleCount / 2)) * 2.0 * M_PI * fc/fsr);
				float coeff = yf * yw * yg * ys;
				this->m_Coefficients.push_back(coeff);
			}
		}

		const float Filter::Process(const float input) {
			
			this->m_Samples.push_back(input);

			if(this->m_Samples.size() < this->m_PoleCount) {
				// Not enough samples yet
				return 0.0f;
			}

			float accumulator = 0.0f;
			float poles = this->m_PoleCount;
			for(uint16_t n = poles; n > 0; --n) {
				accumulator += ((this->m_Coefficients[poles - 1 + n]) * (this->m_Samples[n]));
			}

			this->m_Samples.pop_front();

			return accumulator;
		}
	}
}