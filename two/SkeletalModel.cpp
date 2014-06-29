#include "SkeletalModel.h"

#include <FL/Fl.H>

using namespace std;

void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile)
{
	loadSkeleton(skeletonFile);

	m_mesh.load(meshFile);
	m_mesh.loadAttachments(attachmentsFile, m_joints.size());

	computeBindWorldToJointTransforms();
	updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(Matrix4f cameraMatrix, bool skeletonVisible)
{
	// draw() gets called whenever a redraw is required
	// (after an update() occurs, when the camera moves, the window is resized, etc)

	m_matrixStack.clear();
	m_matrixStack.push(cameraMatrix);

	if( skeletonVisible )
	{
		drawJoints();

		drawSkeleton();
	}
	else
	{
		// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
		glLoadMatrixf(m_matrixStack.top());
		// Tell the mesh to draw itself.
		m_mesh.draw();
	}
}

void SkeletalModel::loadSkeleton( const char* filename )
{
	// Load the skeleton from file here.
	const unsigned int  MAX_SIZE = 100;
	char buffer[MAX_SIZE];
	vector<Joint *> joints;
	ifstream in(filename);
	Vector3f v;
	int id;
	while(in.getline(buffer, MAX_SIZE)) {
		stringstream ss(buffer);
		string s;
		ss >> v[0] >> v[1] >> v[2]>>  id;
		Joint *joint = new Joint;
		joint->transform = Matrix4f::translation(v);
		m_joints.push_back(joint);
		if(id !=  -1) {
			Joint* parent = m_joints[id];
			parent->children.push_back(joint); // add to parents
			joint->bindWorldToJointTransform = 
					parent->bindWorldToJointTransform * joint->transform;
		}
		else {
			m_rootJoint = joint;
			joint->bindWorldToJointTransform = joint->transform;
		}
	}
}

void traverse_joint(MatrixStack& stack,Joint *joint)
{
	stack.push(joint->transform);
	glLoadMatrixf(stack.top());  // here we use glLoadMatrixf
	glutSolidSphere( 0.025f, 12, 12 );	
	std::vector< Joint* >::iterator itr;
	for(itr = joint->children.begin(); itr != joint->children.end(); ++itr){
		traverse_joint(stack, *itr);
	}
	stack.pop();
}

void SkeletalModel::drawJoints( )
{
	// Draw a sphere at each joint. You will need to add a recursive helper function to traverse the joint hierarchy.
	//
	// We recommend using glutSolidSphere( 0.025f, 12, 12 )
	// to draw a sphere of reasonable size.
	//
	// You are *not* permitted to use the OpenGL matrix stack commands
	// (glPushMatrix, glPopMatrix, glMultMatrix).
	// You should use your MatrixStack class
	// and use glLoadMatrix() before your drawing call.
	traverse_joint(m_matrixStack, m_rootJoint);
}

void traverse_skeleton(MatrixStack &stack, Joint *joint)
{
	stack.push(joint->transform);
//	glLoadMatrixf(stack.top());  // here we use glLoadMatrixf
//	glutSolidCube(.0);	
	std::vector< Joint* >::iterator itr;
	for(itr = joint->children.begin(); itr != joint->children.end(); ++itr){
		Matrix4f m = stack.top();
		Vector3f v(0.0, 0.0, 0.0);
		Vector4f p(0.0, 0.0, 0.0, 1.0);
		p = (*itr)->transform * p;
		v = p.xyz();
		Matrix4f scale = Matrix4f::scaling(50 * v.abs(), 1, 1);
		Vector3f xaxis(1.0, 0.0, 0.0);
		Vector3f up = Vector3f::cross(xaxis, v);
		up.normalize();
		float theta =  acos(Vector3f::dot(v, xaxis)/v.abs());
		Matrix4f rotate = Matrix4f::rotation(up, theta);
		Matrix4f trans = Matrix4f::translation(0.5 * v);// M = TRS
		m = m * trans;
		m = m * rotate;
		m = m * scale;	
		glLoadMatrixf(m);  // here we use glLoadMatrixf
		glutSolidCube(.020);	
		traverse_skeleton(stack, *itr);
	}
	stack.pop();
	
}

void SkeletalModel::drawSkeleton( )
{
	// Draw boxes between the joints. You will need to add a recursive helper function to traverse the joint hierarchy.
	traverse_skeleton(m_matrixStack, m_rootJoint);
//	glutSolidCube(.1);
}

#define PI 3.141592653
//#define deg2rad(x) (x * PI/ 180)
#define deg2rad(x) (x) //(x * PI/ 180)

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
	cout << jointIndex << " " << rX << " " << rY << " " << rZ <<endl;
	Joint* joint = m_joints[jointIndex];
	Matrix4f mX = Matrix4f::rotateX(deg2rad(rX));
	Matrix4f mY = Matrix4f::rotateY(deg2rad(rY));
	Matrix4f mZ = Matrix4f::rotateZ(deg2rad(rZ));
	
	joint->transform  = joint->transform * mZ * mY * mX; 

}


void SkeletalModel::computeBindWorldToJointTransforms()
{
	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
}

void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	//
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
}

void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
}

