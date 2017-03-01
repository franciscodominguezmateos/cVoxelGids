/*
 * grid_octree_node.h
 *
 *  Created on: Jul 4, 2016
 *      Author: Francisco Dominguez
 */

#ifndef GRID_OCTREE_NODE_H_
#define GRID_OCTREE_NODE_H_
#include <vector>

using namespace std;

template <class T>
class GridOctreeNode {
public:
	//this two attibutes could be a unions since *value is only used in leave and children in *branches
	T *value;
	//GridOctreeNode *children[8];// this could be **children to save memory
	GridOctreeNode **children;// this could be **children to save memory
//public:
	GridOctreeNode(){
		value=NULL;
		children=NULL;
		//initChildren();
	}
	void initChildren(){
		children=new GridOctreeNode*[8];
		for(int i=0;i<8;i++)
			children[i]=NULL;
	}
	~GridOctreeNode(){
		if(value!=NULL)
			delete value;
		if(children!=NULL)
			for(int i=0;i<8;i++)
				if(children[i]!=NULL)
					delete children[i];
	}
	GridOctreeNode **getChildren(){
		return children;
	}
	bool isLeaf(){
		if(children==NULL)
			return true;
		return children[0]==NULL & children[1]==NULL &
			   children[2]==NULL & children[3]==NULL &
			   children[4]==NULL & children[5]==NULL &
			   children[6]==NULL & children[7]==NULL;
	}
	vector<T*> getLeaves(){
		vector<T*> v;
		if(isLeaf()){
			v.push_back(value);
		}
		else{
			for(int i=0;i<8;i++){
				if(children[i]!=NULL){
					vector<T*> vc=children[i].getLeaves();
					v.insert(vc.begin(),vc.end());
				}
			}
		}
		return v;
	}
	//leves is from root=0 to leaf
	vector<GridOctreeNode*> getNodesAtLevel(int level){
		vector<GridOctreeNode*> v;
		if(level==0){
			v.push_back(this);
		}
		else{
			for(int i=0;i<8;i++){
				if(children[i]!=NULL){
					vector<GridOctreeNode*> vc=children[i].getNodesAtLevel(level-1);
					v.insert(vc.begin(),vc.end());
				}
			}
		}
		return v;
	}
};


#endif /* GRID_OCTREE_NODE_H_ */
