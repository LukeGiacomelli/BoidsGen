/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 23 Apr 2025 11:52:52am
    Author:  lgiac

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>

#define BORDER_WIDTH 1.5f
#define KNOB_SCALE 0.85f

class AdvancedLookAndFeel : public LookAndFeel_V4
{
public:
    AdvancedLookAndFeel()
    {
        setColour(TextButton::textColourOffId, Colour(249, 249, 249));
        setColour(TextButton::buttonColourId, Colour(0xff79b791));

        setColour(Label::backgroundColourId, Colour(249, 249, 249).withAlpha(0.2f));
        setColour(Label::textColourId, Colour(50, 55, 59));

        setColour(Label::backgroundColourId, Colours::transparentBlack);
        setColour(Label::textColourId, Colour(249, 249, 249).withAlpha(0.8f));
        setColour(Label::outlineColourId, Colours::transparentBlack);

        setColour(Slider::trackColourId, Colours::transparentBlack);
        setColour(Slider::textBoxTextColourId, Colour(249, 249, 249).withAlpha(0.8f));
    }

    void drawToggleButton(Graphics& g, ToggleButton& button, bool, bool) override
    {
        auto bounds = button.getLocalBounds().toFloat();
        bool isOn = button.getToggleState();
        bool isEnabled = button.isEnabled();

        Colour bgOn = Colour(0xFF79B791);
        Colour bgOff = Colour(249, 249, 249);
        Colour text = Colour(0xff32373b);

        g.setColour((isOn ? bgOn : bgOff).withAlpha(isEnabled ? 1.f : 0.4f));
        g.fillRoundedRectangle(bounds.reduced(2.0f), 6.0f);

        g.setColour(text.withAlpha(isEnabled ? 0.8f : 0.4f));
        g.setFont(Font("Helvetica", 16.0f, Font::bold));
        g.drawFittedText("Auto", bounds.toNearestInt(), Justification::centred, 1);
    }

    Font getTextButtonFont(TextButton&, int buttonHeight) override { return Font("Arial", 13.0f, Font::bold); }

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
    {
        //variabili utili
        const auto radius = jmin(width, height) * 0.5f * KNOB_SCALE - BORDER_WIDTH * 0.5f;
        const auto centreX = x + width * 0.5f;
        const auto centreY = y + height * 0.5f;
        const auto kx = centreX - radius;
        const auto ky = centreY - radius;
        const auto kw = radius * 2.0f;

        // Colori
        Colour bgColour = Colour::fromString("0xFF#3e3e3e");
        Colour markColour = Colour::fromString("0xFF79B791");

        // Disegna il cerchio di sfondo
        g.setColour(bgColour);
        g.fillEllipse(kx,ky,kw,kw);


        //riempimento bianco in base all’angolo
        const float angle = jmap(sliderPosProportional, rotaryStartAngle, rotaryEndAngle);
        Path p;
        p.addPieSegment(kx, ky, kw, kw, rotaryStartAngle, angle, 0.0f);

        g.setColour(Colours::white);
        g.fillPath(p);
    }
private:
	int numTicks = 21;
};