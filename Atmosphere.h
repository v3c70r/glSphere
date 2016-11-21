#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H
class Atmosphere
{
public:
		int m_nSamples;		// Number of sample rays to use in integral equation
		float m_fSamples;		// Number of sample rays to use in integral equation
		float m_Kr;		// Rayleigh scattering constant
		float m_Kr4PI;
		float m_Km ;		// Mie scattering constant
		float m_Km4PI;
		float m_ESun;		// Sun brightness constant
		float m_g;		// The Mie phase asymmetry factor

		float m_fInnerRadius;
		float m_fOuterRadius;
		float m_fScale;

		float m_fWavelength[3];
		float m_fWavelength4[3];

		float m_fRayleighScaleDepth;
		float m_fMieScaleDepth;

};

#endif
