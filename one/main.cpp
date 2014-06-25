#ifdef WIN32
#include <windows.h>
#endif

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>

#ifdef _WIN32
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif
#include <vecmath.h>

#include "parse.h"
#include "curve.h"
#include "surf.h"
#include "extra.h"
#include "camera.h"

using namespace std;

// If you're really interested in what "namespace" means, see
// Stroustup.  But basically, the functionality of putting all the
// globals in an "unnamed namespace" is to ensure that everything in
// here is only accessible to code in this file.
namespace
{
    // Global variables here.

    // This is the camera
    Camera camera;

    // These are state variables for the UI
    bool gMousePressed = false;
    int  gCurveMode = 1;
    int  gSurfaceMode = 1;
    int  gPointMode = 1;

    // This detemines how big to draw the normals
    const float gLineLen = 0.1f;
    
    // These are arrays for display lists for each drawing mode.  The
    // convention is that drawmode 0 is "blank", and other drawmodes
    // just call the appropriate display lists.
    GLuint gCurveLists[3];
    GLuint gSurfaceLists[3];
    GLuint gAxisList;
    GLuint gPointList;
   
    // These STL Vectors store the control points, curves, and
    // surfaces that will end up being drawn.  In addition, parallel
    // STL vectors store the names for the curves and surfaces (as
    // given by the files).
    vector<vector<Vector3f> > gCtrlPoints;
    vector<Curve> gCurves;
    vector<string> gCurveNames;
    vector<Surface> gSurfaces;
    vector<string> gSurfaceNames;

    // Declarations of functions whose implementations occur later.
    void arcballRotation(int endX, int endY);
    void keyboardFunc( unsigned char key, int x, int y);
    void specialFunc( int key, int x, int y );
    void mouseFunc(int button, int state, int x, int y);
    void motionFunc(int x, int y);
    void reshapeFunc(int w, int h);
    void drawScene(void);
    void initRendering();
    void loadObjects(int argc, char *argv[]);
    void makeDisplayLists();

    // This function is called whenever a "Normal" key press is
    // received.
    void keyboardFunc( unsigned char key, int x, int y )
    {
        switch ( key )
        {
        case 27: // Escape key
            exit(0);
            break;
        case ' ':
        {
            Matrix4f eye = Matrix4f::identity();
            camera.SetRotation(eye);
            camera.SetCenter(Vector3f(0,0,0));
            break;
        }
        case 'c':
        case 'C':
            gCurveMode = (gCurveMode+1)%3;
            break;
        case 's':
        case 'S':
            gSurfaceMode = (gSurfaceMode+1)%3;
            break;
        case 'p':
        case 'P':
            gPointMode = (gPointMode+1)%2;
            break;            
        default:
            cout << "Unhandled key press " << key << "." << endl;        
        }

        glutPostRedisplay();
    }

    // This function is called whenever a "Special" key press is
    // received.  Right now, it does nothing.
    void specialFunc( int key, int x, int y )
    {
		/*
        switch ( key )
        {
		default:
			break;
        }
		*/

        //glutPostRedisplay();
    }

    //  Called when mouse button is pressed.
    void mouseFunc(int button, int state, int x, int y)
    {
        if (state == GLUT_DOWN)
        {
            gMousePressed = true;
            
            switch (button)
            {
            case GLUT_LEFT_BUTTON:
                camera.MouseClick(Camera::LEFT, x, y);
                break;
            case GLUT_MIDDLE_BUTTON:
                camera.MouseClick(Camera::MIDDLE, x, y);
                break;
            case GLUT_RIGHT_BUTTON:
                camera.MouseClick(Camera::RIGHT, x,y);
            default:
                break;
            }                       
        }
        else
        {
            camera.MouseRelease(x,y);
            gMousePressed = false;
        }
        glutPostRedisplay();
    }

    // Called when mouse is moved while button pressed.
    void motionFunc(int x, int y)
    {
        camera.MouseDrag(x,y);        
    
        glutPostRedisplay();
    }

