
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Mesh.h"
#include "tools/include/vsShaderLib.h"
#include "tools/include/vsMathLib.h"
#include <cmath>
#include <iostream>
#define PI 3.141592653f
using namespace std;

void Mesh::init(GLfloat r)
{
    setData(r);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffers[4];
    glGenBuffers(4, buffers);

    //Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticesSize, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(VSShaderLib::VERTEX_COORD_ATTRIB);
    glVertexAttribPointer(VSShaderLib::VERTEX_COORD_ATTRIB, vertexOffset, GL_FLOAT, 0, 0, 0);

    //texture coordinate buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * texCoordsSize, texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(VSShaderLib::VERTEX_COORD_ATTRIB);
    glVertexAttribPointer(VSShaderLib::TEXTURE_COORD_ATTRIB, texOffset, GL_FLOAT, 0, 0, 0);

    //Normals buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normalsSize, normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(VSShaderLib::NORMAL_ATTRIB);
    glVertexAttribPointer(VSShaderLib::NORMAL_ATTRIB, normalOffset, GL_FLOAT, 0, 0, 0);

    //Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * faceIndexSize, faceIndex, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

//Initialize data
//
void Mesh::setData(GLfloat ra)
{

    //Load a sphere according to its stacks and slices
    GLfloat r = ra;
    GLint stacks = 200;
    GLint slices = 200;

    vertexOffset = 4;
    texOffset = 2;
    normalOffset = 3;

    verticesSize = 8 + 4 * (stacks-1) * slices;
    texCoordsSize = 4 + 2 * (stacks-1) * slices;
    normalsSize = 6 + 3 * (stacks - 1) * slices;
    faceIndexSize = 6 * slices * ( stacks - 1);

    vertices = new GLfloat[verticesSize];
    texCoords = new GLfloat[texCoordsSize];
    normals = new GLfloat[normalsSize];
    faceIndex = new GLint[faceIndexSize];


    //two pole of my sphere
	vertices[0] = 0.0f; vertices[1] = -r; vertices[2] = 0.0f;vertices[3] = 1.0f;
	normals[0] = 0.0f; normals[1] = -1; normals[2] = 0.0f;

	vertices[verticesSize-4] = 0.0f; vertices[verticesSize-3] = r; vertices[verticesSize-2] = 0.0f; vertices[verticesSize-1] = 1.0f;
	normals[normalsSize-3] = 0.0f; normals[normalsSize-2] = 1; normals[normalsSize-1] = 0.0f;

    //calculating vertices
    for (int i = 1; i < stacks; i++)
        for (int j=0; j < slices; j++)
        {
            GLfloat alpha = (GLfloat)i/stacks * PI - PI/2;
            GLfloat theta = j * 2 * PI / slices;

            int nIndex = ((i - 1) * slices + j + 1) * normalOffset;
            int vIndex = ((i - 1) * slices + j + 1) * vertexOffset;


            normals[nIndex]         =
                 vertices[vIndex] = r * fabs(cos(alpha)) * cos(theta);//x 
            normals[nIndex + 1]     =
                 vertices[vIndex + 1] = r * sin(alpha); //y
            normals[nIndex + 2]     =
                 vertices[vIndex + 2] = r * fabs(cos(alpha)) * sin(theta);//z
            vertices[vIndex + 3] = 1.0f;   //w
        }
    

    int p = 0;  //track points
    for (int j = 0; j < slices; j++)
    {
        faceIndex[p++] = 0;
        faceIndex[p++] = j + 1;
        faceIndex[p++] = (j+1) % slices + 1;
    }


    p = 3 * slices;

    for ( int i = 1; i < stacks - 1; i++)
        for ( int j = 0; j < slices; j++)
        {
            //first triangle of a rectangle
            faceIndex[ p++ ] = (i-1) * slices + 1 + j;
            faceIndex[ p++ ] = (i) * slices + 1 + j;
            faceIndex[ p++ ] = (i) * slices + 1 + (j + 1)%slices;

            //second triangle of a rectangle
            faceIndex[ p++ ] = (i) * slices + 1 + (j + 1)%slices;
            faceIndex[ p++ ] = (i-1) * slices + 1 + (j + 1)%slices;
            faceIndex[ p++ ] = (i-1) * slices + 1 + j;
        }

    
    cout<<"p = "<<p<<endl;
    cout<<"Face Size = "<<faceIndexSize<<endl;
    int j;
    for (j = 0; j < slices-1; j++)
    {
        faceIndex[p++] = verticesSize/4-1;
        faceIndex[p++] = verticesSize/4-2-j;
        faceIndex[p++] = verticesSize/4-3-j;
    }
        faceIndex[p++] = verticesSize/4-1;
        faceIndex[p++] = verticesSize/4-2-j;
        faceIndex[p++] = verticesSize/4-2;
        


    cout<<"P final = "<<p<<endl;
    cout<<"Face Size = "<<faceIndexSize<<endl;

    //cout<<vertices[verticesSize-4]<<vertices[verticesSize-5]<<vertices[verticesSize-6]<<vertices[verticesSize-7]<<vertices[verticesSize-8]<<endl;

        /*
    verticesSize = 16;
    texCoordsSize = 8;
    normalsSize = 12;
    faceIndexSize = 6;
    vertices = new GLfloat[verticesSize];
    texCoords = new GLfloat[texCoordsSize];
    normals = new GLfloat[normalsSize];
    faceIndex = new GLint[faceIndexSize];

    vertexOffset = 4;
    texOffset = 2;
    normalOffset = 3;

    //Vertices
	vertices[0] = -1.0f; vertices[1] = 1.0f; vertices[2] = 0.0f;vertices[3] = 1.0f;
    vertices[4] = -1.0f; vertices[5] = -1.0f; vertices[6] = 0.0f; vertices[7] = 1.0f; 
    vertices[8] = 1.0f; vertices[9] = -1.0f; vertices[10] =  0.0f; vertices[11] =  1.0f; 
    vertices[12] = 1.0f; vertices[13] =   1.0f; vertices[14] =  0.0f; vertices[15] =  1.0f;

    //texCoords
    texCoords[0] = -1.0; texCoords[1] =  1.0; 
    texCoords[2] = -1.0; texCoords[3] =  -1.0; 
    texCoords[4] = 1.0; texCoords[5] =  -1.0; 
    texCoords[6] = 1.0; texCoords[7] =  1.0 ;

    //normals
    normals[0] = 0; normals[1] = 0; normals[2] = 1; 
    normals[3] = 0; normals[4] = 0; normals[5] = 1; 
    normals[6] = 0; normals[7] = 0; normals[8] = 1; 
    normals[9] = 0; normals[10] = 0; normals[11] = 1;

    //faceIndex
    
    faceIndex[0] = 0; faceIndex[1] =  1; faceIndex[2] =  3; faceIndex[3] =  1; faceIndex[4] =  2; faceIndex[5] =  3;
    */

}








