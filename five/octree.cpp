#include "Ray.h"
#include "Hit.h"
#include "Vector3f.h"
#include "Mesh.hpp"
#include "octree.hpp"
#include <vector>

///@brief two intervals intersect
bool intersect(float * a, float * b)
{
	if(a[0]>b[1]){
		float * tmp = a;
		a = b;
		b = tmp;
	}
	return b[0]<=a[1];
}

///@brief two boxes intersect
bool boxOverlap(Box * a, Box * b)
{
	for(int dim = 0 ;dim<3;dim++){
		float ia[2]={a->mn[dim], a->mx[dim]};
		float ib[2]={b->mn[dim], b->mx[dim]};
		bool inter = intersect(ia, ib);
		if(!inter){
			return false;
		}
	}
	return true;
}
bool inside(const Box & a, const Box & b){
	for(int dim = 0 ;dim<3;dim++){
		if(a.mn[dim]<b.mn[dim]
		||a.mx[dim]>b.mx[dim]){
			return false;
		}
	}
	return true;
}
///@brief bounding box for a triangle
Box trigBox(int t, const Mesh & m ){
	Box b;
	b.mn = m.v[m.t[t][0]];
	b.mx = m.v[m.t[t][0]];
	
	for(int ii = 1;ii<3;ii++){
		for(int dim = 0; dim <3 ; dim++){
			if(b.mn[dim]> m.v[m.t[t][ii]][dim]){
				b.mn[dim] = m.v[m.t[t][ii]][dim];
			}
			if(b.mx[dim]<m.v[m.t[t][ii]][dim]){
				b.mx[dim] = m.v[m.t[t][ii]][dim];
			}
		}
	}
	return b;
}

///@brief pbox parent's box
void Octree::buildNode(OctNode  & parent, const Box & pbox ,
	const std::vector<int>&trigs, 
	const Mesh & m, int level)
{
	if(trigs.size() <= Octree :: max_trig
		|| level>maxLevel){
		parent.obj = trigs;
		return;
	}
	level++;
	//initialize 8 children
	for(int ii = 0; ii<8;ii++){
		parent.child[ii]=new OctNode();
	}
	const Vector3f & mn = pbox.mn;
	const Vector3f & mx = pbox.mx;
	Vector3f mid = (mn + mx)/2;
	//childBox;
	Box cBox[8];
	//ewww....
	cBox[0] = Box(mn,mid);
	cBox[1] = Box(mn[0], mn[1],  mid[2], mid[0], mid[1], mx[2] );
	cBox[2] = Box(mn[0], mid[1], mn[2],  mid[0], mx[1],  mid[2] );
	cBox[3] = Box(mn[0], mid[1], mid[2], mid[0], mx[1],  mx[2] );
	cBox[4] = Box(mid[0], mn[1],  mn[2],  mx[0],  mid[1], mid[2] );
	cBox[5] = Box(mid[0], mn[1],  mid[2], mx[0],  mid[1], mx[2] );
	cBox[6] = Box(mid[0], mid[1], mn[2],  mx[0],  mx[1],  mid[2] );
	cBox[7] = Box(mid[0], mid[1], mid[2], mx[0],  mx[1],  mx[2] );
	//std::vector<bool>added (trigs.size(),false);
	for(int ii = 0 ; ii<8;ii++){
		std::vector<int> childTrigs;
		for(unsigned int vi =0; vi<trigs.size();vi++){
			int trigIdx = trigs[vi];
			Box tBox = trigBox(trigIdx, m);
			if(inside(tBox, cBox[ii])
				||boxOverlap(&tBox, &(cBox[ii]))
				){
				childTrigs.push_back(trigIdx);
		//		added[vi]=true;
			}
		}
		buildNode(*(parent.child[ii]),cBox[ii], childTrigs,m,level);
	}
//	for (unsigned int ii = 0 ;ii<added.size();ii++){
//		if(!added[ii]){
//			std::cout<<ii<<" not added\n";
//		}
//	}
}

void Octree::build(const Mesh & m)
{
	///compute bounding box for m
	box.mn = m.v[0];
	box.mx = m.v[0];
	for(unsigned int ii = 1 ; ii< m.v.size();ii++){
		for(int dim = 0; dim < 3; dim++){
			if(box.mn[dim]>m.v[ii][dim]){
				box.mn[dim] = m.v[ii][dim];
			}
			if(box.mx[dim]<m.v[ii][dim]){
				box.mx[dim] = m.v[ii][dim];
			}
		}
	}

	std::vector<int>trigs(m.t.size());
	for(unsigned int ii = 0 ; ii < trigs.size();ii++){
		trigs[ii] = ii;
	}
	buildNode(root,box,trigs, m,0);
}

