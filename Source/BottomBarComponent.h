
#pragma once
#include <JuceHeader.h>
#include "PluginParameters.h"
#include "Piano.h"
#include "AdvancedMenu.h"
#include "CustomLookAndFeel.h"

typedef AudioProcessorValueTreeState::SliderAttachment sliderAttachment;

#define sliderMouseSens 200
#define MAX_OCTAVE_RANGE 4

class BottomBar : public juce::Component
{
public:
    BottomBar(AudioProcessorValueTreeState& vts, Piano* p, AdvancedMenu* am, CustomLookAndFeel& lf)
        : parameters(vts), piano(p), advancedMenu(am), look(lf)
    {
        settings.setButtonText("Settings...");
        settings.setColour(TextButton::textColourOffId, Colours::black.withAlpha(0.6f));
        settings.setColour(TextButton::buttonColourId, Colours::transparentBlack);
        settings.setColour(TextButton::buttonOnColourId, Colours::transparentWhite);

        hiOctSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
        hiOctSlider.setSliderSnapsToMousePosition(false);
        hiOctSlider.setMouseDragSensitivity(sliderMouseSens);
        hiOctSlider.setAlwaysOnTop(true);
        hiOctSlider.setColour(Slider::trackColourId, Colours::transparentBlack);

        highOctave.setText("|  High octave:", juce::dontSendNotification);
        highOctave.attachToComponent(&hiOctSlider,true);

        lowOctSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
        lowOctSlider.setAlwaysOnTop(true);
        lowOctSlider.setMouseDragSensitivity(sliderMouseSens);
        lowOctSlider.setAlwaysOnTop(true);
        lowOctSlider.setColour(Slider::trackColourId, Colours::transparentBlack);

        lowOctave.setText("Low octave:", juce::dontSendNotification);
        lowOctave.attachToComponent(&lowOctSlider, true);

        //Problema con il range, in alcuni casi la modifica del valore (per cambio di range) non viene notificata
        hiOctSlider.onValueChange = [this]() {
                lowOctSlider.setRange(hiOctSlider.getValue() - MAX_OCTAVE_RANGE, hiOctSlider.getValue() - 1);
            };
        lowOctSlider.onValueChange = [this]() {
                hiOctSlider.setRange(lowOctSlider.getValue() + 1, lowOctSlider.getValue() + MAX_OCTAVE_RANGE);
            };
        settings.onClick = [this]()
            {
                advancedMenu->setVisible(!advancedMenu->isVisible());
                look.setHideTooltip(true);
            };

        addAndMakeVisible(highOctave, 0);
        addAndMakeVisible(lowOctave, 0);
        addAndMakeVisible(hiOctSlider, 0);
        addAndMakeVisible(lowOctSlider, 0);
        addAndMakeVisible(settings);
    }

    ~BottomBar() {};

    void paint(Graphics& g) override
    {
        g.fillAll(Colours::transparentBlack);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(2);
 
        settings.setBounds(area.getX()-5, area.getY(), 100, 30);

        hiOctSlider.setBounds(area.getWidth()-35, area.getY(), 25, 25);
        hiAttachment.reset(new sliderAttachment(parameters, Parameters::nameHighOctave, hiOctSlider));

        lowOctSlider.setBounds(area.getWidth() - 170, area.getY(), 25, 25);
        loAttachment.reset(new sliderAttachment(parameters, Parameters::nameLowOctave, lowOctSlider));

        lowOctSlider.setValue(piano->getLowOctave());
        hiOctSlider.setValue(piano->getHighOctave());
    }

    void hidePianoUI()
    {
        lowOctSlider.setVisible(!lowOctSlider.isVisible());
        hiOctSlider.setVisible(!hiOctSlider.isVisible());
    }
private:
    CustomLookAndFeel& look;
    AudioProcessorValueTreeState& parameters;
    Piano* piano;
    AdvancedMenu* advancedMenu;

    Label highOctave, lowOctave;
    Slider lowOctSlider, hiOctSlider;
    TextButton settings;

    std::unique_ptr<sliderAttachment> hiAttachment;
    std::unique_ptr<sliderAttachment> loAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BottomBar)
};
