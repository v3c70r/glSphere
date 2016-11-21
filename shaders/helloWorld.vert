#version 420
layout (std140) uniform Matrices
{
    mat4 m_pvm;
    mat3 m_normal;
    mat4 m_viewModel;
    mat4 m_view;
};


in vec4 position;   //model space
in vec3 normal;     //model space

out Data {
    vec3 normal;
    vec4 eye;
}DataOut;

void main()
{
    DataOut.normal = normalize( m_normal * normal);  //Normal to view space;
    DataOut.eye = -(m_viewModel * position);

    gl_Position = m_pvm * position;
}
