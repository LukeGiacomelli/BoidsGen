/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Piano.h"
#include "PianoDrawer.h"
#include "BoidsDrawer.h"
#include "TopBarComponent.h"
#include "BottomBarComponent.h"
#include "Knobs.h"
#include "CustomLookAndFeel.h"
#include "AdvancedLookAndFeel.h"
#include "AdvancedMenu.h"

//==============================================================================
/**
*/
class MidiBoidsAudioProcessorEditor  : public Component
{
public:
    MidiBoidsAudioProcessorEditor (MidiBoidsAudioProcessor&, AudioProcessorValueTreeState&);
    ~MidiBoidsAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MidiBoidsAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& parameters;

    CustomLookAndFeel newLook;
    AdvancedLookAndFeel advancedMenuLook;

    TooltipWindow ttw;

    Piano* piano = audioProcessor.getPiano();

    TopBarComponent menu{ parameters };
    AdvancedMenu advancedMenu{ parameters , newLook};
    BottomBar bottomMenu{ parameters, piano , &advancedMenu, newLook};
    PianoDrawer pd{ piano, menu.getPianoButton() };
    BoidsDrawer boid_scene{ audioProcessor.getBoids(), audioProcessor.getActiveBoids()};
    Knobs knobsLayout{ parameters};

    void hideUI();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiBoidsAudioProcessorEditor)
};

//Wrapper
class WrappedPluginEditor : public AudioProcessorEditor
{
public:
    WrappedPluginEditor(MidiBoidsAudioProcessor&, AudioProcessorValueTreeState&);
    void resized() override;
private:
    MidiBoidsAudioProcessor& audioProcessor;

    Piano* piano = audioProcessor.getPiano();
    int originalWidth = piano->getScreen().getX();
    int originalHeight = piano->getScreen().getY();

    MidiBoidsAudioProcessorEditor editor;
};