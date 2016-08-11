//
//  MidiIPanel.hpp
//  Alphalive 2
//
//  Created by Luke Saxton on 03/08/2016.
//
//

#ifndef MidiIPanel_hpp
#define MidiIPanel_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "AppData.hpp"
#include "PadInspectorPanelBase.h"
#include "NoteSettingsPanel.hpp"




class MidiIPanel : public PadInspectorPanelBase,
                   public Button::Listener
{
public:
    
   
    
    MidiIPanel();
    ~MidiIPanel();
    void resized() override;
    void paint(Graphics& g) override;
    void refreshData() override;
    void padDataChangeCallback(const int changedData) override;
    void buttonClicked (Button* button) override;
    
private:
    TextButton noteButton, pressureButton;

    
    ScopedPointer<NoteSettingsPanel> noteSettingsPanel;
    // scoped pointer to pressure settings panel
    PadData* padData;
    bool panelEnabled;
    
    
};

#endif /* MidiIPanel_hpp */