    // Called when the window is resized
    // w, h - width and height of the window in pixels.
    void reshapeFunc(int w, int h)
    {
        camera.SetDimensions(w,h);

        camera.SetViewport(0,0,w,h);
        camera.ApplyViewport();

        // Set up a perspective view, with square aspect ratio
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        camera.SetPerspective(50);
        camera.ApplyPerspective();
    }

    // This function is responsible for displaying the object.
    void drawScene(void)
    {
        // Clear the rendering window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode( GL_MODELVIEW );  
        glLoadIdentity();              

        // Light color (RGBA)
        GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
        GLfloat Lt0pos[] = {3.0,3.0,5.0,1.0};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
        glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

        camera.ApplyModelview();

        // Call the relevant display lists.
        if (gSurfaceMode)
            glCallList(gSurfaceLists[gSurfaceMode]);

        if (gCurveMode)
            glCallList(gCurveLists[gCurveMode]);

        // This draws the coordinate axes when you're rotating, to
        // keep yourself oriented.
        if (gMousePressed)
        {
            glPushMatrix();
            glTranslated(camera.GetCenter()[0], camera.GetCenter()[1], camera.GetCenter()[2]);
            glCallList(gAxisList);
            glPopMatrix();
        }

        if (gPointMode)
            glCallList(gPointList);
                 
        // Dump the image to the screen.
        glutSwapBuffers();


    }

    // Initialize OpenGL's rendering modes
    void initRendering()
    {
        glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
        glEnable(GL_LIGHTING);     // Enable lighting calculations
        glEnable(GL_LIGHT0);       // Turn on light #0.

        // Setup polygon drawing
        glShadeModel(GL_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Antialiasing
        // This looks like crap
        /*
          glEnable(GL_BLEND);
          glEnable(GL_POINT_SMOOTH);
          glEnable(GL_LINE_SMOOTH);
          glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);    
          glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        */
        
        // Clear to black
        glClearColor(0,0,0,1);

        // Base material colors (they don't change)
        GLfloat diffColor[] = {0.4, 0.4, 0.4, 1};
        GLfloat specColor[] = {0.9, 0.9, 0.9, 1};
        GLfloat shininess[] = {50.0};

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColor);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }

    // Load in objects from standard input into the global variables: 
    // gCtrlPoints, gCurves, gCurveNames, gSurfaces, gSurfaceNames.  If
    // loading fails, this will exit the program.
    void loadObjects(int argc, char *argv[])
    {
        if (argc < 2)
        {
            cerr<< "usage: " << argv[0] << " SWPFILE [OBJPREFIX] " << endl;
            exit(0);
        }

        ifstream in(argv[1]);
        if (!in)
        {
            cerr<< argv[1] << " not found\a" << endl;
            exit(0);
        }

        
        cerr << endl << "*** loading and constructing curves and surfaces ***" << endl;
        
        if (!parseFile(in, gCtrlPoints,
                       gCurves, gCurveNames,
                       gSurfaces, gSurfaceNames))
        {
            cerr << "\aerror in file format\a" << endl;
            in.close();
            exit(-1);              
        }

        in.close();

        // This does OBJ file output
        if (argc > 2)
        {
            cerr << endl << "*** writing obj files ***" << endl;
            
            string prefix(argv[2]);

            for (unsigned i=0; i<gSurfaceNames.size(); i++)
            {
                if (gSurfaceNames[i] != ".")
                {
                    string filename =
                        prefix + string("_")
                        + gSurfaceNames[i]
                        + string(".obj");

                    ofstream out(filename.c_str());

                    if (!out)
                    {
                        cerr << "\acould not open file " << filename << ", skipping"<< endl;
                        out.close();
                        continue;
                    }
                    else
                    {
                        outputObjFile(out, gSurfaces[i]);
                        cerr << "wrote " << filename <<  endl;
                    }
                }
            }
            
        }

        cerr << endl << "*** done ***" << endl;


    }

