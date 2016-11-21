#version 420


in Data {
    vec3 normal;
    vec4 eye;
    vec3 outColor;
    vec3 outSecondaryColor;
}DataIn;

out vec4 outputF;

void main (void)
{
	outputF = vec4(DataIn.outColor + 0.25 * DataIn.outSecondaryColor, 1.0);

}
