/*
  ==============================================================================

    TopBarComponent.h
    Created: 23 Apr 2025 9:26:20am
    Author:  lgiac

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginParameters.h"

typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

class TopBarComponent : public juce::Component
{
public:
    TopBarComponent(AudioProcessorValueTreeState& vts)
        : parameters(vts)
    {
        addAndMakeVisible(logo);
        logo.setImage(ImageCache::getFromMemory(BinaryData::boidsGEN_png, BinaryData::boidsGEN_pngSize));


        auto nextImg = ImageCache::getFromMemory(BinaryData::right_arrow_png, BinaryData::right_arrow_pngSize);
        addAndMakeVisible(nextButton);
        nextButton.setImages(
            false, 
            true, 
            true, 
            nextImg,
            1.f,
            Colour(249, 249, 249),
            nextImg, 0.5f, Colour(249, 249, 249).withAlpha(0.5f),
            nextImg, 0.8f, Colour(249, 249, 249).withAlpha(0.8f)
        );

        auto prevImg = ImageCache::getFromMemory(BinaryData::left_arrow_png, BinaryData::left_arrow_pngSize);
        addAndMakeVisible(previousButton);
        previousButton.setImages(
            false,
            true,
            true,
            prevImg,
            1.f,
            Colour(249, 249, 249),
            prevImg, 0.5f, Colour(249, 249, 249).withAlpha(0.5f),
            prevImg, 0.8f, Colour(249, 249, 249).withAlpha(0.8f)
        );

        auto i = 0;
        for (auto& s : Parameters::scaleNames) scalesBox.addItem(s, ++i);
        addAndMakeVisible(scalesBox);
        scalesBox.setText("Major", juce::dontSendNotification);
        scalesBox.setJustificationType(juce::Justification::centred);
        scalesBox.setTooltip(String("With the 'Live' scale/mode boids will follow your MIDI input"));

        i = 0;
        for (auto& s : Parameters::notes) notesBox.addItem(s, ++i);
        addAndMakeVisible(notesBox);
        notesBox.setText("C", juce::dontSendNotification);
        notesBox.setJustificationType(juce::Justification::centred);

        addAndMakeVisible(pianoButton);
        pianoButton.setButtonText("Tonnetz");
        pianoButton.setClickingTogglesState(true);
    }

    ~TopBarComponent() {};

    void paint(Graphics& g) override 
    {
        g.fillAll(Colour(0xff32373b).darker(0.1f).withAlpha(0.9f));
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(10);
        const int btn_height = 40;

        logo.setBounds(area.getX(), -60, 285, 190);

        auto center_of_area = 260 + area.getWidth() / 2;
        previousButton.setBounds(center_of_area - 305, area.getHeight() / 2 - (btn_height / 4), 60, btn_height);
        notesBox.setBounds(center_of_area - 250, area.getHeight() / 2 - (btn_height / 4), 60, btn_height);
        scalesBox.setBounds(center_of_area - 180, area.getHeight() / 2 - (btn_height / 4), 430, btn_height);
        nextButton.setBounds(center_of_area + 245, area.getHeight() / 2 - (btn_height / 4), 60, btn_height);
        
        pianoButton.setBounds(area.getWidth() - 70, area.getHeight() / 2 - (btn_height / 4), 70, btn_height);

        notesAttachment.reset(new ComboAttachment(parameters, Parameters::nameSelectedTonic, notesBox));
        scaleAttachment.reset(new ComboAttachment(parameters, Parameters::nameSelectedScale, scalesBox));
        pianoButtonAttachment.reset(new ButtonAttachment(parameters, Parameters::nameSelectedPiano, pianoButton));

        previousButton.onClick = [this]()
            {
                auto newValue = notesBox.getSelectedItemIndex() == 0 ? notesBox.getNumItems() - 1 : notesBox.getSelectedItemIndex() - 1;
                notesBox.setSelectedItemIndex(newValue);
            };
        nextButton.onClick = [this]() 
            {
                notesBox.setSelectedItemIndex(notesBox.getSelectedItemIndex() + 1);
            };
        
        scalesBox.onChange = [this]()
            {
                if (Parameters::scaleNames[scalesBox.getSelectedItemIndex()] == "Live")
                    notesBox.setEnabled(false);
                else
                    notesBox.setEnabled(true);
            };
    }

    ToggleButton& getPianoButton()
    {
        return pianoButton;
    }

private:
    AudioProcessorValueTreeState& parameters;

    ImageButton nextButton, previousButton;
    ToggleButton pianoButton;

    std::unique_ptr<ComboAttachment> scaleAttachment, notesAttachment;
    std::unique_ptr<ButtonAttachment> pianoButtonAttachment;

    ComboBox scalesBox, notesBox;
    ImageComponent logo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopBarComponent)
};
