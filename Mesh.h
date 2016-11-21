//Mesh.h
//Define a mesh class to encapsulate VAO and VBO

#ifndef __MESH_H
#define __MESH_H

#include <GL/glew.h>
#include <GL/freeglut.h>


class Mesh
{
private:
    GLfloat *vertices;
    GLuint verticesSize;
    GLfloat *normals;
    GLuint normalsSize;
    GLfloat *texCoords;
    GLuint texCoordsSize;
    GLint *faceIndex;
    GLuint faceIndexSize;

    GLint vertexOffset;
    GLint texOffset;
    GLint normalOffset;

    GLuint vao;


public:
    /* create a VAO
     * NOTE: Should be init after shader created
     */
    void init(GLfloat r);

    /* set arraies
     * */

    void setData(GLfloat ra);

    inline int getVao()
    {
        return vao;
    }


    inline void bind()
    {
        glBindVertexArray(vao);
    }
    inline void draw()
    {
        glDrawElements(GL_TRIANGLES, faceIndexSize, GL_UNSIGNED_INT, 0);
    }


    inline void unbind()
    {
        glBindVertexArray(0);
    }


    ~Mesh()
    {
        glBindVertexArray(0);
        delete []vertices;
        delete []normals;
        delete []texCoords;
        delete []faceIndex;
    }

};


#endif
