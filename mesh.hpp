#include<vector>
#include <glm/glm.hpp>
#include<bits/stdc++.h>
#include<set>
#include<string>
#include<iostream>
#include<fstream>
using namespace std;
using namespace glm;

//Store data of ONE vertex
struct Vertex
{
int id;
vec3 position;
vec3 normal;
set<int> neighbours;
};

//Store data of ONE triangle
struct Triangle
{
int id;
ivec3 vertices;
ivec3 neighbours;
};


class Mesh{
public:

//Stores vertex IDs of all the vertices of the Mesh
vector<Vertex> vertices;
//Stroes triangle IDs of all the triangles of the Mesh
vector<Triangle> triangles;

//For rasterization
vec3 *rvertices;
vec3 *rnormals;
ivec3 *rtriangles;


//Return index of the vertex with given id
int findVertex(int id)
{
for(int i=0;i<vertices.size();i++){
   if(vertices[i].id==id){
      return i;
   }
}
return -1;
}

//Return index of the triangle with given id
int findTriangle(int id)
{
for(int i=0;i<triangles.size();i++){
   if(triangles[i].id==id){
      return i;
   }
}
return -1;
}

void loopSubdivision()
{
struct Edge{
int f;
int t;
int id;
};
vector<Triangle> TR;
vector<Vertex> VR;
vector<Edge> edges;

bool vflags[vertices.size()+1]={false};
int vc=vertices.size();
int tc=0;

for(int j=0;j<triangles.size();j++)
{
Vertex v1,v2,v3;
Triangle t1,t2,t3,t4;
t1.id=++tc;
t2.id=++tc;
t3.id=++tc;
t4.id=++tc;

int V1_index=findVertex(triangles[j].vertices[0]);
int V2_index=findVertex(triangles[j].vertices[1]);
int V3_index=findVertex(triangles[j].vertices[2]);

Vertex V1=vertices[V1_index];
Vertex V2=vertices[V2_index];
Vertex V3=vertices[V3_index];

int f1=-1,f2=-1,f3=-1;

for(int e=0;e<edges.size();e++)
{
if((edges[e].f==V1.id||edges[e].t==V1.id)&&(edges[e].f==V2.id||edges[e].t==V2.id)){
   f1=edges[e].id;
}
if((edges[e].f==V2.id||edges[e].t==V2.id)&&(edges[e].f==V3.id||edges[e].t==V3.id)){
   f2=edges[e].id;
}
if((edges[e].f==V1.id||edges[e].t==V1.id)&&(edges[e].f==V3.id||edges[e].t==V3.id)){
   f3=edges[e].id;
}

}

if(f1==-1)
{
v1.id=++vc;
f1=v1.id;
if(triangles[j].neighbours[0]==-1)
{
v1.position=vec3(0.5*(V1.position[0]+V2.position[0]),0.5*(V1.position[1]+V2.position[1]),0.5*(V1.position[2]+V2.position[2]));
v1.normal=vec3(0.5*(V1.normal[0]+V2.normal[0]),0.5*(V1.normal[1]+V2.normal[1]),0.5*(V1.normal[2]+V2.normal[2]));
}
else{
int nt=findTriangle(triangles[j].neighbours[0]);
int id_new=triangles[nt].vertices[0]+triangles[nt].vertices[1]+triangles[nt].vertices[2]-(V1.id+V2.id);
int temp=findVertex(id_new);
v1.position=vec3(
   0.375*(V1.position[0]+V2.position[0])+0.125*(V3.position[0]+vertices[temp].position[0]),
   0.375*(V1.position[1]+V2.position[1])+0.125*(V3.position[1]+vertices[temp].position[1]),
   0.375*(V1.position[2]+V2.position[2])+0.125*(V3.position[2]+vertices[temp].position[2]));
v1.normal=vec3(
   0.375*(V1.normal[0]+V2.normal[0])+0.125*(V3.normal[0]+vertices[temp].normal[0]),
   0.375*(V1.normal[1]+V2.normal[1])+0.125*(V3.normal[1]+vertices[temp].normal[1]),
   0.375*(V1.normal[2]+V2.normal[2])+0.125*(V3.normal[2]+vertices[temp].normal[2]));

}
struct Edge ed;
ed.id=f1;
ed.f=V1.id;
ed.t=V2.id;
edges.push_back(ed);
VR.push_back(v1);

}


if(f2==-1){
   v2.id=++vc;
   f2=v2.id;
if(triangles[j].neighbours[1]==-1)
{
v2.position=vec3(0.5*(V2.position[0]+V3.position[0]),0.5*(V2.position[1]+V3.position[1]),0.5*(V2.position[2]+V3.position[2]));
v2.normal=vec3(0.5*(V2.normal[0]+V3.normal[0]),0.5*(V2.normal[1]+V3.normal[1]),0.5*(V2.normal[2]+V3.normal[2]));
}
else{

int nt=findTriangle(triangles[j].neighbours[1]);
int id_new=triangles[nt].vertices[0]+triangles[nt].vertices[1]+triangles[nt].vertices[2]-(V2.id+V3.id);
int temp=findVertex(id_new);
v2.position=vec3(
   0.375*(V2.position[0]+V3.position[0])+0.125*(V1.position[0]+vertices[temp].position[0]),
   0.375*(V2.position[1]+V3.position[1])+0.125*(V1.position[1]+vertices[temp].position[1]),
   0.375*(V2.position[2]+V3.position[2])+0.125*(V1.position[2]+vertices[temp].position[2]));

v2.normal=vec3(
   0.375*(V2.normal[0]+V3.normal[0])+0.125*(V1.normal[0]+vertices[temp].normal[0]),
   0.375*(V2.normal[1]+V3.normal[1])+0.125*(V1.normal[1]+vertices[temp].normal[1]),
   0.375*(V2.normal[2]+V3.normal[2])+0.125*(V1.normal[2]+vertices[temp].normal[2]));
}

struct Edge ed;
ed.id=f2;
ed.f=V2.id;
ed.t=V3.id;
edges.push_back(ed);
VR.push_back(v2);

}
if(f3==-1){
v3.id=++vc;
f3=v3.id;
if(triangles[j].neighbours[2]==-1)
{
v3.position=vec3(0.5*(V1.position[0]+V3.position[0]),0.5*(V1.position[1]+V3.position[1]),0.5*(V1.position[2]+V3.position[2]));
v3.normal=vec3(0.5*(V1.normal[0]+V3.normal[0]),0.5*(V1.normal[1]+V3.normal[1]),0.5*(V1.normal[2]+V3.normal[2]));
}
else{
   int nt=findTriangle(triangles[j].neighbours[2]);
int id_new=triangles[nt].vertices[0]+triangles[nt].vertices[1]+triangles[nt].vertices[2]-(V1.id+V3.id);
int temp=findVertex(id_new);
v3.position=vec3(
   0.375*(V1.position[0]+V3.position[0])+0.125*(V2.position[0]+vertices[temp].position[0]),
   0.375*(V1.position[1]+V3.position[1])+0.125*(V2.position[1]+vertices[temp].position[1]),
   0.375*(V1.position[2]+V3.position[2])+0.125*(V2.position[2]+vertices[temp].position[2]));

v3.normal=vec3(
   0.375*(V1.normal[0]+V3.normal[0])+0.125*(V2.normal[0]+vertices[temp].normal[0]),
   0.375*(V1.normal[1]+V3.normal[1])+0.125*(V2.normal[1]+vertices[temp].normal[1]),
   0.375*(V1.normal[2]+V3.normal[2])+0.125*(V2.normal[2]+vertices[temp].normal[2]));
}
struct Edge ed;
ed.id=f3;
ed.f=V1.id;
ed.t=V3.id;
edges.push_back(ed);
VR.push_back(v3);

}


float beta1=3.0/(8.0*V1.neighbours.size());
float beta2=3.0/(8.0*V2.neighbours.size());
float beta3=3.0/(8.0*V3.neighbours.size());

if(V1.neighbours.size()==3)
{
beta1=0.1875;
}
if(V2.neighbours.size()==3)
{
beta2=0.1875;
}
if(V3.neighbours.size()==3)
{
beta3=0.1875;
}

float x=0;
float y=0;
float z=0;

if(vflags[V1.id]==false)
{
for(auto k=V1.neighbours.begin();k!=V1.neighbours.end();k++)
{
int index=findVertex(*k);
x=x+vertices[index].position[0];
y=y+vertices[index].position[1];
z=z+vertices[index].position[2];
}

V1.position=vec3(
V1.position[0]*(1-V1.neighbours.size()*beta1)+x*beta1,
V1.position[1]*(1-V1.neighbours.size()*beta1)+y*beta1,
V1.position[2]*(1-V1.neighbours.size()*beta1)+z*beta1
);
V1.normal=vec3(
V1.normal[0]*(1-V1.neighbours.size()*beta1)+x*beta1,
V1.normal[1]*(1-V1.neighbours.size()*beta1)+y*beta1,
V1.normal[2]*(1-V1.neighbours.size()*beta1)+z*beta1
);
vflags[V1.id]=true;
VR.push_back(V1);
}

if(vflags[V2.id]==false){
x=0;
y=0;
z=0;
for(auto k=V2.neighbours.begin();k!=V2.neighbours.end();k++)
{
int index=findVertex(*k);
x=x+vertices[index].position[0];
y=y+vertices[index].position[1];
z=z+vertices[index].position[2];
}
V2.position=vec3(
V2.position[0]*(1-V2.neighbours.size()*beta2)+x*beta2,
V2.position[1]*(1-V2.neighbours.size()*beta2)+y*beta2,
V2.position[2]*(1-V2.neighbours.size()*beta2)+z*beta2
);
V2.normal=vec3(
V2.normal[0]*(1-V2.neighbours.size()*beta1)+x*beta1,
V2.normal[1]*(1-V2.neighbours.size()*beta1)+y*beta1,
V2.normal[2]*(1-V2.neighbours.size()*beta1)+z*beta1
);
vflags[V2.id]=true;
VR.push_back(V2);
}


if(vflags[V3.id]==false){
x=0;
y=0;
z=0;
for(auto k=V3.neighbours.begin();k!=V3.neighbours.end();k++)
{
int index=findVertex(*k);
x=x+vertices[index].position[0];
y=y+vertices[index].position[1];
z=z+vertices[index].position[2];
}

V3.position=vec3(
V3.position[0]*(1-V3.neighbours.size()*beta3)+x*beta3,
V3.position[1]*(1-V3.neighbours.size()*beta3)+y*beta3,
V3.position[2]*(1-V3.neighbours.size()*beta3)+z*beta3
);
V3.normal=vec3(
V3.normal[0]*(1-V3.neighbours.size()*beta1)+x*beta1,
V3.normal[1]*(1-V3.neighbours.size()*beta1)+y*beta1,
V3.normal[2]*(1-V3.neighbours.size()*beta1)+z*beta1
);
vflags[V3.id]=true;
VR.push_back(V3);
}

t2.vertices=ivec3(f1,V1.id,f3);
t3.vertices=ivec3(V2.id,f1,f2);
t4.vertices=ivec3(f2,f3,V3.id);
t1.vertices=ivec3(f1,f2,f3);
TR.push_back(t1);
TR.push_back(t2);
TR.push_back(t3);
TR.push_back(t4);

}

triangles.clear();
vertices.clear();
triangles=TR;
vertices=VR;
setNeighbours();

}

void normall()
{
   for(int i=0;i<triangles.size();i++)
   {
      int v1=triangles[i].vertices[0];
      int v2=triangles[i].vertices[1];
      int v3=triangles[i].vertices[2];
      int index1=findVertex(v1);
      int index2=findVertex(v2);
      int index3=findVertex(v3);
      vec3 v4=vertices[index1].position;
      vec3 v5=vertices[index2].position;
      vec3 v6=vertices[index3].position;
      vec3 v7=v4-v5;
      vec3 v8=v4-v6;
      vec3 normal1=cross(v7,v8);
      vertices[index1].normal+=normal1;
      vertices[index2].normal+=normal1;
      vertices[index3].normal+=normal1;
   }
   for(int i=0;i<vertices.size();i++)
   {
      vertices[i].normal=normalize(vertices[i].normal);
   }
}


//Calling this method sets the neighbours of all the vertices and triangles.
void setNeighbours()
{

for(int i=0;i<vertices.size();i++)
{
vertices[i].neighbours.clear();
}

for(int i=0;i<triangles.size();i++)
{
triangles[i].neighbours[0]=-1;
triangles[i].neighbours[1]=-1;
triangles[i].neighbours[2]=-1;
}


 for(int i=0;i<triangles.size();i++)
 {
   Triangle t=triangles[i];
   int v1=t.vertices[0];
   int v2=t.vertices[1];
   int v3=t.vertices[2];

   int index1=findVertex(v1);
   int index2=findVertex(v2);
   int index3=findVertex(v3);

   vertices[index1].neighbours.insert(v2);
   vertices[index1].neighbours.insert(v3);
   vertices[index3].neighbours.insert(v1);
   vertices[index3].neighbours.insert(v2);
   vertices[index2].neighbours.insert(v1);
   vertices[index2].neighbours.insert(v3);


   for(int j=0;j<triangles.size();j++)
   {
   if(j!=i)
   {
   int iv1=triangles[j].vertices[0];
   int iv2=triangles[j].vertices[1];
   int iv3=triangles[j].vertices[2];
   if((v1==iv1||v1==iv2||v1==iv3)&&(v2==iv1||v2==iv2||v2==iv3))
   {
   triangles[i].neighbours[0]=triangles[j].id;
   }
   else if((v2==iv1||v2==iv2||v2==iv3)&&(v3==iv1||v3==iv2||v3==iv3)){
   triangles[i].neighbours[1]=triangles[j].id;
   }
   else if((v1==iv1||v1==iv2||v1==iv3)&&(v3==iv1||v3==iv2||v3==iv3)){
   triangles[i].neighbours[2]=triangles[j].id;
   }   
   }

   }
 }

}

//Implementation of Gabriel Taubin
void smooth(int itr,float l,float m)
{

vector<Vertex> svertices;
for(int i=0;i<vertices.size();i++)
{
svertices.push_back(vertices[i]);
}

for(int i=0;i<itr;i++)
{
for(int j=0;j<vertices.size();j++)
{
float posx=0;
float posy=0;
float posz=0;

for(auto k=svertices[j].neighbours.begin();k!=svertices[j].neighbours.end();k++)
{
int index=findVertex(*k);
posx=posx+svertices[index].position[0]-svertices[j].position[0];
posy=posy+svertices[index].position[1]-svertices[j].position[1];
posz=posz+svertices[index].position[2]-svertices[j].position[2];
}

posx=posx/svertices[j].neighbours.size();
posy=posy/svertices[j].neighbours.size();
posz=posz/svertices[j].neighbours.size();

float newx=vertices[j].position[0]+l*posx;
float newy=vertices[j].position[1]+l*posy;
float newz=vertices[j].position[2]+l*posz;
vertices[j].position[0]=newx;
vertices[j].position[1]=newy;
vertices[j].position[2]=newz;


posx=0;
posy=0;
posz=0;

for(auto k=svertices[j].neighbours.begin();k!=svertices[j].neighbours.end();k++)
{
int index=findVertex(*k);
posx=posx+svertices[index].position[0]-vertices[j].position[0];
posy=posy+svertices[index].position[1]-vertices[j].position[1];
posz=posz+svertices[index].position[2]-vertices[j].position[2];
}

posx=posx/svertices[j].neighbours.size();
posy=posy/svertices[j].neighbours.size();
posz=posz/svertices[j].neighbours.size();

newx=vertices[j].position[0]+m*posx;
newy=vertices[j].position[1]+m*posy;
newz=vertices[j].position[2]+m*posz;

vertices[j].position[0]=newx;
vertices[j].position[1]=newy;
vertices[j].position[2]=newz;


}
svertices.clear();
for(int i=0;i<vertices.size();i++)
{
svertices.push_back(vertices[i]);
}

}

}


//This methods sets the rvertices, rnormals and rtraingles that are sent to Rasterization API
void rasterMesh()
{
fstream ofile;
ofile.open("mesh.obj",ios::out);

rvertices=(vec3*) malloc(sizeof(vec3)*vertices.size());
rnormals=(vec3*) malloc(sizeof(vec3)*vertices.size());
rtriangles=(ivec3*) malloc(sizeof(vec3)*triangles.size());

for(int i=1;i<=vertices.size();i++)
{
int index=findVertex(i);
ofile<<"v "<<vertices[index].position[0]<<" "<<vertices[index].position[2]<<" "<<vertices[index].position[2]<<endl;
rvertices[i-1]=vec3(vertices[index].position[0],vertices[index].position[1],vertices[index].position[2]);
rnormals[i-1]=vec3(vertices[index].normal[0],vertices[index].normal[1],vertices[index].normal[2]);
}

for(int j=1;j<=triangles.size();j++)
{
int index=findTriangle(j);
ofile<<"f "<<triangles[index].vertices[0]<<" "<<triangles[index].vertices[1]<<" "<<triangles[index].vertices[2]<<endl;
rtriangles[j-1]=ivec3(triangles[index].vertices[0]-1,triangles[index].vertices[1]-1,triangles[index].vertices[2]-1);
}
}

};


