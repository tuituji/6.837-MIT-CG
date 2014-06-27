// modelerview.h

// This is the base class for your view window.  It contains
// a camera control for your use.  The draw() function will 
// set up default lighting and apply the projection.
  
#ifndef MODELERVIEW_H
#define MODELERVIEW_H
  
#ifdef WIN32
#include <windows.h>
#endif

#include <FL/Fl_Gl_Window.H>
#include <GL/gl.h>

class Camera;
class ModelerView;

#include "SkeletalModel.h"

using namespace std;

class ModelerView : public Fl_Gl_Window 
{
public:

    ModelerView(int x, int y, int w, int h, const char *label = 0);

    void loadModel(int argc, char* argv[]);
    
    virtual ~ModelerView ();
    
    virtual int handle(int event);
	virtual void update();
    virtual void draw();

	void updateJoints();
	void drawAxes();

    Camera *m_camera;
	SkeletalModel model;

	bool m_drawAxes;
	bool m_drawSkeleton;		// if false, the mesh is drawn instead.
};


#endif	/*  */
