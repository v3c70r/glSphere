
#version 420
layout (std140) uniform Matrices
{
    mat4 m_pvm;
    mat3 m_normal;
    mat4 m_viewModel;
    mat4 m_view;
};

layout (std140) uniform Stmosphere
{
    vec4 v3LightPos;        //=vec4(-1, -0, 0,1);    //Light Position
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

    float fg;
    float fg2;
};


in vec4 position;   //model space
in vec3 normal;     //model space

out Data {
    vec3 normal;        //output normal position
    vec4 eye;           //output eye position
    vec3 outColor;      //Rayleigh Scattering out put color
    vec3 outSecondaryColor;     //Mie Scattering out put color
    vec3 v3Direction;           //Light direction
    vec3 lightPos;
    float g;
    float g2;
}DataOut;




float scale(float fCos)
{
	float x = 1.0 - fCos;
    return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main(void)
{
    vec3 v3CameraPos = -(m_viewModel * position).xyz+ position.xyz;


	vec3 v3Pos = position.xyz;
	vec3 v3Ray = v3Pos - v3CameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

    //Calculate fNear, the distance to the nearest position
	float B = 2.0 * dot(v3CameraPos, v3Ray);
	float C = fCameraHeight2 - fOuterRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	float fNear = 0.5 * (-B - sqrt(fDet));

	vec3 v3Start = v3CameraPos + v3Ray * fNear;
	fFar -= fNear;
	float fStartAngle = dot(v3Ray, v3Start) / fOuterRadius;
	float fStartDepth = exp(-1.0 / fScaleDepth);
	float fStartOffset = 0;//fStartDepth*scale(fStartAngle);

	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	vec3 v3FrontColor = vec3(0.0, 0.0, 0.0);
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(v3LightPos.xyz, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
		vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;       //Move to next point
	}

	DataOut.outSecondaryColor = v3FrontColor * fKmESun;         //Mie 
	DataOut.outColor = v3FrontColor * (v3InvWavelength * fKrESun);  //Rayleigh
	gl_Position = m_pvm * position;
	DataOut.v3Direction = v3CameraPos - v3Pos;
}

