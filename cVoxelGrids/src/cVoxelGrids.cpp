//============================================================================
// Name        : cVoxelGrids.cpp
// Author      : Francisco Dominguez
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include "grid_octree.h"

using namespace std;

int main() {
	GridOctree<float> go(4,4,4);
	go.setLevel(2);
	go.setVoxel(0,0,0,0);
	go.setVoxel(1,0,0,1);
	go.setVoxel(0,1,0,10);
	go.setVoxel(0b10,0b10,0b10,222);
	go.setVoxel(0b11,0b01,0b11,222);
	cout << go.getVoxel(0,0,0)<<endl;
	cout << go.getVoxel(1,0,0)<<endl;
	cout << go.getVoxel(0,1,0)<<endl;
	cout << go.getVoxel(0b10,0b10,0b10)<<endl;
	cout << go.getVoxel(0b11,0b01,0b11)<<endl;
	vector<GridOctreeNode<float>*> vn=go.getPath(0b0011,0b1001,0b0011);
	for(GridOctreeNode<float> *n:vn){
		for(int i=7;i>=0;i--){
			if(n->getChildren()[i]==NULL)
				cout<<0;
			else
				cout<<1;
		}
		cout <<endl;
	}
	return 0;
}
