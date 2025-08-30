#pragma once

#include <JuceHeader.h>
#include "PluginParameters.h"
#include "Eigen/Dense"
#include "Boids.h"
#include "Piano.h"
#include "Quadtree.h"
#include "MidiManager.h"
#include "QuadtreeThread.h"

using namespace Eigen;

#define PI 3.141592635
#define number_of_boids 2500
#define start_octave 2
#define last_octave 3
#define plain_layout "Chrom" //Tonnetz or Chrom

class MidiBoidsAudioProcessor  : public juce::AudioProcessor, AudioProcessorValueTreeState::Listener
{
public:
    MidiBoidsAudioProcessor();
    ~MidiBoidsAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    std::vector<std::unique_ptr<Boids>>& getBoids() { return boids; }
    int& getActiveBoids() { return activeBoids; }
    Piano* getPiano() { return &piano; }

    //==============================================================================
    bool MidiBoidsAudioProcessor::hasEditor() const { return true; }
    juce::AudioProcessorEditor* MidiBoidsAudioProcessor::createEditor();

    const juce::String MidiBoidsAudioProcessor::getName() const { return JucePlugin_Name; }
    bool MidiBoidsAudioProcessor::acceptsMidi() const { return true; }
    bool MidiBoidsAudioProcessor::producesMidi() const { return true; }
    bool MidiBoidsAudioProcessor::isMidiEffect() const { return false; }
    double MidiBoidsAudioProcessor::getTailLengthSeconds() const { return 0.0; }
    int MidiBoidsAudioProcessor::getNumPrograms() { return 1; }
    int MidiBoidsAudioProcessor::getCurrentProgram() { return 0; }
    void MidiBoidsAudioProcessor::setCurrentProgram(int index) {}
    const juce::String MidiBoidsAudioProcessor::getProgramName(int index) { return {}; }
    void MidiBoidsAudioProcessor::changeProgramName(int index, const juce::String& newName) {}

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    int iteration_counter = 0;
    double sr = 44100;
    int blockSize = 512;
    int blockPerSecond = 0;
    int updateInterval = 3;

    String piano_layout = plain_layout;
    std::array<String, 8>* notesInTonality;
    int numberOfNotesPressed = 0;
    std::vector<std::vector<Area>>* areasCollection;
    int activeBoids = Parameters::defaultBoidsNumber;

    float default_speed = Parameters::defaultBoidsRecklessness;
    float default_view = Parameters::defaultBoidsView;
    float max_speed = Parameters::defaultBoidsMaxSpeed;
    std::vector<std::unique_ptr<Boids>> boids;
    std::vector<Boids*> neighbors;
    Piano piano{ start_octave,last_octave,piano_layout,midiManager, activeBoids};

    //Quadtree
    Quadtree qt{ Rectangle<float>{0.f, 0.f, static_cast<float>(piano.getScreen().getX()), static_cast<float>(piano.getScreen().getY())}, 0}; //Tutto schermo
    Quadtree qt2{ Rectangle<float>{0.f, 0.f, static_cast<float>(piano.getScreen().getX()), static_cast<float>(piano.getScreen().getY())}, 0}; 
    std::atomic<Quadtree*> activeQt;
    std::unique_ptr<QuadtreeThread> qtThread;

    MidiManager midiManager;

    void parameterChanged(const String& paramID, float newValue) override;
    AudioProcessorValueTreeState parameters;

    float separationForce = Parameters::defaultSeparation;
    float alignmentForce = Parameters::defaultAlignment;
    float cohesionForce = Parameters::defaultCohesion;
    float tonalityAvoidingForce = Parameters::defaultAvoidTonality;
    float tonalityFollowingForce = Parameters::defaultFollowTonality;
    float boidsBias = Parameters::defaultBoidsBias;

    String selectedScale = "Cmaj";
    float sustainReduction = Parameters::defaultSustain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiBoidsAudioProcessor)
};
