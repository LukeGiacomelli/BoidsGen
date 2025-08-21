#pragma once

#include <JuceHeader.h>

class MidiManager
{
public:
	MidiManager() {};
	~MidiManager() {};

	void prepareToPlay(double sr, int maxNumSamples) { fs = sr; }
	void releaseResources() ;
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages);
	bool getNotesPressed(MidiBuffer& midiMessages, std::array<String, 7>& pressedNotes, int& noteCount);

	void addNote(double midiNumber, bool note_on, float vel);

private:

	double fs = 44100.0;
	std::vector<MidiMessage> newMidiMessage;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiManager)
};