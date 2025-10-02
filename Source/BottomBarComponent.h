
#pragma once
#include <JuceHeader.h>
#include "PluginParameters.h"
#include "Piano.h"
#include "AdvancedMenu.h"
#include "CustomLookAndFeel.h"


#define sliderMouseSens 60
#define MAX_OCTAVE_RANGE 5

class BottomBar : public juce::Component
{
public:
    BottomBar(AudioProcessorValueTreeState& vts, Piano* p, AdvancedMenu* am, CustomLookAndFeel& lf)
        : parameters(vts), piano(p), advancedMenu(am), look(lf)
    {
        //Settings
        settings.setButtonText("Settings...");
        settings.setColour(TextButton::textColourOffId, Colours::black.withAlpha(0.6f));
        settings.setColour(TextButton::buttonColourId, Colours::transparentBlack);
        settings.setColour(TextButton::buttonOnColourId, Colours::transparentWhite);

        //Octave
        octaveSlider.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
        octaveSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        octaveSlider.setRange(0, 8, 1.f);
        octaveSlider.setSliderSnapsToMousePosition(false);
        octaveSlider.setMouseDragSensitivity(sliderMouseSens);
        octaveSlider.setAlwaysOnTop(true);
        octaveSlider.setColour(Slider::trackColourId, Colours::transparentBlack);
        octaveLabel.setText("[octave range]", juce::dontSendNotification);
        octaveLabel.attachToComponent(&octaveSlider, true);
        octaveLabel.setAlpha(0.0f);
 

        settings.onClick = [this]()
            {
                advancedMenu->setVisible(!advancedMenu->isVisible());
                look.setHideTooltip(true);
            };

        auto* lowParameter = parameters.getParameter(Parameters::nameLowOctave);
        auto* hiParameter = parameters.getParameter(Parameters::nameHighOctave);

        octaveSlider.onDragStart = [this]() 
            {
                octaveLabel.setAlpha(0.8f);
            };

        octaveSlider.onDragEnd = [this]()
            {
                octaveLabel.setAlpha(0.f);
            };

        octaveSlider.onValueChange = [this, lowParameter, hiParameter]() 
            {
                auto lo = (float)octaveSlider.getMinValue();
                auto hi = (float)octaveSlider.getMaxValue();

                if (octaveSlider.getMaxValue() - octaveSlider.getMinValue() >= 1 && octaveSlider.getMaxValue() - octaveSlider.getMinValue() < MAX_OCTAVE_RANGE)
                {
                    loAttachment->setValueAsCompleteGesture(lo);
                    hiAttachment->setValueAsCompleteGesture(hi);
                }
            };

        loAttachment = std::make_unique <ParameterAttachment>(*lowParameter, [this, lowParameter](float newValue)
            {
                const auto v = lowParameter->convertFrom0to1(newValue);

                if (octaveSlider.getMaxValue() - octaveSlider.getMinValue() >= 1 && octaveSlider.getMaxValue() - octaveSlider.getMinValue() < MAX_OCTAVE_RANGE)
                    octaveSlider.setMinValue(v, dontSendNotification);
            }, 
            nullptr
        );
        hiAttachment = std::make_unique <ParameterAttachment>(*hiParameter, [this, hiParameter](float newValue)
            {
                const auto v = hiParameter->convertFrom0to1(newValue);

                if (octaveSlider.getMaxValue() - octaveSlider.getMinValue() >= 1 && octaveSlider.getMaxValue() - octaveSlider.getMinValue() < MAX_OCTAVE_RANGE)
                    octaveSlider.setMaxValue(v, dontSendNotification);
            }, 
            nullptr
        );

        addAndMakeVisible(octaveSlider);
        addAndMakeVisible(settings);
    }

    ~BottomBar() {};

    void paint(Graphics& g) override
    {
        g.fillAll(Colours::transparentBlack);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(2).translated(0,20);
 
        settings.setBounds(area.getX()-5, area.getY()-10, 120, 40);

        octaveSlider.setBounds(area.getWidth() - 140, area.getY()+7, 120, 12);

        octaveSlider.setMinAndMaxValues(piano->getLowOctave(), piano->getHighOctave(), dontSendNotification);
    }

    void hidePianoUI()
    {
        octaveSlider.setVisible(!octaveSlider.isVisible());
    }
private:
    CustomLookAndFeel& look;
    AudioProcessorValueTreeState& parameters;
    Piano* piano;
    AdvancedMenu* advancedMenu;

    Label octaveLabel;
    Slider octaveSlider;
    TextButton settings;

    std::unique_ptr<ParameterAttachment> hiAttachment;
    std::unique_ptr<ParameterAttachment> loAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BottomBar)
};
