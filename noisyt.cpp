#include"viewer.hpp"
#include"mesh.hpp"
#include<vector>
#include<bits/stdc++.h>
#include<set>
#include<string>
#include<iostream>
#include<fstream>
using namespace std;

namespace V = COL781::Viewer;
using namespace glm;

int main() {
	V::Viewer v;
	if (!v.initialize("Mesh viewer", 640, 640)) 
	{
		return EXIT_FAILURE;
	}
	Mesh obj=objectToMesh("../meshes/noisycube.obj");
	obj.smooth(10,0.33,-0.34);
	obj.rasterMesh();
	v.setVertices(obj.vertices.size(), obj.rvertices);
	v.setNormals(obj.vertices.size(), obj.rnormals);
	v.setTriangles(obj.triangles.size(),obj.rtriangles);
	v.view();
}
