#pragma once

#include <JuceHeader.h>

typedef AudioProcessorValueTreeState::SliderAttachment sliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment buttonAttachment;

class AdvancedMenu : public Component
{
public:
	AdvancedMenu(AudioProcessorValueTreeState& vts)
		: parameters(vts)
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
		bnAttachment.reset(new sliderAttachment(parameters, Parameters::nameBoidsNumber, boidsNumberSlider));

		boidsNumber.setText("Boids number:", dontSendNotification);
		boidsNumber.attachToComponent(&boidsNumberSlider, true);

		//Max speed
		max_speed.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
		msAttachment.reset(new sliderAttachment(parameters, Parameters::nameBoidsMaxSpeed, max_speed));

		maxSpeedLabel.setText("Max speed: ", dontSendNotification);
		maxSpeedLabel.attachToComponent(&max_speed, true);

		//Threshold
		thrSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
		thrSlider.setRotaryParameters(0.0f, MathConstants<float>::twoPi, true);
		thrAttachment.reset(new sliderAttachment(parameters, Parameters::nameThreshold, thrSlider));

		thrLabel.setText("Notes threshold: ", dontSendNotification);
		thrLabel.attachToComponent(&thrSlider, true);

		autoThresh.setClickingTogglesState(true);
		autoThresh.setToggleState(true, false);
		autoThrAttachment.reset(new buttonAttachment(parameters, Parameters::nameAutoThreshold, autoThresh));

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
	}

	ImageButton& getHideUIButton() { return hideUI; }

private:
	Label hideUILabel, boidsNumber, maxSpeedLabel, thrLabel;
	ImageButton closeBtn, hideUI;
	ToggleButton autoThresh;

	Slider boidsNumberSlider, max_speed, thrSlider;

	std::unique_ptr<sliderAttachment> bnAttachment;
	std::unique_ptr<sliderAttachment> thrAttachment;
	std::unique_ptr<sliderAttachment> msAttachment;
	std::unique_ptr<buttonAttachment> hideUIAttachment;
	std::unique_ptr<buttonAttachment> autoThrAttachment;

	AudioProcessorValueTreeState& parameters;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdvancedMenu)
};