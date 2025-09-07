#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

#define sliderMouseSens 200
#define rotatorySliderMouseSens 100

typedef AudioProcessorValueTreeState::SliderAttachment sliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment buttonAttachment;

class AdvancedMenu : public Component
{
public:
	AdvancedMenu(AudioProcessorValueTreeState& vts, CustomLookAndFeel& lf)
		: parameters(vts), look(lf)
	{
		//Close button
		Image closeImg = ImageCache::getFromMemory(BinaryData::CloseWindowicon_png, BinaryData::CloseWindowicon_pngSize);
		closeBtn.setImages(
			false,
			true,
			true,
			closeImg,
			1.f,
			Colour(249, 249, 249),
			closeImg, 0.5f, Colour(249, 249, 249).withAlpha(0.5f),
			closeImg, 0.8f, Colour(249, 249, 249).withAlpha(0.8f)
		);
		closeBtn.onClick = [this]()
			{
				this->setVisible(!this->isVisible());
				look.setHideTooltip(false);
			};

		//Hide UI button
		Image hideImg = ImageCache::getFromMemory(BinaryData::hideUI_png, BinaryData::hideUI_pngSize);
		hideUI.setImages(
			false,
			true,
			true,
			hideImg,
			1.f,
			Colour(249, 249, 249),
			hideImg, 0.5f, Colour(249, 249, 249).withAlpha(0.5f),
			hideImg, 0.8f, Colour(249, 249, 249).withAlpha(0.8f)
		);
		hideUILabel.setText("Hide UI: ", dontSendNotification);
		hideUILabel.attachToComponent(&hideUI, true);

		//Boids number
		boidsNumberSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
		boidsNumberSlider.setSliderSnapsToMousePosition(false);
		boidsNumberSlider.setMouseDragSensitivity(sliderMouseSens);
		bnAttachment.reset(new sliderAttachment(parameters, Parameters::nameBoidsNumber, boidsNumberSlider));

		boidsNumber.setText("Boids number:", dontSendNotification);
		boidsNumber.attachToComponent(&boidsNumberSlider, true);

		//Max speed
		max_speed.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
		max_speed.setSliderSnapsToMousePosition(false);
		max_speed.setMouseDragSensitivity(sliderMouseSens);
		msAttachment.reset(new sliderAttachment(parameters, Parameters::nameBoidsMaxSpeed, max_speed));

		maxSpeedLabel.setText("Max speed: ", dontSendNotification);
		maxSpeedLabel.attachToComponent(&max_speed, true);

		//Threshold
		thrSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
		thrSlider.setRotaryParameters(0.0f, MathConstants<float>::twoPi, true);
		thrSlider.setMouseDragSensitivity(rotatorySliderMouseSens);
		thrAttachment.reset(new sliderAttachment(parameters, Parameters::nameThreshold, thrSlider));
		thrSlider.setEnabled(!autoThresh.getToggleState());

		thrLabel.setText("Notes threshold: ", dontSendNotification);
		thrLabel.attachToComponent(&thrSlider, true);
		thrLabel.setEnabled(!autoThresh.getToggleState());

		autoThresh.setClickingTogglesState(true);
		autoThresh.setToggleState(true, dontSendNotification);
		autoThrAttachment.reset(new buttonAttachment(parameters, Parameters::nameAutoThreshold, autoThresh));
		autoThresh.onClick = [this]()
			{
				thrSlider.setEnabled(!autoThresh.getToggleState());
				thrLabel.setEnabled(!autoThresh.getToggleState());
			};
		

		//Boids bias
		biasSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
		biasSlider.setRotaryParameters(0.0f, MathConstants<float>::twoPi, true);
		biasSlider.setMouseDragSensitivity(rotatorySliderMouseSens);
		biasAttachment.reset(new sliderAttachment(parameters, Parameters::nameBoidsBias, biasSlider));

		biasLabel.setText("Boids movements bias: ", dontSendNotification);
		biasLabel.attachToComponent(&biasSlider, true);

		//Reset parameters
		Image resetImg = ImageCache::getFromMemory(BinaryData::reset_png, BinaryData::reset_pngSize);
		resetParameters.setImages(
			false,
			true,
			true,
			resetImg,
			1.f,
			Colour(249, 249, 249),
			resetImg, 0.5f, Colour(249, 249, 249).withAlpha(0.5f),
			resetImg, 0.8f, Colour(249, 249, 249).withAlpha(0.8f)
		);
		resetParameters.setClickingTogglesState(true);
		resetLabel.setText("Reset parameters: ", dontSendNotification);
		resetLabel.attachToComponent(&resetParameters, true);
		resetLabel.setColour(Label::textColourId, Colour(50, 55, 59).brighter(0.6f));
		resetAttachment.reset(new buttonAttachment(parameters, Parameters::nameReset, resetParameters));

		//Credits
		credits.setText("Made by Giacomelli Luca", dontSendNotification);
		credits.setColour(Label::textColourId, Colour(50, 55, 59).brighter(0.6f));

		addAndMakeVisible(boidsNumber, 0);
		addAndMakeVisible(boidsNumberSlider, 0);
		addAndMakeVisible(hideUILabel);
		addAndMakeVisible(hideUI);
		addAndMakeVisible(closeBtn);
		addAndMakeVisible(max_speed);
		addAndMakeVisible(maxSpeedLabel);
		addAndMakeVisible(thrSlider);
		addAndMakeVisible(thrLabel);
		addAndMakeVisible(autoThresh);
		addAndMakeVisible(biasSlider);
		addAndMakeVisible(biasLabel);
		addAndMakeVisible(resetParameters);
		addAndMakeVisible(resetLabel);
		//addAndMakeVisible(credits);
	}

