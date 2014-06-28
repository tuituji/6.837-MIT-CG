#ifndef OCTREE_HPP
#define OCTREE_HPP
struct Box
{
	Vector3f mn, mx;
	Box(){}
	Box(const Vector3f & a, const Vector3f & b):
	mn(a),mx(b)
	{}
	Box(float mnx, float mny,float mnz,
		float mxx,float mxy,float mxz):
	mn(Vector3f(mnx,mny,mnz)),
		mx(Vector3f(mxx,mxy,mxz))
	{}
};

struct OctNode
{
	OctNode * child[8];
	OctNode(){
		child[0] = 0;
	}
	///@brief is this terminal
	bool isTerm(){return child[0]==0;}
	std::vector<int> obj;
};
class Mesh;
struct Octree
{
	//if a node contains more than 7 triangles and it 
	//hasn't reached the max level yet, 
	///split
	static const int max_trig = 7;
	int maxLevel;
	OctNode root;
	Octree(int level = 8):
	maxLevel(level){
	}
	Box box;
	void build(const Mesh & m);
	void buildNode(OctNode  & parent, const Box & pbox ,
		const std::vector<int>&trigs, 
		const Mesh & m, int level);
	
	///@brief indexing
	unsigned char aa;
	void proc_subtree (float tx0, float ty0, float tz0, float tx1, float ty1, float tz1, OctNode* node);
	void intersect(const Ray & ray);
	
	void ** arg;
	void (*termFunc) (int idx, void ** arg);
};
Octree buildOctree(const Mesh & m, int maxLevel=7);
#endif