
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "coordinate.h"
#include "tools/include/vsShaderLib.h"
#include "tools/include/vsMathLib.h"
#include <cmath>
#include <iostream>
#define PI 3.141592653f
using namespace std;

void Line::init()
{
    setData();
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
void Line::setData()
{

    //Load a sphere according to its stacks and slices
    GLfloat length = 20.0f;

    vertexOffset = 4;
    texOffset = 2;
    normalOffset = 3;

    verticesSize = vertexOffset * 6;
    texCoordsSize = texOffset * 2;
    normalsSize = 6 * normalOffset;
    faceIndexSize = 6;

    vertices = new GLfloat[verticesSize];
    texCoords = new GLfloat[texCoordsSize];
    normals = new GLfloat[normalsSize];
    faceIndex = new GLint[faceIndexSize];

    //x
    vertices[0] = -length; vertices[1] = 0.0f; vertices[2] = 0.0f;vertices[3] = 1.0f;
    vertices[4] = length; vertices[5] = 0.0f; vertices[6] = 0.0f;vertices[7] = 1.0f;

    //y
    vertices[8] = 0.0f; vertices[9] = length; vertices[10] = 0.0f;vertices[11] = 0.0f;
    vertices[12] = 0.0f; vertices[13] = -length; vertices[14] = 0.0f;vertices[15] = 1.0f;

    //z
    vertices[16] = 0.0f; vertices[17] = 0.0f; vertices[18] = length;vertices[19] = 0.0f;
    vertices[20] = 0.0f; vertices[21] = 0.0f; vertices[22] = -length;vertices[23] = 1.0f;

    faceIndex[0] = 0; faceIndex[1] = 1;
    faceIndex[2] = 2; faceIndex[3] = 3;
    faceIndex[4] = 4; faceIndex[5] = 5;

}









