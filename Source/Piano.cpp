/*
  ==============================================================================

    Piano.cpp
    Created: 8 Apr 2025 3:22:30pm
    Author:  lgiac

  ==============================================================================
*/

#include "Piano.h"

#define fifth 7
#define major_third 4
#define pianoYShift 50
#define MAX_OCTAVE_RANGE 5 //Non sincronizzato con con bottom bar component (+1 che in bottom bar)

Piano::Piano(int low_o, int high_o, String scale_key, MidiManager& midi, int& n_boids)
    : low_oct(low_o), high_oct(high_o), scale(scale_key)
{
    int maxNumberOfNotes = MAX_OCTAVE_RANGE * 12; //12 note per ottava

    //Cerco la successiva radice intera (per fare un quadrato)
    while ((sqrt(maxNumberOfNotes) - (int)sqrt(maxNumberOfNotes)) != 0) {
        maxNumberOfNotes++;
    }
    auto num_of_rows = std::ceil(sqrt(maxNumberOfNotes));
    maxNumberOfNotes += 8; //Numero di note in caso di tonnetz (worst-case)

    areas_collection.clear();
    areas_collection.resize(num_of_rows, std::vector<Area>(num_of_rows, Area("/", low_oct, 0, midi, n_boids, areasThreshold, autoThreshold))); //Inizializzo

    updatePiano();
    updatePianoTonality(currentTonality);
}

void Piano::updatePiano()
{
    resetPiano();
    updated = false;

    int oct_range = high_oct - low_oct;
    numberOfNotes = oct_range * 12; //12 note per ottava
    int numberOfRows = std::ceil(sqrt(numberOfNotes));
    int additionalAreas = int(numberOfRows / 2); //Per tonnetz

    int counter = 0;

    //Riempo le chroma e octave
    if (scale == "Chrom") {
        for (int i = 0; i < numberOfRows; ++i) {
            for (int j = 0; j < numberOfRows; ++j) {
                areas_collection[i][j].setNota(notes[counter % 12], (int)((counter / 12) + low_oct));
                areas_collection[i][j].setUsed(true);
                counter++;
            }
        }
    }
    else //Tonnetz
    {
        auto starting_note = 0;
        auto numberOfCols = numberOfRows;
        for (int i = 0; i < numberOfRows; i++)
        {
            //Additional areas per piano tonnetz
            if (i < additionalAreas || (numberOfRows - i) <= additionalAreas)
                numberOfCols = numberOfRows + 1;
            else
                numberOfCols = numberOfRows;

            for (int j = 0; j < numberOfCols; j++)
            {
                areas_collection[i][j].setNota(notes[(starting_note + (j * fifth)) % 12], (int)(counter / 12) + low_oct);
                areas_collection[i][j].setUsed(true);
                ++counter;
            }
            starting_note += major_third;
        }
    }

    //----------------Setto le dimensioni del piano-----------------
    float cellWidth = piano_width / numberOfRows; 
    auto deltaOffset = scale == "Tonnetz" ? cellWidth / 2 : 0; //se tonnetz sfasamento di cellwidth/2

    auto startOffset = 0.f;
    auto numberOfCols = numberOfRows;
    for (size_t i = 0; i < numberOfRows; ++i) 
    {
        //Additional areas per piano tonnetz
        if ((i < additionalAreas || (numberOfRows - i) <= additionalAreas) && scale == "Tonnetz")
        {
            numberOfCols = numberOfRows + 1;
            if ((numberOfRows - i) == additionalAreas) startOffset -= deltaOffset * 2;
        }
        else
            numberOfCols = numberOfRows;

        for (size_t j = 0; j < numberOfCols; ++j)
        { 
            if(areas_collection[i][j].getNota().getChroma() != "/")
                areas_collection[i][j].setAreaBounds(startOffset + (j * cellWidth), i * cellWidth, cellWidth, cellWidth);
        
        }
        startOffset += deltaOffset;
    }

    updatePianoBoundsInTheScreen(numberOfRows, cellWidth, deltaOffset);
}

void Piano::resetPiano()
{
    //Ogni cambio sulle ottave resetto le note sul piano per ricalcolarle
    for (auto& row : areas_collection)
        for (auto& a : row)
        {
            a.setNota("/", 0);
            a.setUsed(false);
            a.setAreaBounds(-100, -100, 0, 0); //Se non utilizzata la posiziono fuori dall schermo e con dimensioni nulle
        }
}

void Piano::updatePianoTonality(String newTonality)
{
    currentTonality = newTonality;

    auto ton = tonalities[currentTonality];
    for (auto& r : areas_collection)
    {
        for (auto& a : r)
        {
            //Se a.nota è nella scala selezionata la setto come presente
            auto in = std::find(ton.begin(), ton.end(), a.getNota().getChroma()) != ton.end();
            a.setInSelectedTonality(in);
        }
    }
}


void Piano::updatePianoBoundsInTheScreen(int cols, float cellWidth, float deltaOffset)
{
    pianoBoundsInTheScreen.setBounds(
        ((screen_width - lato) / 2) - ((cols - 1) * deltaOffset) / 2,
        (screen_height - lato) / 2,
        lato + ((cols - 1) * deltaOffset),
        lato);

    pianoBoundsInTheScreen.translate(0,pianoYShift); //Lo shifto per considerare l'origine dei cerchi al centro dell'area e per il topbarmenu 
}