// Returns the Mesh object given the path of the mesh .obj file
Mesh objectToMesh(string name)
{
Mesh mesh;

fstream ifile;
ifile.open(name,ios::in);
string line;
vector<vec3> normals;
int verc=1;
int trc=1;
while(getline(ifile,line))
{

if(line.length()==0||line=="\n"||line=="")
 {
  continue;
 }
stringstream line1(line);
string query;
vector<string> myline;
while(getline(line1,query,' '))
{
myline.push_back(query);
}

if(myline[0]=="vn")
{
normals.push_back(vec3(stof(myline[1]),stof(myline[2]),stof(myline[3])));
}
else if(myline[0]=="v")
{
struct Vertex vertex;
vertex.id=verc;
++verc;
vertex.normal=vec3(0.0,0.0,0.0);
vertex.position=vec3(stof(myline[1]),stof(myline[2]),stof(myline[3]));
mesh.vertices.push_back(vertex);
}
else if(myline[0]=="f")
{

struct Triangle tr;
tr.id=trc;
tr.vertices=ivec3(0,0,0);
trc=trc+1;
string tokens[4];

int index=0;
string ok="";
int i=0;

while(true)
{
if(line[i]==' '||line[i]=='\n')
{
tokens[index]=ok;
ok="";
++index;
}
else{
   ok=ok+line[i];
}
++i;
if(index==4){
   break;
}
}


for(int op=1;op<4;op++)
{
string vs[3]={"","",""};
ok="";
index=0;
i=0;
while(true)
{
if(tokens[op][i]=='/')
{
vs[index]=ok;
ok="";
++index;
}
else if(tokens[op][i]=='\0'){
   vs[index]=ok;
   break;
}
else{
   ok=ok+tokens[op][i];
}
++i;

}
int vertid=atoi(vs[0].c_str());
tr.vertices[op-1]=vertid;
if(vs[2].length()>0){
  mesh.vertices[mesh.findVertex(vertid)].normal=normals[atoi(vs[2].c_str())-1]; 
}
}
mesh.triangles.push_back(tr);
}


else{ continue;}


}
mesh.setNeighbours();
mesh.normall();
return mesh;

//end
}


