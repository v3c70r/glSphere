//Mesh.h
//Define a mesh class to encapsulate VAO and VBO

#ifndef __COOR
#define __COOR

#include <GL/glew.h>
#include <GL/freeglut.h>


class Line
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
    void init();

    /* set arraies
     * */

    void setData();

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
        glDrawElements(GL_LINES, faceIndexSize, GL_UNSIGNED_INT, 0);
    }


    inline void unbind()
    {
        glBindVertexArray(0);
    }


    ~Line()
    {
        glBindVertexArray(0);
        delete []vertices;
        delete []normals;
        delete []texCoords;
        delete []faceIndex;
    }

};


#endif
