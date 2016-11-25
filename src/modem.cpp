#include "modem.hpp"

OrbitalTechnology::DSP::Filter bandPassFilterI(128, 100.0f, 1000.0f);
OrbitalTechnology::DSP::Filter bandPassFilterQ(128, 100.0f, 1000.0f);

double localOscillatorPhase = 0.0;
float localOscillatorFrequency = -(1500.0 + (1000.0f / 2));
float localOscillatorDelta =  2 * M_PI * localOscillatorFrequency / 44100;
float localOscillatorAmplitude = 1.0f;

std::deque<ComplexSignal> signalData;
std::ofstream outputFile;

bool writing = false;

int dataCallback(
	const void *input, 
	void *output, 
	unsigned long frameCount, 
	const PaStreamCallbackTimeInfo* timeInfo, 
	PaStreamCallbackFlags statusFlags, 
	void *userData ) {
	
	float* floatData = (float*) input;

	for(unsigned int s = 0; s < frameCount; s++) {
		if (floatData[s] == 0.0) {
			if (writing) {
				writing = false;
				std::cout << "Finished Writing" << std::endl;
				running = false;
			}
			continue;
		}

		writing = true;

		ComplexSignal signalSample(floatData[s], 0.0f);		
		ComplexSignal localOscillatorSample;
		localOscillatorSample.Sin(localOscillatorPhase);

		ComplexSignal intermediate;

		intermediate.I = bandPassFilterI.Process((signalSample.I * localOscillatorSample.I) - (signalSample.Q * localOscillatorSample.Q));
	    intermediate.Q = bandPassFilterQ.Process((signalSample.I * localOscillatorSample.Q) + (signalSample.Q * localOscillatorSample.I));

		outputFile.write((char *)&intermediate.I, sizeof(float));
		outputFile.write((char *)&intermediate.Q, sizeof(float));

		localOscillatorPhase += localOscillatorDelta;
	}

	return paContinue;
}

void msgApplication(const Orbital::Queuing::Message msg) {
	std::cout << "[application-messages] " << msg.Content() << std::endl;
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

	bandPassFilterI.GenerateCoefficients();
	bandPassFilterQ.GenerateCoefficients();

	PaStream* stream;
    PaError err;

    int selectedDevice = 0;

    err = Pa_Initialize();
    if(err != paNoError) {
    	std::cerr << Pa_GetErrorText(err) << std::endl;
    	return -1;
    }

    PaDeviceIndex deviceCount = Pa_GetDeviceCount();
    if (deviceCount < 0) {
    	std::cerr << Pa_GetErrorText(deviceCount) << std::endl;
    	return -1;
    }
    std::cout << "Devices: " << deviceCount << std::endl;

    for(unsigned d = 0; d < deviceCount; d++) {
    	const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(d);
    	std::cout << "Device " << d << ": " << deviceInfo->name << ": " << deviceInfo->maxInputChannels << "/" << deviceInfo->maxOutputChannels << std::endl;
    	if (strcmp(deviceInfo->name, "pulse") == 0) {
    		selectedDevice = d;
    	}
    }

    std::cout << "Selected Device: " << selectedDevice << std::endl;

    PaStreamParameters inputParams;
    inputParams.device = Pa_GetDefaultInputDevice();
    inputParams.channelCount = 1;
    inputParams.sampleFormat = paFloat32,
    inputParams.suggestedLatency = Pa_GetDeviceInfo(inputParams.device)->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = NULL;

    std::cout << "Opening Device" << std::endl;

    err = Pa_OpenStream(&stream, &inputParams, NULL, 44100, 256, paNoFlag, dataCallback, NULL);
    if (err != paNoError) {
    	std::cerr << Pa_GetErrorText(err) << std::endl;
    	return -1;
    }

    std::cout << "Opened device" << std::endl;

    // Open File
    outputFile.open("/home/andy/lo.wav", std::ios::binary | std::ios::trunc);

    err = Pa_StartStream(stream);
    if (err != paNoError) {
    	std::cerr << Pa_GetErrorText(err) << std::endl;
    	return -1;
    }

	while(running && queue.Process()) {

	}

	return 0;
}