//Function for Square Mesh
Mesh squareMesh(float dim,int m,int n)
{
Mesh mesh;
int vc=1;
int tc=1;
float cstart=-1*dim;
float rstart=dim;
float rowwise=(dim*2.0)/m;
float colwise=(dim*2.0)/n;

for(int i=0;i<m+1;i++)
{
for(int j=0;j<n+1;j++)
{
Vertex v1;
v1.id=vc;
v1.normal=vec3(0.0,0.0,0.0);
v1.position=vec3(cstart+j*colwise,rstart-rowwise*i,0.0);
++vc;
mesh.vertices.push_back(v1);
}
}


for(int i=0;i<m;i++)
{
for(int j=1;j<=n;j++)
{
Triangle t1;
Triangle t2;
t1.id=tc;
++tc;
t2.id=tc;
++tc;
int v1=(n+1)*i+j;
int v2=(n+1)*i+j+1;
int v3=(n+1)*(i+1)+j;
int v4=(n+1)*(i+1)+j+1;
t1.vertices=ivec3(v1,v2,v3);
t2.vertices=ivec3(v2,v3,v4);
mesh.triangles.push_back(t1);
mesh.triangles.push_back(t2);
}
}
mesh.normall();
mesh.setNeighbours();
return mesh;

}

//Function for Spherical Mesh
Mesh sphereMesh(int slices,int stacks){
float w=2.0;
Mesh mesh;
int vc=1;
int tc=1;
Vertex v1;
v1.id=vc;
v1.position=vec3(0.0,1.0/w,0.0);
v1.normal=vec3(0,0,0);
mesh.vertices.push_back(v1);
++vc;

for (int i=0;i<stacks-1;i++)
{
float phi = M_PI*double(i+1)/double(stacks);
for (int j=0;j<slices;j++)
{
    float theta=(2.0*M_PI*double(j))/double(slices);
    float x=sin(phi)*cos(theta)/w;
    float y=sin(phi)*sin(theta)/w;
    float z=cos(phi)/w;
    Vertex v;
    v.id=vc;
    ++vc;
    v.position=vec3(x,y,z);
    v.normal=vec3(0,0,0);
    mesh.vertices.push_back(v);
}
}

Vertex v2;
v2.id=vc;
v2.position=vec3(0.0,-1.0/w,0.0);
v2.normal=vec3(0,0,0);
mesh.vertices.push_back(v2);

int V1,V2;
for (int i=1;i<=slices;++i)
{
V1=i+1;
V2=(i+1)%slices+1;
Triangle t1,t2;
t1.id=tc;
++tc;
t2.id=tc;
++tc;
t1.vertices=ivec3(1,V2,V1);
V1=i+slices*(stacks-2)+1;
V2=(i+1)%slices+slices*(stacks-2)+1;
t2.vertices=ivec3(vc,V1,V2);
mesh.triangles.push_back(t1);
mesh.triangles.push_back(t2);
}
for(int j=0;j<stacks-2;j++){
int start1=1+j*(slices)+1;
int start2=1+(j+1)*slices+1;
int end=slices-1;
for(int k=0;k<end;k++)
{
int v1=start1+k;
int v2=start2+k;
int v3=start1+k+1;
int v4=start2+k+1;
Triangle t1,t2;
t1.id=tc;
++tc;
t2.id=tc;
++tc;
t1.vertices=ivec3(v1,v2,v3);
t2.vertices=ivec3(v2,v3,v4);
mesh.triangles.push_back(t1);
mesh.triangles.push_back(t2);
}
Triangle t1,t2;
t1.id=tc;
++tc;
t2.id=tc;
++tc;
t1.vertices=ivec3(start1,start2,start1+end);
t2.vertices=ivec3(start2,start2+end,start1+end);
mesh.triangles.push_back(t1);
mesh.triangles.push_back(t2);
}
mesh.normall();
mesh.setNeighbours();
return mesh;
}