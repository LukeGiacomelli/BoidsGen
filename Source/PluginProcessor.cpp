
#include "PluginProcessor.h"
#include "PluginEditor.h"

#define update_per_second 30

MidiBoidsAudioProcessor::MidiBoidsAudioProcessor()
    : parameters(*this, nullptr, "MB", Parameters::createParameterLayout())
{
    Parameters::addListenerToAllParameters(parameters, this);
    defaultParametersState = parameters.copyState();

    for (int i = 0; i < number_of_boids; ++i)
    {
        auto b = std::make_unique<Boids>(piano.getPianoBoundsInTheScreen(), default_speed, max_speed, default_view);
        boids.push_back(std::move(b));
    }
    
    //Separo il quadtree su un thread diverso da quello audio
    activeQt.store(&qt);
    qtThread = std::make_unique<QuadtreeThread>(boids, activeQt, qt, qt2, activeBoids);
    qtThread->startThread();

    notesInTonality = &piano.getNoteInTonality();
    areasCollection = &piano.getAreaCollection();
}

MidiBoidsAudioProcessor::~MidiBoidsAudioProcessor()
{
    qtThread->stopThread(1000);
}


void MidiBoidsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sr = sampleRate;
    blockSize = samplesPerBlock;
    blockPerSecond = sr / blockSize;
    updateInterval = blockPerSecond / update_per_second;

    neighbors.clear();
    neighbors.reserve(100);

    midiManager.prepareToPlay(sampleRate);
}

void MidiBoidsAudioProcessor::releaseResources()
{
    midiManager.releaseResources();
}


void MidiBoidsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    auto now = iteration_counter * (buffer.getNumSamples() / sr); //Tempo di esecuzione approssimato

    //Boids manager
    if (iteration_counter % updateInterval == 0) 
    {
        const Vector2f pianoPosition = { piano.getPianoBoundsInTheScreen().getX(), piano.getPianoBoundsInTheScreen().getY() };

        for (int i = 0; i < activeBoids; ++i)
        {
            auto& b = boids[i];

            neighbors.clear();

            auto range = b->getRange() / 2;
            Rectangle<float> view = Rectangle<float>{ b->getPosition().x() - range, b->getPosition().y() - range, range * 2, range * 2 };
            activeQt.load()->query(view, neighbors);

            b->borders();

            auto separation = b->separation(neighbors);
            auto alignment = b->alignment(neighbors);
            auto cohesion = b->cohesion(neighbors);

            auto avoidNonTonality = b->avoidNonTonality(pianoPosition, *notesInTonality, *areasCollection);
            auto followTonality = b->followTonality(pianoPosition, *notesInTonality, *areasCollection, selectedScale);

            alignment *= alignmentForce;    //Peso della regola
            separation *= separationForce;
            cohesion *= cohesionForce;

            avoidNonTonality *= tonalityAvoidingForce;
            followTonality *= tonalityFollowingForce;

            //Variazioni di rotta casuale se non in un gruppo (prob)
            if (alignment == Vector2f(0, 0)) b->alignementBias(boidsBias);

            auto allForces = separationForce + alignmentForce + cohesionForce;
            b->applyForce((separation + alignment + cohesion) / (allForces ? allForces : 1));

            auto tonForce = tonalityAvoidingForce + tonalityFollowingForce;
            b->applyForce((avoidNonTonality + followTonality) / (tonForce ? tonForce : 1));

            if (iteration_counter % updateInterval * 2 == 0) b->checkForNotes(pianoPosition, *areasCollection, now);

            b->update();
        }

        //Resetto le pedane
        auto reduction = 0.f;
        if (!piano.getAutoThreshold())
            reduction = std::max(0.25f, std::min(piano.getThreshold() / 2, piano.getThreshold() / 5)) * sustainReduction;
        else
            reduction = std::max(0.05f, std::min(activeBoids * 0.025f, 25.f)) * sustainReduction;

        for (auto& row : *areasCollection)
            for (auto& a : row)
                if (a.getUsed()) a.setPeso(a.getPeso() > 0 ? a.getPeso() - reduction : 0, now);

    }
    //Live mode
    if (selectedScale == "Live")
    {
        auto toUpdate = midiManager.getNotesPressed(midiMessages, *notesInTonality, numberOfNotesPressed);
        if (toUpdate)
            piano.updatePianoTonality();
    }

    midiManager.processBlock(buffer, midiMessages);

    ++iteration_counter;
}

juce::AudioProcessorEditor* MidiBoidsAudioProcessor::createEditor() { return new WrappedPluginEditor(*this,parameters); }

//==============================================================================
void MidiBoidsAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MidiBoidsAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}

void MidiBoidsAudioProcessor::resetParameters()
{
    parameters.replaceState(defaultParametersState.createCopy());
}

void MidiBoidsAudioProcessor::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == Parameters::nameSeparation) 
    {
        separationForce = newValue;
    }

    if (paramID == Parameters::nameAlignment)
    {
        alignmentForce = newValue;
    }
    if (paramID == Parameters::nameCohesion)
    {
        cohesionForce = newValue;
    }
    if (paramID == Parameters::nameAvoidTonality)
    {
        tonalityAvoidingForce = newValue;
    }
    if (paramID == Parameters::nameFollowTonality)
    {
        tonalityFollowingForce = newValue;
    }
    if (paramID == Parameters::nameSelectedScale)
    {
        selectedScale = Parameters::scaleNames[(int)newValue];
        piano.setPianoScale(selectedScale);
        piano.updatePianoTonality();
        notesInTonality = &piano.getNoteInTonality();
    }
    if (paramID == Parameters::nameSelectedTonic)
    {
        piano.setPianoTonic(newValue);
        piano.updatePianoTonality();
        notesInTonality = &piano.getNoteInTonality();
    }
    if(paramID == Parameters::nameSelectedPiano)
    {
        piano_layout = newValue ? "Tonnetz" : "Chrom";
        piano.setScale(piano_layout);
        piano.updatePianoTonality();
    }
    if (paramID == Parameters::nameBoidsRecklessness)
    {
        default_speed = newValue;
    }
    if (paramID == Parameters::nameBoidsMaxSpeed)
    {
        max_speed = newValue;
    }
    if (paramID == Parameters::nameSustain)
    {
        sustainReduction = 2.01f - newValue;
    }
    if(paramID == Parameters::nameHighOctave)
    {
        piano.setHighOct(newValue);
        piano.updatePianoTonality();
    }
    if (paramID == Parameters::nameLowOctave)
    {
        piano.setLowOct(newValue);
        piano.updatePianoTonality();
    }
    if (paramID == Parameters::nameBoidsView)
    {
        default_view = newValue;
    }
    if (paramID == Parameters::nameBoidsNumber)
    {
        activeBoids = newValue;
    }
    if (paramID == Parameters::nameThreshold)
    {        
        piano.setThreshold(newValue);
    }
    if (paramID == Parameters::nameAutoThreshold)
    {
        piano.useAutoThreshold(newValue);
    }
    if(paramID == Parameters::nameBoidsBias)
    {
        boidsBias = newValue;
    }
    if (paramID == Parameters::nameReset)
    {
        resetParameters();
    }
    
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiBoidsAudioProcessor();
}
