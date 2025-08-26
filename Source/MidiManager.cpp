/*
  ==============================================================================

    MidiManager.cpp
    Created: 30 May 2025 12:25:01pm
    Author:  lgiac

  ==============================================================================
*/

#include "MidiManager.h"

void MidiManager::releaseResources() //Non funziona perchè non viene più richiamato la processBlock!
{
    newMidiMessage.push_back(MidiMessage::allNotesOff(1));
}

bool MidiManager::getNotesPressed(MidiBuffer& midiMessages, std::array<String, 7>& pressedNotes, int& noteCount)
{
    bool toUpdate = false;

    for (const auto notes : midiMessages)
    {
        const auto& msg = notes.getMessage();
        if (msg.isNoteOn())
        {
            int noteNumber = msg.getNoteNumber();
            pressedNotes[noteCount++ % pressedNotes.size()] = MidiMessage::getMidiNoteName(noteNumber, true, false, false);
            //pressedNotes[noteNumber % pressedNotes.size()] = MidiMessage::getMidiNoteName(noteNumber, true, false, false);
            toUpdate = true;
        }
        else if (msg.isNoteOff())
        {
            int noteNumber = msg.getNoteNumber();
            std::replace(pressedNotes.begin(), pressedNotes.end(), MidiMessage::getMidiNoteName(noteNumber, true, false, false), (String)"");

            //pressedNotes[noteNumber % pressedNotes.size()] = "";
            toUpdate = true;
        }
    }

    return toUpdate;
}

void MidiManager::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    MidiBuffer processedMidi;
    

    for (int i = 0; i < newMidiMessage.size(); ++i)
    {
        if(newMidiMessage[i].isNoteOn() || newMidiMessage[i].isNoteOff())
        {
            processedMidi.addEvent(newMidiMessage[i], i);
        }
    }

    midiMessages.swapWith(processedMidi);
    newMidiMessage.clear();
}

void MidiManager::addNote(double midiNumber, bool note_on, float vel)
{
    auto msg = note_on ? MidiMessage::noteOn(1, midiNumber, vel) : MidiMessage::noteOff(1, midiNumber, vel);
    newMidiMessage.push_back(msg);
}
