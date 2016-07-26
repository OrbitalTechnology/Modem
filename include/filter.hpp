#pragma once

#include <vector>
#include <deque>
#include <stdint.h>

namespace OrbitalTechnology {
	namespace DSP {
		class Filter {
			public:
				Filter();
				Filter(const uint16_t poleCount, const float low, const float high);
				~Filter();

				void GenerateCoefficients();
				const float Process(const float input);

			protected:

			private:
				float 				m_LowFrequency;
				float 				m_HighFrequency;
				uint16_t			m_PoleCount;
				std::deque<float> 	m_Samples;
				std::vector<float>	m_Coefficients;
		};
	}
}