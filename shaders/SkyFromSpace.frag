
#version 420
in Data {
    vec3 normal;
    vec4 eye;
    vec3 outColor;
    vec3 outSecondaryColor;
    vec3 v3Direction;
    vec3 lightPos;
    float g;
    float g2;
}DataIn;



out vec4 outputF;


void main (void)
{
	float fCos = dot(DataIn.lightPos, DataIn.v3Direction) / length(DataIn.v3Direction);
	float rayleighPhase = 0.75 * (1.0 + fCos*fCos);
	float miePhase = 1.5 * ((1.0 - DataIn.g2) / (2.0 + DataIn.g2)) * (1.0 + fCos*fCos) / pow(1.0 + DataIn.g2 - 2.0*DataIn.g*fCos, 1.5);
	outputF = vec4(rayleighPhase * DataIn.outColor + miePhase * DataIn.outSecondaryColor, 1.0);
}

