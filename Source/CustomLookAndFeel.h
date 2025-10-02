/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 23 Apr 2025 11:52:52am
    Author:  lgiac

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>

#define MIN_SLIDER_GAP 10 
#define MAX_SLIDER_GAP 79 

class CustomLookAndFeel : public LookAndFeel_V4
{
public:
    CustomLookAndFeel() 
    {
        setColour(TextButton::textColourOffId, Colour(249, 249, 249));
        setColour(TextButton::buttonColourId, Colour(0xff79b791));
        
        setColour(Label::backgroundColourId, Colour(249, 249, 249).withAlpha(0.2f));
        setColour(Label::textColourId, Colour(50, 55, 59));

        setColour(ComboBox::backgroundColourId, Colour(249, 249, 249).withAlpha(0.2f));
        setColour(ComboBox::textColourId, Colour(249, 249, 249));
        setColour(ComboBox::outlineColourId, Colours::transparentBlack);

        setColour(Label::backgroundColourId, Colours::transparentBlack);
        //setColour(Label::textColourId, Colour(249, 249, 249).withAlpha(0.5f));
        setColour(Label::textColourId, Colour::fromString("0xff32373b").withAlpha(0.6f));
        setColour(Label::outlineColourId, Colours::transparentBlack);

        setColour(Slider::textBoxTextColourId, Colours::black.withAlpha(0.6f));
    }

    void drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, Slider::SliderStyle, Slider& s) override
    {
        minSliderPos = maxSliderPos - minSliderPos <= MIN_SLIDER_GAP || maxSliderPos - minSliderPos > MAX_SLIDER_GAP ? safeMin : minSliderPos;
        maxSliderPos = maxSliderPos - minSliderPos <= MIN_SLIDER_GAP || maxSliderPos - minSliderPos > MAX_SLIDER_GAP ? safeMax : maxSliderPos;

        g.setColour(Colour::fromString("0xFF#3e3e3e"));

        g.drawLine(x, y, x, y + height);
        g.drawLine(x + width, y, x + width, y + height);

        g.fillRect(Rectangle<float>({ minSliderPos ,(float)y }, { maxSliderPos, (float)y + height }));

        if(maxSliderPos - minSliderPos > MIN_SLIDER_GAP && maxSliderPos - minSliderPos < MAX_SLIDER_GAP)
        {
            safeMin = minSliderPos;
            safeMax = maxSliderPos;
        }
    }

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
    {
        // Calcola il quadrato centrale più piccolo
        float diameter = std::min(width, height) - 8.0f; // padding
        float radius = diameter / 2.0f;
        float centreX = x + width / 2.0f;
        float centreY = y + height / 2.0f;
        Point<float> centre(centreX, centreY);
        Rectangle<float> knobArea(centreX - radius, centreY - radius, diameter, diameter);

        // Angolo per la lineetta
        float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // Colori
        Colour bgColour = Colour::fromString("0xFF#3e3e3e");
        Colour markColour = Colour::fromString("0xFF79B791");

        // Disegna il cerchio di sfondo
        g.setColour(bgColour);
        g.fillEllipse(knobArea);

        // Lineetta indicatore
        float pointerLength = radius * 0.95f;
        float pointerThickness = 3.0f;

        Point<float> endPoint = centre.getPointOnCircumference(pointerLength, angle);
        Point<float> startPoint = centre.getPointOnCircumference(radius * 0.4f, angle);

        g.setColour(markColour);
        g.drawLine(Line<float>(startPoint, endPoint), pointerThickness);
    }

    void drawToggleButton(Graphics& g, ToggleButton& button, bool, bool) override
    {
        auto bounds = button.getLocalBounds().toFloat();
        bool isOn = button.getToggleState();
        bool isEnabled = button.isEnabled();

        Colour bgOn = Colour::fromString("0xFF79B791"); 
        Colour bgOff = Colour::fromString("0xFF32373B"); 
        Colour text = Colour::fromString("0xFFF9F9F9"); 

        g.setColour((isOn ? bgOn : bgOff).withAlpha(isEnabled ? 1.f : 0.4f));
        g.fillRoundedRectangle(bounds.reduced(2.0f), 6.0f);

        g.setColour(text.withAlpha(isEnabled ? 0.8f : 0.4f));
        g.setFont(Font("Helvetica", 16.0f, Font::bold));
        g.drawFittedText("Tonnetz", bounds.toNearestInt(), Justification::centred, 1);
    }

    Font getTextButtonFont(TextButton&, int buttonHeight) override { return Font("Arial", 13.0f, Font::bold); }

    Rectangle<int> getTooltipBounds(const String& tipText,Point<int> mousePos,Rectangle<int> parentArea) override
    {
        auto r = LookAndFeel_V4::getTooltipBounds(tipText, {}, parentArea);

        r.setBounds(parentArea.getX(), hideTooltip ? parentArea.getHeight() : parentArea.getBottom() - 35, parentArea.getWidth(), 35);
        return r;
    }

    void drawTooltip(juce::Graphics& g, const juce::String& text, int width, int height) override
    {
        g.setColour(Colour(0xffa26769).brighter(0.1f));
        g.fillRoundedRectangle({ 0.f, 0.f, (float)width, (float)height }, 6.f);

        juce::AttributedString s;
        s.setJustification(juce::Justification::centredLeft);
        s.append(text, 15.f, Colours::white);

        juce::TextLayout tl;
        tl.createLayout(s, (float)width - 50);
        tl.draw(g, { (float)10, (float)2.5f, (float)width - 25, (float)height - 5});
    }

    void setHideTooltip(bool hide) { hideTooltip = hide; }
private:
    bool hideTooltip = false;
    float safeMax = 0, safeMin = 0;
};