    void makeDisplayLists()
    {
        gCurveLists[1] = glGenLists(1);
        gCurveLists[2] = glGenLists(1);
        gSurfaceLists[1] = glGenLists(1);
        gSurfaceLists[2] = glGenLists(1);
        gAxisList = glGenLists(1);
        gPointList = glGenLists(1);

        // Compile the display lists
        
        glNewList(gCurveLists[1], GL_COMPILE);
        {
            for (unsigned i=0; i<gCurves.size(); i++)
                drawCurve(gCurves[i], 0.0);
        }
        glEndList();
                
        glNewList(gCurveLists[2], GL_COMPILE);
        {
            for (unsigned i=0; i<gCurves.size(); i++)
                drawCurve(gCurves[i], gLineLen);
        }
        glEndList();
        
        glNewList(gSurfaceLists[1], GL_COMPILE);
        {
            for (unsigned i=0; i<gSurfaces.size(); i++)
                drawSurface(gSurfaces[i], true);
        }
        glEndList();

        glNewList(gSurfaceLists[2], GL_COMPILE);
        {
            for (unsigned i=0; i<gSurfaces.size(); i++)
            {
                drawSurface(gSurfaces[i], false);
                drawNormals(gSurfaces[i], gLineLen);
            }
        }
        glEndList();

        glNewList(gAxisList, GL_COMPILE);
        {
            // Save current state of OpenGL
            glPushAttrib(GL_ALL_ATTRIB_BITS);

            // This is to draw the axes when the mouse button is down
            glDisable(GL_LIGHTING);
            glLineWidth(3);
            glPushMatrix();
            glScaled(5.0,5.0,5.0);
            glBegin(GL_LINES);
            glColor4f(1,0.5,0.5,1); glVertex3d(0,0,0); glVertex3d(1,0,0);
            glColor4f(0.5,1,0.5,1); glVertex3d(0,0,0); glVertex3d(0,1,0);
            glColor4f(0.5,0.5,1,1); glVertex3d(0,0,0); glVertex3d(0,0,1);

            glColor4f(0.5,0.5,0.5,1);
            glVertex3d(0,0,0); glVertex3d(-1,0,0);
            glVertex3d(0,0,0); glVertex3d(0,-1,0);
            glVertex3d(0,0,0); glVertex3d(0,0,-1);
        
            glEnd();
            glPopMatrix();
            
            glPopAttrib();
        }
        glEndList();

        glNewList(gPointList, GL_COMPILE);
        {
            // Save current state of OpenGL
            glPushAttrib(GL_ALL_ATTRIB_BITS);

            // Setup for point drawing
            glDisable(GL_LIGHTING);    
            glColor4f(1,1,0.0,1);
            glPointSize(4);
            glLineWidth(1);

            for (unsigned i=0; i<gCtrlPoints.size(); i++)
            {
                glBegin(GL_POINTS);
                for (unsigned j=0; j<gCtrlPoints[i].size(); j++)
                    glVertex(gCtrlPoints[i][j]);
                glEnd();

                glBegin(GL_LINE_STRIP);
                for (unsigned j=0; j<gCtrlPoints[i].size(); j++)
                    glVertex(gCtrlPoints[i][j]);
                glEnd();
            }
            
            glPopAttrib();
        }
        glEndList();

    }
    
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char* argv[] )
{
    // Load in from standard input
    loadObjects(argc, argv);

    glutInit(&argc,argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 600, 600 );
    
    camera.SetDimensions(600, 600);

    camera.SetDistance(10);
    camera.SetCenter(Vector3f(0,0,0));
    
    glutCreateWindow("Assignment 1");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

    // Set up callback functions for mouse
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);

    // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Trigger timerFunc every 20 msec
    //  glutTimerFunc(20, timerFunc, 0);

    makeDisplayLists();
        
    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop();

    return 0;	// This line is never reached.
}
