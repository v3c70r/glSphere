
#version 420
layout (std140) uniform Matrices
{
    mat4 m_pvm;
    mat3 m_normal;
    mat4 m_viewModel;
    mat4 m_view;
};

layout (std140) uniform Atmosphere
{
    vec4 v3LightPos;    //Light Position
    vec3 v3InvWavelength;   //1/pow(waveLength, 4) of R, G, B
    float fCameraHeight;     //Distance from core to camera
    float fCameraHeight2;    //^2
    float fOuterRadius;      
    float fOuterRadius2;     //^2
    float fInnerRadius;
    float fInnerRadius2;
    float fKrESun;
    float fKmESun;
    float fKr4PI;
    float fKm4PI;
    float fScale;

    float fScaleDepth;
    float fScaleOverScaleDepth;

    int nSamples;
    float fSamples;
    //float fg;
    //float fg2;
};

layout (std140) uniform Materials
{
    vec4 diffues;
};


in vec4 position;   //model space
in vec3 normal;     //model space

out Data {
    vec3 normal;
    vec4 eye;
    vec3 outColor;
    vec3 outSecondaryColor;
}DataOut;

float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}


void main(void)
{
    vec3 v3CameraPos = -(m_viewModel * position).xyz+ position.xyz;
	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	vec3 v3Pos = position.xyz;
	vec3 v3Ray = v3Pos - v3CameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	float B = 2.0 * dot(v3CameraPos, v3Ray);
	float C = fCameraHeight2 - fOuterRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	float fNear = 0.5 * (-B - sqrt(fDet));

    //define postions
	vec3 v3Start = v3CameraPos + v3Ray * fNear;
	fFar -= fNear;
	float fDepth = exp((fInnerRadius - fOuterRadius) / fScaleDepth);
	float fCameraAngle = dot(-v3Ray, v3Pos) / length(v3Pos);
	float fLightAngle = dot(v3LightPos.xyz, v3Pos) / length(v3Pos);
	float fCameraScale = scale(fCameraAngle);
	float fLightScale = scale(fLightAngle);
	float fCameraOffset = 0;            //fDepth*fCameraScale;
	float fTemp = (fLightScale + fCameraScale);

	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

    //Using diffuse reflection to calculate the front color

    vec3 l_dir = normalize(v3LightPos.xyz);
    vec3 n = normalize(normal);

    float intensity = max(dot(l_dir, n), 0.0)*3;

    vec3 groundColor = intensity * diffues.xyz;
	vec3 v3FrontColor = groundColor;//vec3(0.0, 0.0, 0.0);//
	vec3 v3Attenuate = vec3(0.0, 0.0, 0.0);
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fScatter = fDepth*fTemp - fCameraOffset;
		v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}


	DataOut.outColor = v3FrontColor * (v3InvWavelength * fKrESun + fKmESun);

	DataOut.outSecondaryColor = v3Attenuate;

    gl_Position = m_pvm * position;
}
