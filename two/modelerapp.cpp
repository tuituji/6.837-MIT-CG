#include "modelerapp.h"
#include "ModelerView.h"
#include "modelerui.h"

#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Box.H>

#include <cstring>
#include <cstdio>
#include <cstdlib>

// CLASS ModelerControl METHODS

ModelerControl::ModelerControl():m_minimum(0.0f), m_maximum(1.0f), m_stepsize(0.1f),
m_value(0.0f)
{
}

ModelerControl::ModelerControl(const char *name, float minimum,
			       float maximum, float stepsize, float value)
{
    SetVals(name, minimum, maximum, stepsize, value);
}

ModelerControl::ModelerControl(const ModelerControl & o)
{
    operator=(o);
}

ModelerControl & ModelerControl::operator=(const ModelerControl & o)
{
    if (this != &o)
	SetVals(o.m_name, o.m_minimum, o.m_maximum, o.m_stepsize,
		o.m_value);
    return *this;
}

void ModelerControl::SetVals(const char *name, float minimum,
			     float maximum, float stepsize, float value)
{
    strncpy(m_name, name, 128);
    m_minimum = minimum;
    m_maximum = maximum;
    m_stepsize = stepsize;
    m_value = value;
}


// ****************************************************************************


// Set the singleton initially to a NULL instance
ModelerApplication *ModelerApplication::m_instance = NULL;

// CLASS ModelerApplication METHODS

ModelerApplication *ModelerApplication::Instance()
{
    // Make a new instance if none exists, otherwise, return
    // the existing instance of the ModelerApplication
    return (m_instance) ? (m_instance) : (m_instance =
					  new ModelerApplication());
}

void ModelerApplication::Init( int argc, char* argv[],
							  const ModelerControl controls[],
							  unsigned numControls )
{
    int i;

    m_animating = false;
    m_numControls = numControls;

    // ********************************************************
    // Create the FLTK user interface
    // ********************************************************

    m_ui = new ModelerUserInterface();

    // Store pointers to the controls for manipulation
    m_controlLabelBoxes = new Fl_Box *[numControls];
    m_controlValueSliders = new Fl_Value_Slider *[numControls];

    // Constants for user interface setup
    const int textHeight = 20;
    const int sliderHeight = 20;
    const int packWidth = m_ui->m_controlsPack->w();

    m_ui->m_controlsPack->begin();
    // For each control, add appropriate objects to the user interface
    for (i = 0; i < m_numControls; i++) {
	// Add the entry to the selection box
	m_ui->m_controlsBrowser->add(controls[i].m_name);

	// Add the label (but make it invisible for now)
	Fl_Box *box =
	    new Fl_Box(0, 0, packWidth, textHeight, controls[i].m_name);
	box->labelsize(10);
	box->hide();
	box->box(FL_FLAT_BOX);	// otherwise, Fl_Scroll messes up (ehsu)
	m_controlLabelBoxes[i] = box;

	// Add the slider (but make it invisible for now)
	Fl_Value_Slider *slider =
	    new Fl_Value_Slider(0, 0, packWidth, sliderHeight, 0);
	slider->type(1);
	slider->range(controls[i].m_minimum, controls[i].m_maximum);
	slider->step(controls[i].m_stepsize);
	slider->value(controls[i].m_value);
	slider->hide();
	m_controlValueSliders[i] = slider;
	slider->
	    callback((Fl_Callback *) ModelerApplication::SliderCallback);
    }
    m_ui->m_controlsPack->end();

    // Make sure that we remove the view from the
    // Fl_Group, otherwise, it'll blow up 
    // THIS BUG FIXED 04-18-01 ehsu
    m_ui->m_modelerWindow->remove(*(m_ui->m_modelerView));
    delete m_ui->m_modelerView;

    m_ui->m_modelerWindow->begin();

    m_ui->m_modelerView = new ModelerView(0, 0, m_ui->m_modelerWindow->w(), m_ui->m_modelerWindow->h(), NULL);
    m_ui->m_modelerView->loadModel(argc, argv);

    Fl_Group::current()->resizable(m_ui->m_modelerView);
    m_ui->m_modelerWindow->end();
}

ModelerApplication::~ModelerApplication()
{
    // FLTK handles widget deletion
    delete m_ui;
    delete [] m_controlLabelBoxes;
    delete [] m_controlValueSliders;
}

int ModelerApplication::Run()
{
    if (m_numControls == -1) {
	fprintf(stderr,
		"ERROR: ModelerApplication must be initialized before Run()!\n");
	return -1;
    }
    
    // Just tell FLTK to go for it.
    Fl::visual(FL_RGB | FL_DOUBLE);
    m_ui->show();

    return Fl::run();
}

double ModelerApplication::GetControlValue(int controlNumber)
{
    return m_controlValueSliders[controlNumber]->value();
}

void ModelerApplication::SetControlValue(int controlNumber, double value)
{
    m_controlValueSliders[controlNumber]->value(value);
}

unsigned ModelerApplication::GetNumControls()
{
    return m_numControls;
}

bool ModelerApplication::GetAnimating()
{
    return m_animating;
}

void ModelerApplication::ShowControl(int controlNumber)
{
    m_controlLabelBoxes[controlNumber]->show();
    m_controlValueSliders[controlNumber]->show();
    m_ui->m_controlsWindow->redraw();
}

void ModelerApplication::HideControl(int controlNumber)
{
    m_controlLabelBoxes[controlNumber]->hide();
    m_controlValueSliders[controlNumber]->hide();
    m_ui->m_controlsWindow->redraw();
}

void ModelerApplication::SliderCallback(Fl_Slider *, void *)
{
	ModelerApplication::Instance()->m_ui->m_modelerView->update();
    ModelerApplication::Instance()->m_ui->m_modelerView->redraw();
}
