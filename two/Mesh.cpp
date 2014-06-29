#include "Mesh.h"

using namespace std;

void Mesh::load( const char* filename )
{
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces
	const unsigned int  MAX_SIZE = 100;
	char buffer[MAX_SIZE];
	ifstream in(filename);
	if(!in) {
		cerr<< "error open file" <<endl;
		return;
	}
	while(in.getline(buffer, MAX_SIZE)) {
		// cout << buffer << endl;		
		stringstream ss(buffer);
		string s;
		ss >> s;
		if(s == "v") {
			Vector3f v;
			ss >> v[0] >> v[1] >> v[2];
			currentVertices.push_back(v);
		}
		else if(s == "f") {
			Tuple3u face;		
			ss >> face[0] >> face[1] >> face[2];
			faces.push_back(face);
		}
		else {
			cerr << "error occured\n" <<endl;
		}
	}
	// Add your code here.

	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;
}

void Mesh::draw()
{

	// Since these meshes don't have normals
	// be sure to generate a normal per triangle.
	// Notice that since we have per-triangle normals
	// rather than the analytical normals from
	// assignment 1, the appearance is "faceted".
	std::vector<Tuple3u>::iterator itr;
	std::vector< Vector3f > &verts = currentVertices;
	Vector3f normal;
	glBegin(GL_TRIANGLES);
		for(itr = faces.begin(); itr != faces.end(); ++itr) {
			normal = Vector3f::cross(verts[(*itr)[1]-1] - verts[(*itr)[0]-1],
						verts[(*itr)[2]-1] - verts[(*itr)[0]-1]);
			normal.normalize();
			glNormal3f(normal[0], normal[1], normal[2]);
//			glColor3f( 1, 0, 1 );
			glVertex3f(verts[(*itr)[0]-1][0], verts[(*itr)[0]-1][1],
						verts[(*itr)[0]-1][2]);
			glNormal3f(normal[0], normal[1], normal[2]);
//			glColor3f( 1, 1, 0 );
			glVertex3f(verts[(*itr)[1]-1][0], verts[(*itr)[1]-1][1],
						verts[(*itr)[1]-1][2]);
			glNormal3f(normal[0], normal[1], normal[2]);
//			glColor3f( 1, 0, 0 );
			glVertex3f(verts[(*itr)[2]-1][0], verts[(*itr)[2]-1][1],
						verts[(*itr)[2]-1][2]);
		}
	glEnd();
}

void Mesh::loadAttachments( const char* filename, int numJoints )
{
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments
	string str;
	float w;
	ifstream in(filename);
	if(!in) {
		cerr << "error open file" <<endl;
		return;
	}
	while(getline(in, str)) {
		stringstream ss(str);
		std::vector< float > weight;
		weight.push_back(0.0);// the root ?? need ??
		while(ss >> w){
			weight.push_back(w);
		}
		attachments.push_back(weight);
	}
	return;
}
