/*
 * grid_octree.h
 *
 *  Created on: Jul 4, 2016
 *      Author: Francisco Dominguez
 */

#ifndef GRID_OCTREE_H_
#define GRID_OCTREE_H_

#include <vector>
#include "grid_octree_node.h"

using  namespace std;

struct Idx{
	int i,j,k;
};
template <class T>
class GridOctree {
	int sizeX,sizeY,sizeZ;
	vector<Idx>voxelsIdx;
	float minX,maxX;
	float minY,maxY;
	float minZ,maxZ;
	GridOctreeNode<T> *nodeRoot;
	int level;
public:
	GridOctree(int sizeX=128,int sizeY=128,int sizeZ=128):
		sizeX(sizeZ),sizeY(sizeY),sizeZ(sizeZ),
		voxelsIdx(vector<Idx>()),
		minX(0),maxX(1),
		minY(0),maxY(1),
		minZ(0),maxZ(1),
		nodeRoot(new GridOctreeNode<T>()),level(7){
		nodeRoot->initChildren();
	}
	~GridOctree(){
		delete nodeRoot;
	}
	inline int getChildrenPos(int i,int j,int k,int level){
		int ibit=i>>level & 1;
		int jbit=j>>level & 1;
		int kbit=k>>level & 1;
		int ret= kbit<<2 | jbit<<1 | ibit;
		return ret;
	}
	inline bool isIn(int i,int j,int k){
		if(i>=sizeX || i<0) return false;
		if(j>=sizeY || j<0) return false;
		if(k>=sizeZ || k<0) return false;
		return true;
	}
	inline bool isIn(float x,float y,float z){
		int i,j,k;
		i=getXIdx(x);
		j=getYIdx(y);
		k=getZIdx(z);
		return isIn(i,j,k);
	}
	inline bool isOut(int i,int j,int k){
		if(i>=sizeX || i<0) return true;
		if(j>=sizeY || j<0) return true;
		if(k>=sizeZ || k<0) return true;
		return false;
	}
	inline bool isOut(float x,float y,float z){
		int i,j,k;
		i=getXIdx(x);
		j=getYIdx(y);
		k=getZIdx(z);
		return isOut(i,j,k);
	}
	inline bool isEmpty(int i,int j,int k){
		if(i>=sizeX) return true;
		if(j>=sizeY) return true;
		if(k>=sizeZ) return true;
		int nPos;
		GridOctreeNode<T> *nodeRoot=this->nodeRoot;
		for(int l=level;l>=0;l--){
			nPos=getChildrenPos(i,j,k,l);
			if(nodeRoot->getChildren()[nPos]!=NULL){
				nodeRoot=nodeRoot->getChildren()[nPos];
			}
			else{
				return true;
			}
		}
		return false;
	}
	inline GridOctreeNode<T> *createNodes(GridOctreeNode<T> *nodeRoot,int i,int j,int k,int level){
		int nPos;
		GridOctreeNode<T> *node;
		for(int l=level;l>=0;l--){
			nPos=getChildrenPos(i,j,k,l);
			node=new GridOctreeNode<T>();
			if(l!=0)
				node->initChildren();
			nodeRoot->getChildren()[nPos]=node;
			nodeRoot=node;
		}
		return nodeRoot;
	}
	inline void insertNode(int i,int j,int k,T *value){
		int nPos;
		GridOctreeNode<T> *nodeRoot=this->nodeRoot;
		for(int l=level;l>=0;l--){
			nPos=getChildrenPos(i,j,k,l);
			if(nodeRoot->getChildren()[nPos]!=NULL){
				nodeRoot=nodeRoot->getChildren()[nPos];
			}
			else{
				nodeRoot=createNodes(nodeRoot,i,j,k,l);
				break;
			}
		}
		nodeRoot->value=value;
	}
	vector<GridOctreeNode<T>*> getPath(int i,int j,int k){
		//Make sure voxel i,j,k exist before call this method
		int nPos;
		GridOctreeNode<T> *nodeRoot=this->nodeRoot;
		vector<GridOctreeNode<T>*> nodes;
		nodes.push_back(nodeRoot);
		for(int l=level;l>=0;l--){
			nPos=getChildrenPos(i,j,k,l);
			nodeRoot=nodeRoot->getChildren()[nPos];
			nodes.push_back(nodeRoot);
		}
		//nodes.reserve();
		return nodes;
	}
	inline void deleteNode(int i,int j,int k){/*
		if(isOut(i,j,k)) return;
		int nPos;
		GridOctreeNode<T> *pn=NULL;
		for(int i=0;i<=level;i++){
			pn=nodes[0];
		}*/
	}
	//NULL if the voxel doesn't exist
	inline T *getVoxelPtr(int i,int j,int k){
		if(isOut(i,j,k)) return NULL;
		int nPos;
		GridOctreeNode<T> *nodeRoot=this->nodeRoot;
		for(int l=level;l>=0;l--){
			nPos=getChildrenPos(i,j,k,l);
			if(nodeRoot->getChildren()[nPos]!=NULL){
				nodeRoot=nodeRoot->getChildren()[nPos];
			}
			else{
				return NULL;
			}
		}
		return nodeRoot->value;
	}
	inline T *getVoxelPtr(Idx idx){
		return getVoxelPtr(idx.i,idx.j,idx.k);
	}
	inline T *getVoxelPtr(float x,float y,float z){
		int i,j,k;
		i=getXIdx(x);
		j=getYIdx(y);
		k=getZIdx(z);
		return getVoxelPtr(i,j,k);
	}
	inline T getVoxel(int i,int j,int k){
		T empty;
		T *pv=getVoxelPtr(i,j,k);
		if(pv==NULL) return empty;
		return *pv;
	}
	inline T getVoxel(float x,float y,float z){
		int i,j,k;
		i=getXIdx(x);
		j=getYIdx(y);
		k=getZIdx(z);
		return getVoxel(i,j,k);
	}
	inline void setVoxel(int i,int j,int k,T v){
		if(isOut(i,j,k)) return;
		//if doesn't exit node
		if(getVoxelPtr(i,j,k)==NULL){
			Idx idx=getIdx(i,j,k);
			voxelsIdx.push_back(idx);
		}
		T *p=new T(v);//T need a copy constructor
		insertNode(i,j,k,p);
	}
	inline void setVoxel(float x,float y,float z,T v){
		int i,j,k;
		i=getXIdx(x);
		j=getYIdx(y);
		k=getZIdx(z);
		setVoxel(i,j,k,v);
	}
	inline int getXIdx(float f){
		int i;
		float d=maxX-minX;
		float j;
		j=(f-minX)/d;
		//i=fmax(fmin(j,1.0),0.0)*(size-1);
		i=j*(sizeX-1);
		return i;
	}
	inline int getYIdx(float f){
		int i;
		float d=maxY-minY;
		float j;
		j=(f-minY)/d;
		//i=fmax(fmin(j,1.0),0.0)*(size-1);
		i=j*(sizeY-1);
		return i;
	}
	inline int getZIdx(float f){
		int i;
		float d=maxZ-minZ;
		float j;
		j=(f-minZ)/d;
		//i=fmax(fmin(j,1.0),0.0)*(size-1);
		i=j*(sizeZ-1);
		return i;
	}
	inline void XYZ2ijk(float x,float y,float z,int &i,int &j,int &k){
		i=getXIdx(x);
		j=getYIdx(y);
		k=getZIdx(z);
	}
	//return the cx,cy,cz of the voxel where x,y,z fall in
	inline void XYZ2center(float &x,float &y,float &z,float &cx,float &cy,float &cz){
		int i,j,k;
		XYZ2ijk(x,y,z,i,j,k);
		ijk2XYZ(i,j,k,cx,cy,cz);
	}
	inline Idx getIdx(int &i,int &j,int &k){
		Idx idx={i,j,k};
		return idx;
	}
	inline void getIJKfromIdx(Idx idx,int &i, int &j,int &k){
		i=idx.i;
		j=idx.j;
		k=idx.k;
	}
	inline void getXYZfromIdx(Idx idx,float &x,float &y,float &z){
		int &i=idx.i;
		int &j=idx.j;
		int &k=idx.k;
		ijk2XYZ(i,j,k,x,y,z);
	}
	inline float i2X(int i){
		float j=(float)i/(sizeX-1);
		float d=maxX-minX;
		return minX+d*j+voxelSizeX()/2;
	}
	inline float j2Y(int i){
		float j=(float)i/(sizeY-1);
		float d=maxY-minY;
		return minY+d*j+voxelSizeY()/2;
	}
	inline float k2Z(int i){
		float j=(float)i/(sizeZ-1);
		float d=maxZ-minZ;
		return minZ+d*j+voxelSizeZ()/2;
	}
	inline void ijk2XYZ(int i,int j,int k,float &x,float &y,float &z){
		x=i2X(i);
		y=j2Y(j);
		z=k2Z(k);
	}
	inline float voxelSizeX(){
		float d=maxX-minX;
		return d/sizeX;
	}
	inline float voxelSizeY(){
		float d=maxY-minY;
		return d/sizeY;
	}
	inline float voxelSizeZ(){
		float d=maxZ-minZ;
		return d/sizeZ;
	}
	inline void setMinMax(float minX,float maxX,float minY,float maxY,float minZ,float maxZ){
		this->minX=minX;
		this->maxX=maxX;
		this->minY=minY;
		this->maxY=maxY;
		this->minZ=minZ;
		this->maxZ=maxZ;
	}
	inline int getSizeX(){return sizeX;}
	inline int getSizeY(){return sizeY;}
	inline int getSizeZ(){return sizeX;}
	inline vector<Idx> &getVoxelsIdx(){
		return voxelsIdx;
	}
	inline void clear(T zeros){
	}
	inline void setLevel(int level){this->level=level;}
};




#endif /* GRID_OCTREE_H_ */