	~AdvancedMenu() {};

	void paint(Graphics& g) override
	{
		g.fillAll(Colour(0xff32373b));
	}

	void resized() override
	{
		auto area = getLocalBounds().reduced(20,20);

		closeBtn.setBounds(area.getWidth()-10, area.getY()-10, 40, 40);
		boidsNumberSlider.setBounds(area.getX() + 100, area.getY(), 50, 25);
		hideUI.setBounds(area.getX() + 100, area.getY() + 35, 25, 25);
		max_speed.setBounds(area.getX() + 270, area.getY(), 50, 25);
		thrSlider.setBounds(area.getX() + 270, area.getY()+35, 30, 30);
		autoThresh.setBounds(area.getX() + 310, area.getY()+38, 45, 25);
		biasSlider.setBounds(area.getX() + 500, area.getY(), 30, 30);
		//resetParameters.setBounds(area.getX() + 500, area.getY()+35, 25, 25);
		resetParameters.setBounds(area.getWidth() - 10, area.getHeight() + 10, 25, 25);

		//credits.setBounds(area.getWidth() - 130, area.getHeight() + 10, 150, 25);
	}

	ImageButton& getHideUIButton() { return hideUI; }

private:
	CustomLookAndFeel& look;

	Label hideUILabel, boidsNumber, maxSpeedLabel, thrLabel, biasLabel, credits, resetLabel;
	ImageButton closeBtn, hideUI, resetParameters;
	ToggleButton autoThresh;

	Slider boidsNumberSlider, max_speed, thrSlider, biasSlider;

	std::unique_ptr<sliderAttachment> bnAttachment;
	std::unique_ptr<sliderAttachment> thrAttachment;
	std::unique_ptr<sliderAttachment> msAttachment;
	std::unique_ptr<sliderAttachment> biasAttachment;
	std::unique_ptr<buttonAttachment> autoThrAttachment;
	std::unique_ptr<buttonAttachment> resetAttachment;

	AudioProcessorValueTreeState& parameters;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdvancedMenu)
};