int first_node(float tx0,float ty0,float tz0, float txm, float tym,float tzm){
char bits = 0;
///find max x0 y0 z0
if(tx0 > ty0){
    if(tx0 > tz0){ // PLANE YZ
		if(tym < tx0) {bits|=2;}
		if(tzm < tx0) {bits|=1;}
		return bits;
	}
}
else {
	if(ty0 > tz0){
		if(txm < ty0) {bits|=4;}
		if(tzm < ty0) {bits|=1;}
        return bits;
    }
}
if(txm < tz0){bits|=4;}
if(tym < tz0){bits|=2;}
return bits;
}

int new_node(float txm, int x, float tym, int y, float tzm, int z)
{
	if(txm < tym){
		if(txm < tzm){return x;}
	}
	else{
		if(tym < tzm){return y;}
	}
	return z;
}

void Octree::proc_subtree (float tx0, float ty0, float tz0, float tx1, float ty1, float tz1, OctNode* node)
{
float txm, tym, tzm;
int currNode;
if(tx1 < 0 || ty1 < 0 || tz1 < 0) {return;}
if(node->isTerm()){
	//loop over things
	for(unsigned int ii = 0 ; ii<node->obj.size();ii++){
		termFunc(node->obj[ii],arg);
	}
	return;
}
txm = 0.5*(tx0 + tx1);
tym = 0.5*(ty0 + ty1);  
tzm = 0.5*(tz0 + tz1);  
currNode = first_node(tx0,ty0,tz0,txm,tym,tzm);
do{
	switch (currNode){
	case 0: {
		proc_subtree(tx0,ty0,tz0,txm,tym,tzm,node->child[aa]);
        currNode = new_node(txm,4,tym,2,tzm,1);
        break;}
    case 1: {
        proc_subtree(tx0,ty0,tzm,txm,tym,tz1,node->child[1^aa]);
        currNode = new_node(txm,5,tym,3,tz1,8);
        break;}
    case 2: {
        proc_subtree(tx0,tym,tz0,txm,ty1,tzm,node->child[2^aa]);
        currNode = new_node(txm,6,ty1,8,tzm,3);
        break;}
    case 3: {
        proc_subtree(tx0,tym,tzm,txm,ty1,tz1,node->child[3^aa]);
        currNode = new_node(txm,7,ty1,8,tz1,8);
        break;}
    case 4: {
        proc_subtree(txm,ty0,tz0,tx1,tym,tzm,node->child[4^aa]);
        currNode = new_node(tx1,8,tym,6,tzm,5);
        break;}
    case 5: {
        proc_subtree(txm,ty0,tzm,tx1,tym,tz1,node->child[5^aa]);
        currNode = new_node(tx1,8,tym,7,tz1,8);
        break;
			}
    case 6: {
        proc_subtree(txm,tym,tz0,tx1,ty1,tzm,node->child[6^aa]);
        currNode = new_node(tx1,8,ty1,8,tzm,7);
        break;}
    case 7: {
        proc_subtree(txm,tym,tzm,tx1,ty1,tz1,node->child[7^aa]);
        currNode = 8;
        break;}
    }
} while (currNode<8);
}

void Octree::intersect(const Ray & ray){
	Vector3f rd=ray.getDirection();
	//assumes rd normalized
	rd.normalize();
	Vector3f ro=ray.getOrigin();
	aa=0;
	Vector3f size = box.mx + box.mn;
	if(rd[0]<0.0f){
		ro[0] = size[0] - ro[0];
		rd[0] = - rd[0];
		aa |= 4 ; 
	}
	if(rd[1] < 0.0f){
		ro[1] = size[1] - ro[1];
		rd[1] = - rd[1];
		aa |= 2 ;
	}
	if(rd[2] < 0.0f){
		ro[2] = size[2] - ro[2];
		rd[2] = - rd[2];
		aa |= 1 ;
	}
	
	float divx = 1 / rd[0]; // IEEE stability fix
	float divy = 1 / rd[1];
	float divz = 1 / rd[2];

	float tx0 = (box.mn[0] - ro[0]) * divx;
	float tx1 = (box.mx[0] - ro[0]) * divx;
	float ty0 = (box.mn[1] - ro[1]) * divy;
	float ty1 = (box.mx[1] - ro[1]) * divy;
	float tz0 = (box.mn[2] - ro[2]) * divz;
	float tz1 = (box.mx[2] - ro[2]) * divz;

	if( max(max(tx0,ty0),tz0) <= min(min(tx1,ty1),tz1) ){
		proc_subtree(tx0,ty0,tz0,tx1,ty1,tz1, &root);
	}
}
