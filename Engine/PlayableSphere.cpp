//
//  PlayableSphere.cpp
//  AlphaLearn
//
//  Created by Luke Saxton on 10/02/2016.
//
//

#include "PlayableSphere.hpp"
#include "MidiScales.h"
#include "AlphaLive2Engine.hpp"

PlayableSphere::PlayableSphere(const int numPads, const int _sphereID) : sphereID(_sphereID)
{
    
    destination = InternalMidiRouter::MidiDestination::MidiOut;
    
    sphereData = AppData::Instance()->getSphereData(sphereID);
    
    if (sphereData == nullptr) {
        sphereID = AppData::Instance()->createNewSphereDataObject(numPads);
    }
    
    
    if (numPads > 0)
    {
        for (int i = 0; i < numPads; i++)
        {
            playablePads.add(new PlayablePad(sphereData->getPadData(i)));
        }
        
    }
    
    if (AppData::Instance()->getEnginePointer()->getMidiRouterPointer() != 0)
    {
        router = AppData::Instance()->getEnginePointer()->getMidiRouterPointer();
        mapSphere(36, Natural, TwoRow);
        midiThruEnabled = false;
        sphereMidiEnabled = true;

    }
    else
    {
        jassert(false);
        //Can't route things to a router that doesn't exist..
    }
    
   
    
    
}


PlayableSphere::~PlayableSphere()
{
    
}

void PlayableSphere::mapSphere(const int rootNote, const MappedScale scale, const RowConfig config = OneRow)
{
    
    currentRootNote = rootNote;
    AppData::Instance()->getGlobalScaleData()->setScale(scale);
    currentScale = MappedScale(AppData::Instance()->getGlobalScaleData()->getScale());
    currentRowConfig = config;
    AppData::Instance()->getGlobalScaleData()->setKey(rootNote % 12);
    AppData::Instance()->getGlobalScaleData()->setScale((rootNote/12) - 2);
    
    int octaveCount, positionInScale;
    octaveCount = positionInScale = 0;
    
    int index = 0;
    Array<int> sphereMask;
    sphereMask.clear();
    
    switch (config)
    {
        case OneRow:
            while (SphereMask::OneRowMask[index] != -1)
            {
                sphereMask.add(SphereMask::OneRowMask[index]);
                index++;
            }
            break;
        case TwoRow:
            while (SphereMask::TwoRowMask[index] != -1)
            {
                sphereMask.add(SphereMask::TwoRowMask[index]);
                index++;
            }
            break;
        case CenterRow:
            while (SphereMask::TwoRowMask[index] != -1)
            {
                sphereMask.add(SphereMask::CenterRowsMask[index]);
                index++;
            }
            break;
        default:
            jassert(false); //how did you manage that?
            break;
    }
    
    
    
    if (router != nullptr)
    {
        router->panic(destination);
    }
    //DBG("Map: " + String(rootNote) + " : " + String(scale));

    for (int i = 0; i < playablePads.size(); i++)
    {
        int maskValue = sphereMask.getUnchecked(i);
        
        
        octaveCount = maskValue / 7;
        positionInScale =  maskValue % 7;

        
        switch (scale) {
            case Major:
                playablePads.getUnchecked(i)->setMidiNote(rootNote + (octaveCount * 12) + IntervalSpacings::Major[positionInScale]);
                break;
            case Natural:
                playablePads.getUnchecked(i)->setMidiNote(rootNote + (octaveCount * 12) + IntervalSpacings::NaturalMinor[positionInScale]);
                break;
            case Harmonic:
                playablePads.getUnchecked(i)->setMidiNote(rootNote + (octaveCount * 12) + IntervalSpacings::HarmonicMinor[positionInScale]);
                break;
            case Hungarian:
                playablePads.getUnchecked(i)->setMidiNote(rootNote + (octaveCount * 12) + IntervalSpacings::Hungarian[positionInScale]);
                break;
            case Chromatic:
                playablePads.getUnchecked(i)->setMidiNote(i+rootNote);
            default:
                break;
        }
        
    }
}


void PlayableSphere::mapSphere(const int key, const int octave, const MappedScale scale, const RowConfig config = OneRow)
{
    int rootNote = 36 + (key + 12*octave); //C3 = 60; C-2 = 0;
    
    mapSphere(rootNote, scale, config);
}

void PlayableSphere::setRootNote(const int newRootNote)
{
    mapSphere(newRootNote, currentScale, currentRowConfig);
}

const int PlayableSphere::getRootNote()
{
    return currentRootNote;
}

void PlayableSphere::setRowConfig(const RowConfig newConfig)
{
    mapSphere(currentRootNote, currentScale, newConfig);
}


void PlayableSphere::hitPad(const int padID, const int vel)
{
    if (sphereMidiEnabled)
    {
        if (padID < playablePads.size())
        {
            playablePads[padID]->hitPad(vel);
        }
    }
    
}
void PlayableSphere::pressPad(const int padID, const float pressure)
{
    if (sphereMidiEnabled)
    {
        if (padID < playablePads.size())
        {
            playablePads[padID]->pressPad(pressure);
        }
    }
    
}

void PlayableSphere::midiThruToDestination (const int note, const int vel)
{
    if (midiThruEnabled)
    {
        router->handleInternalMidiNoteWithDestination(destination, note, vel);

    }
}

void PlayableSphere::setDestination (const InternalMidiRouter::MidiDestination newDestination)
{
    destination = newDestination;
}


const PlayableSphere::MappedScale PlayableSphere::getCurrentScale()
{
    return currentScale;
}

void PlayableSphere::setScale (MappedScale newScale)
{
    mapSphere(currentRootNote, newScale, currentRowConfig);
}

void PlayableSphere::setMidiThruEnabled (bool shouldBeEnabled)
{
    midiThruEnabled = shouldBeEnabled;
    if (!shouldBeEnabled)
    {
        router->panic(destination);
    }
}

void PlayableSphere::setSphereMidiEnabled(bool shouldBeEnabled)
{
    sphereMidiEnabled = shouldBeEnabled;
    if (!shouldBeEnabled)
    {
        router->panic(destination);
    }
}

void PlayableSphere::setSphereID(const int newID)
{
    sphereID = newID;
}

const int PlayableSphere::getSphereID()
{
    return sphereID;
}

void PlayableSphere::transposeMidiByOctave(const int octavesToTranspose)
{
    mapSphere(currentRootNote + (octavesToTranspose*12), currentScale, currentRowConfig);
}

void PlayableSphere::transposeMidiByNote(const int semiTonesToTranspose)
{
    mapSphere(currentRootNote + semiTonesToTranspose, currentScale, currentRowConfig);

}


