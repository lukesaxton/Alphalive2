 //
 //  LowpassFilter.h
 //  AlphaLive
 //
 //  Created by Liam Lacey on 15/12/2011.
 //  Updated for new Framework by Luke Saxton on 13/10/2015
 //  Copyright 2015 nu desine.
 //
 //  This file is part of AlphaLive.
 //
 //  AlphaLive is free software: you can redistribute it and/or modify
 //  it under the terms of the GNU General Public License, version 2,
 //  as published by the Free Software Foundation.
 //
 //  AlphaLive is distributed in the hope that it will be useful,
 //  but WITHOUT ANY WARRANTY; without even the implied warranty of
 //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 //  GNU General Public License for more details.
 //
 //  You should have received a copy of the GNU General Public License
 //  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef H_LOWPASSFILTER
#define H_LOWPASSFILTER


#include "Dsp.h"
#include "FXHeader.h"
class LowpassFilter : public WrappedAudioEffect
{
public:
    enum Parameters
    {
        Mix = 0,
        Frequency,
        Bandwidth
    };
    static const int numParameters = 3;
    
	LowpassFilter(FXInits* fxInits);
	~LowpassFilter();
    
    //Function that processes the audio
    void processAudio (const AudioSourceChannelInfo& bufferToFill) override;
    //Function that takes in a pressure value and modulates the filter depending on the set AlphaTouch parameter
    void processAlphaTouch (double pressureValue) override;
    
    //Functions that set the effect parameters
    void setFrequency (double value);
    void setMix (double value);
    void setBandwidth( double value);
    
    //Functions that set the AlphaTouch settings
	void setAlphaTouchParam (int value) override;
    void setAlphaTouchIntensity (double value) override;
    void setAlphaTouchReverse (int value) override;
    
    void setSampleRate (float value) override;

    //Added with Macro Ability
    void setParameter(const int parameter, const double newValue) override;
    const String getEffectName() override;
    const Array<FXData::MinMax>* getMinMax() override;
    const StringArray* getParameterNames() override;
    const double getParameterValue(const int parameter) override;
    const int getNumParameters() override;
    
    const String effectName = "Low-Pass Filter";

private:

    //The filter
    Dsp::Filter *filter;
    Dsp::Params params;
    
    double mix, mixPrev, mixControlValue;
    double frequencyControlValue;
    double bandwidthControlValue;
    
    int alphaTouchParam;
    double alphaTouchIntensity;
    bool alphaTouchReverse;
    
    CriticalSection sharedMemory;
    
    int sampleRate;
    
    Array<FXData::MinMax> minMaxValues;
    ScopedPointer<StringArray> parameterNames;
};

#endif //H_LOWPASSFILTER