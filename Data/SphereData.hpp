//
//  SphereData.hpp
//  AlphaLive Midi
//
//  Created by Luke Saxton on 15/07/2016.
//
//
//  Alphalive 2 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2,
//  as published by the Free Software Foundation.
//
//  Alphalive 2 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SphereData_hpp
#define SphereData_hpp
#include "AppDataTypes.h"
#include "PadData.hpp"

/*
    AppDataFormat object containing a number of PadData objects. Contains all the data you need for a 
    PlayableSphere - Key, Scale and Tempo info as well as misc state flags and child PadData objects.
 
    When saving is finally implemented, this class should convert its data to a ValueTree for export
    to XML.
 
    See AppDataListeners for more info on AppDataFomat.
 
*/

class SphereData : public AppDataFormat
{
public:
    enum SphereType
    {
        Instrument =  0,
        Controller,
        Empty,
        FINAL_SPHERETYPE
    };
    
    enum MappedScale
    {
        Major = 0,
        Natural,
        Harmonic,
        Hungarian,
        Chromatic,
        FINAL_SCALE
    };
    
    enum RowConfig
    {
        OneRow = 0,
        TwoRow,
        CenterRow,
        FINAL_ROWCONF
    };
    
    enum DataIDS
    {
        SphereID = 0,
        SphereTypeData,
        RootNote,
        Octave,
        Scale,
        RowConfigData,
        MidiThru,
        MidiEnabled,
        FINAL_ID
    };
    
    SphereData(int numPads = 48, const int _sphereID = -1);
    ~SphereData();
    
    ValueTree* toValueTree();
    
    //============= GETS ===========================================
    //Objects
    PadData* getPadData(const int forPad);
    const int getNumPadDataObjects();
    TempoData* getTempoData();
    ScaleData* getScaleData();
    
    //Variables
    const int getSphereID();
    const enum SphereType getSphereType();
    const int getRootNote();
    const int getOctave();
    const enum MappedScale getScale();
    const enum RowConfig getRowConfig();
    const bool getMidiThruEnabled();
    const bool getSphereMidiEnabled();
    //============= SETS ===========================================
    //Variables
    bool setSphereID(const int newID);
    bool setSphereType(const int newType);
    bool setRootNote(const int newRootNote);
    bool setOctave(const int newOctave);
    bool setScale(const int newScale);
    bool setRowConfig(const int newConfig);
    void setMidiThruEnabled(const bool isEnabled);
    void setSphereMidiEnabled(const bool isEnabled);
    
    bool editPadGroup(const int groupID, const PadData::DataIDs dataID, const var changedData);
    
private:
    
    //Objects
    OwnedArray<PadData> padData;
    TempoData tempoData;
    ScaleData scaleData;
    
    CriticalSection dataLock;
    
    //Variables
    int sphereID = -1;
    SphereType sphereType = Empty;
    int rootNote = 36;
    int octave = 0;
    MappedScale scale;
    RowConfig rowConfig;
    bool midiThruEnabled = false;
    bool sphereMidiEnabled = true;
    

};





#endif /* SphereData_hpp */
