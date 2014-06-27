// DO NOT mess with this file.  If you do, animator will not work with
// your model, and you'll have to write a new one.  If you really really
// really need to do something here (unlikely) then don't complain if and
// when animator doesn't work.  -- Eugene
#ifndef MODELERAPP_H
#define MODELERAPP_H

#include "ModelerView.h"
struct ModelerControl
{
    ModelerControl();
    ModelerControl(const char *name, float minimum, float maximum,
                   float stepsize, float value);
    ModelerControl(const ModelerControl & o);
    ModelerControl & operator=(const ModelerControl & o);
    
    void SetVals(const char *name, float minimum, float maximum,
                 float stepsize, float value);
    
    char m_name[128];
    float m_minimum;
    float m_maximum;
    float m_stepsize;
    float m_value;
};

// Forward declarations for ModelerApplication
class ModelerView;
class ModelerUserInterface;
class Fl_Box;
class Fl_Slider;
class Fl_Value_Slider;

// The ModelerApplication is implemented as a "singleton" design pattern,
// the purpose of which is to only allow one instance of it.
class ModelerApplication
{
public:
    ~ModelerApplication();
    
    // Fetch the global ModelerApplication instance
    static ModelerApplication *Instance();
    
    // Initialize the application; see sample models for usage
    void Init( int argc, char* argv[], const ModelerControl controls[], unsigned numControls);
    
    // Starts the application, returns when application is closed
    int Run();
    
    // Get and set slider values.
    double GetControlValue(int controlNumber);
    void SetControlValue(int controlNumber, double value);
    unsigned GetNumControls();
    
    // [update 05/01/02]
    bool GetAnimating();

 
private:
    // Private for singleton
    ModelerApplication() : m_numControls(-1) { }
    ModelerApplication(const ModelerApplication &) { }
    
    // The instance
    static ModelerApplication *m_instance;
    
    friend class ModelerUserInterface;
    
    void ShowControl(int controlNumber);
    void HideControl(int controlNumber);
    
    ModelerUserInterface * m_ui;
    
    int m_numControls;
    
    Fl_Box ** m_controlLabelBoxes;
    Fl_Value_Slider ** m_controlValueSliders;
    
    static void SliderCallback(Fl_Slider *, void *);
    static void RedrawLoop(void *);
    
    // Just a flag for updates
    bool m_animating;
};

#endif
