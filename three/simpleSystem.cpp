
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem()
{
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;

	// YOUR CODE HERE

	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
		Vector3f pos ;//YOUR PARTICLE POSITION
	  glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
}
