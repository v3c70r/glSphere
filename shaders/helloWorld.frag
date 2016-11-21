#version 420
layout (std140) uniform Materials
{
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    float shininess;
};

layout (std140) uniform DirectionalLight
{
    vec4 l_dir;
    vec4 l_diffuse;
    vec4 l_ambient;
    vec4 l_specular;
};

in Data
{
    vec3 normal;
    vec4 eye;
} DataIn;

out vec4 outputF;


void main()
{
    vec4 outAmbient = l_ambient * ambient;

    vec4 outDiffuse = vec4(0.0);
    vec4 outSpec = vec4(0.0);

    //normalize input vectors
    vec3 n = normalize(DataIn.normal);
    vec3 e = normalize(vec3(DataIn.eye));
    
    float intensity = max(dot(n, l_dir.xyz), 0.0);

    if (intensity > 0.0)
    {
        outDiffuse = l_diffuse * diffuse * intensity;
        //calculate halfvector
        vec3 halfVec = normalize(l_dir.xyz + e);
        //spec
        float NdotHV = max(dot(halfVec, n) ,0.0);
        outSpec = specular * l_specular * pow(NdotHV, shininess);
    }
    //if (abs(DataIn.eye.z ) < 15.000001)
     //   outputF = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    //else
        outputF = outAmbient + outDiffuse + outSpec;
}
