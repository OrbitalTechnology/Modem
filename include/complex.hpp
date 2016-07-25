#pragma once

#include <math.h>

struct ComplexSignal {
	ComplexSignal() {
		I = 0.0f;
		Q = 0.0f;
	};

	ComplexSignal(float value) {
		I = cos(value);
		Q = sin(value);
	};

	ComplexSignal(float i, float q) {
	    I = i;
	    Q = q;
	};

	void Sin(double phase) {
	    I = cos(phase);
	    Q = sin(phase);
	}

	const double Amplitude() {
		return sqrt(I*I + Q*Q);
	};

	const float Phase() {
		return atan2(I, Q);
	};

	float I;
	float Q;
};