#pragma once

#include <JuceHeader.h>
#include "PluginParameters.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class Knobs : public Component 
{
public:
	Knobs(AudioProcessorValueTreeState& vts)
        : parameters(vts)
    {
        leftLabels[0].setText("boids view", dontSendNotification);
        leftLabels[1].setText("boids recklessness", dontSendNotification);
        leftLabels[2].setText("notes persistence", dontSendNotification);
        leftLabels[3].setText("avoid non-tonality", dontSendNotification);

        rightLabels[0].setText("separation force", dontSendNotification);
        rightLabels[1].setText("alignment force", dontSendNotification);
        rightLabels[2].setText("cohesion force", dontSendNotification);
        rightLabels[3].setText("tonality force", dontSendNotification);

        leftSliders[0].setTooltip(juce::String("Range of view of these guys."));
        leftSliders[1].setTooltip(juce::String("Boids can overthink too...\t[A low value will result in boids moving slow and with caution]"));
        leftSliders[2].setTooltip(juce::String("Sustain of the generated notes, not in seconds, not in beat, let the boids cook... (can affect the perceived threshold)"));
        leftSliders[3].setTooltip(juce::String("Control the force that make boids avoid note out of the tonality"));

        rightSliders[0].setTooltip(juce::String("Boids need space too!"));
        rightSliders[1].setTooltip(juce::String("This align boids direction with the one of their neighbors."));
        rightSliders[2].setTooltip(juce::String("The force that keep boids togheter..."));
        rightSliders[3].setTooltip(juce::String("Boids like tonality, who don't ?! [WARNING: if they focus to much on the things they like, they will lose focus on things to avoid!]"));


        for (int i = 0; i < 4; ++i)
        {
            leftLabels[i].setJustificationType(Justification::centred);
            leftSliders[i].setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
            leftSliders[i].setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

            rightLabels[i].setJustificationType(Justification::centred);
            rightSliders[i].setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
            rightSliders[i].setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
            
            addAndMakeVisible(leftLabels[i]);
            addAndMakeVisible(rightLabels[i]);

            addAndMakeVisible(leftSliders[i]);
            addAndMakeVisible(rightSliders[i]);
        }

    };
	~Knobs() 
    {
        for (int i = 0; i < slidersAttachment.size(); i++)
            slidersAttachment[i].reset();
    };

    void paint(Graphics& g) override
    {
        g.fillAll(Colours::transparentBlack);
    }

    void resized() override
    {
        int spacing = 72;
        int sliderSize = 110;

        // Colonna destra
        int rightX = getWidth() - sliderSize - 40; // bordo destro - larghezza - padding
        for (int i = 0; i < rightSliders.size(); ++i)
        {
            int top = i * (sliderSize + spacing);
            rightSliders[i].setBounds(rightX, top, sliderSize, sliderSize);
            rightLabels[i].setBounds(rightSliders[i].getBounds().withTop(rightSliders[i].getBounds().getBottom()).withHeight(20));
        }

        // Colonna sinistra
        int leftX = 40;
        for (int i = 0; i < leftSliders.size(); ++i)
        {
            int top = i * (sliderSize + spacing);
            leftSliders[i].setBounds(leftX, top, sliderSize, sliderSize);
            leftLabels[i].setBounds(leftSliders[i].getBounds().withTop(leftSliders[i].getBounds().getBottom()).withHeight(20));
        }

        slidersAttachment[0].reset(new SliderAttachment(parameters, Parameters::nameBoidsView, leftSliders[0])); //range of view
        slidersAttachment[1].reset(new SliderAttachment(parameters, Parameters::nameBoidsRecklessness, leftSliders[1])); //Boids recklessness
        slidersAttachment[2].reset(new SliderAttachment(parameters, Parameters::nameSustain, leftSliders[2])); //Note sustain
        slidersAttachment[3].reset(new SliderAttachment(parameters, Parameters::nameAvoidTonality,leftSliders[3])); //Avoid non tonality
        slidersAttachment[4].reset(new SliderAttachment(parameters, Parameters::nameSeparation, rightSliders[0])); //Separation
        slidersAttachment[5].reset(new SliderAttachment(parameters, Parameters::nameAlignment, rightSliders[1])); //Alignment
        slidersAttachment[6].reset(new SliderAttachment(parameters, Parameters::nameCohesion, rightSliders[2])); //Cohesion
        slidersAttachment[7].reset(new SliderAttachment(parameters, Parameters::nameFollowTonality, rightSliders[3])); //Tonality
    }
private: 

    std::array<Slider,4> leftSliders;
    std::array<Slider,4> rightSliders;

    std::array<Label, 4> leftLabels;
    std::array<Label, 4> rightLabels;

    std::array<std::unique_ptr<SliderAttachment>, 8> slidersAttachment;

    AudioProcessorValueTreeState& parameters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knobs)
};