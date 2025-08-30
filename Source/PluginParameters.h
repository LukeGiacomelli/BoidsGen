#pragma once
#include <JuceHeader.h>

namespace Parameters
{
	static const String nameSeparation = "S";
	static const String nameAlignment= "A";
	static const String nameCohesion = "C";
	static const String nameAvoidTonality = "AT";
	static const String nameFollowTonality = "FT";
	static const String nameSelectedScale = "SS";
	static const String nameSelectedTonic = "ST";
	static const String nameSelectedPiano = "P";
	static const String nameBoidsRecklessness = "BR";
	static const String nameBoidsMaxSpeed = "MS";
	static const String nameSustain = "SUS";
	static const String nameHighOctave = "HO";
	static const String nameLowOctave = "LO";
	static const String nameBoidsView = "BV";
	static const String nameBoidsNumber = "BN";
	static const String nameThreshold = "TH";
	static const String nameAutoThreshold = "ATHR";
	static const String nameBoidsBias = "BB";

	static const float defaultSeparation = 2.5f;
	static const float defaultAlignment = 1.f;
	static const float defaultCohesion = 1.2f;
	static const float defaultAvoidTonality = 2.6f;
	static const float defaultFollowTonality = 0.5f;
	static const int defaultSelectedScale = 0; //Maj
	static const int defaultSelectedTonic = 0; //C
	static const bool defaultSelectedPiano = false; //Chrom
	static const float defaultBoidsRecklessness = 1.5f;
	static const float defaultBoidsMaxSpeed= 5.f;
	static const float defaultSustain = 0.3f;
	static const float defaultHighOctave = 4.f;
	static const float defaultLowOctave = 2.f;
	static const float defaultBoidsView = 60.f;
	static const float defaultBoidsNumber = 1000.f;
	static const float defaultThreshold = -1.f;
	static const bool defaultAutoThreshold = false;
	static const float defaultBoidsBias = 0.4f;

	const StringArray scaleNames = {
		"Major",
		"Major 7th",
		"Major Dominant",
		"Major 6th",
		"Major Augmented",
		"Minor",
		"Minor 7th",
		"Minor Major 7th",
		"Minor 6th",
		"Blues",
		"Dorian",
		"Mixolydian",
		"Phrygian",
		"Lydian",
		"Locrian",
		"Harmonic Minor",
		"Melodic Minor",
		"Pentatonic Neutral",
		"Pentatonic Minor",
		"Pentatonic Major",
		"Dim Half",
		"Dim Whole",
		"Augmented",
		"Roumanian Minor",
		"Spanish - Gypsy",
		"Double Harmonic",
		"Eight-Tone Spanish",
		"Enigmatic",
		"Algerian",
		"Balinese",
		"Hirajoshi",
		"Hungarian Gypsy",
		"Japanese",
		"Persian",
		"Prometheus",
		"Six-Tone Symetrical",
		"Super Locrian",
		"Wholetone",
		"Half Diminished",
		"Live"
	};
	const StringArray notes = { "C", "C#", "D", "D#","E", "F", "F#", "G", "G#", "A", "A#", "B" };

	static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
	{
		std::vector<std::unique_ptr<RangedAudioParameter>> params;

		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameSeparation), "separation force", NormalisableRange(0.f, 5.f, 0.01f), defaultSeparation));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameAlignment), "alignment force", NormalisableRange(0.f, 5.f, 0.01f), defaultAlignment));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameCohesion), "cohesion force", NormalisableRange(0.f, 5.f, 0.01f), defaultCohesion));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameAvoidTonality), "avoid non-tonality", NormalisableRange(0.f, 5.f, 0.01f), defaultAvoidTonality));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameFollowTonality), "follow tonality", NormalisableRange(0.f, 5.f, 0.01f), defaultFollowTonality));
		params.push_back(std::make_unique<AudioParameterChoice>(ParameterID(nameSelectedScale), "Selected scale", scaleNames, defaultSelectedScale));
		params.push_back(std::make_unique<AudioParameterChoice>(ParameterID(nameSelectedTonic), "Selected tonic", notes, defaultSelectedTonic));
		params.push_back(std::make_unique<AudioParameterBool>(ParameterID(nameSelectedPiano), "Selected piano", defaultSelectedPiano));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameBoidsRecklessness), "boids recklessness", NormalisableRange(0.5f, 4.5f, 0.01f), defaultBoidsRecklessness));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameBoidsMaxSpeed), "boids max-speed", NormalisableRange(1.4f, 7.5f, 0.1f), defaultBoidsMaxSpeed));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameSustain), "sustain", NormalisableRange(0.001f, 2.f, 0.001f), defaultSustain));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameHighOctave), "high octave", NormalisableRange(-1.f, 8.f, 1.f), defaultHighOctave));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameLowOctave), "low octave", NormalisableRange(-2.f, 7.f, 1.f), defaultLowOctave));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameBoidsView), "boids view", NormalisableRange(40.f, 150.f, 1.f,0.8f, false), defaultBoidsView));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameBoidsNumber), "boids number", NormalisableRange(1.f, 2500.f, 5.f), defaultBoidsNumber));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameThreshold), "threshold", NormalisableRange(1.f, 1000.f, 5.f), defaultThreshold));
		params.push_back(std::make_unique<AudioParameterFloat>(ParameterID(nameBoidsBias), "boids bias", NormalisableRange(0.f, 1.f, 0.1f), defaultBoidsBias));
		params.push_back(std::make_unique<AudioParameterBool>(ParameterID(nameAutoThreshold), "auto threshold", defaultAutoThreshold));


		return { params.begin(), params.end() };
	}

	static void addListenerToAllParameters(AudioProcessorValueTreeState& valueTreeState, AudioProcessorValueTreeState::Listener* listener)
	{
		std::unique_ptr<XmlElement> xml(valueTreeState.copyState().createXml());

		for (auto* element : xml->getChildWithTagNameIterator("PARAM"))
		{
			const String& id = element->getStringAttribute("id");
			valueTreeState.addParameterListener(id, listener);
		}
	}


}