#include "PluginEditor.h"

//==============================================================================
MidiBoidsAudioProcessorEditor::MidiBoidsAudioProcessorEditor (MidiBoidsAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : audioProcessor (p), parameters(vts)
{
    //Le info sulle dimensioni del canvas le setto nell'header del piano
    menu.setLookAndFeel(&newLook);
    bottomMenu.setLookAndFeel(&newLook);
    knobsLayout.setLookAndFeel(&newLook);
    advancedMenu.setLookAndFeel(&advancedMenuLook);
    this->setLookAndFeel(&newLook);

    addAndMakeVisible(pd, 0);
    addAndMakeVisible(boid_scene);
    addAndMakeVisible(menu);
    addAndMakeVisible(knobsLayout);
    addAndMakeVisible(bottomMenu);
    addAndMakeVisible(advancedMenu);
    addAndMakeVisible(ttw);

    advancedMenu.setVisible(false);
    advancedMenu.getHideUIButton().onClick = [this]()
        {
            hideUI();
        };

}

MidiBoidsAudioProcessorEditor::~MidiBoidsAudioProcessorEditor() 
{
    menu.setLookAndFeel(nullptr);
    knobsLayout.setLookAndFeel(nullptr);
    bottomMenu.setLookAndFeel(nullptr);
    advancedMenu.setLookAndFeel(nullptr);
    this->setLookAndFeel(nullptr);
}

//==============================================================================
void MidiBoidsAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Colour::fromString("0xfff9f9f9"));
}

void MidiBoidsAudioProcessorEditor::resized() 
{
    menu.setBounds(0, 0, getWidth(), 80);
    pd.setBounds(piano->getPianoBoundsInTheScreen());
    boid_scene.setBounds(getBounds());
    knobsLayout.setBounds(getBounds().reduced(0, 50).translated(0, 125));
    bottomMenu.setBounds(0, getHeight() - 30, getWidth(), 30);
    advancedMenu.setBounds(getBounds().getX(), getHeight()-100, getWidth(), 100);
}

void MidiBoidsAudioProcessorEditor::hideUI() 
{
    menu.setVisible(!menu.isVisible());
    pd.setVisible(!pd.isVisible());
    knobsLayout.setVisible(!knobsLayout.isVisible());
    bottomMenu.hidePianoUI();
}

WrappedPluginEditor::WrappedPluginEditor(MidiBoidsAudioProcessor& p, AudioProcessorValueTreeState& vts)
: AudioProcessorEditor(p), audioProcessor(p), editor(p, vts)
{
    addAndMakeVisible(editor);

    if (auto* constrainer = getConstrainer())
    {
        constrainer->setFixedAspectRatio(static_cast<double>(originalWidth) / static_cast<double>(originalHeight));
        constrainer->setSizeLimits(originalWidth / 3, originalHeight / 3, originalWidth * 2, originalHeight * 2);
    }

    setResizable(true, false);
    setSize(originalWidth, originalHeight);
}

void WrappedPluginEditor::resized()
{
    const auto scaleFactor = static_cast<float>(getWidth()) / originalWidth;
    editor.setTransform(AffineTransform::scale(scaleFactor));
    editor.setBounds(0, 0, originalWidth, originalHeight);
}
