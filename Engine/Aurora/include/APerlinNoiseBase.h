#ifndef _APERLINNOISEBASE_H_
#define _APERLINNOISEBASE_H_

#define MAX_OCTAVE    16
#define LERP(s, a, b) ((a) * (1.0f - (s)) + (b) * (s))
#define S_CURVE(t)    ((t) * (t) * (3.0f - 2.0f * (t)))

class APerlinNoiseBase
{
public:
	inline void SetTurbulence(bool bTurbulence) { m_bTurbulence = bTurbulence; }
	inline bool GetTurbulence() { return m_bTurbulence; }
	inline void SetActiveOctave(int nIndex) { m_nActiveOctave = nIndex; }
	inline float GetBaseAmplitude() { return m_vBaseAmplitude; }

protected:
	APerlinNoiseBase();
	~APerlinNoiseBase();

	bool InitParams(float vAmplitude, int nWaveLength, float vPersistence, int nOctaveNum);

	unsigned int RandInteger();
	float RandFloat();

	struct NOISEVALUE
	{
		float v[3];
	};

	bool m_bTurbulence;    // flag indicates whether use sum of fractal fabs value or not
	unsigned int m_dwSeed;
	int m_nBaseWaveLength;
	float m_vBaseAmplitude;
	float m_vPersistence;

	int m_nActiveOctave;
	int m_nOctaveNum;
	int m_nStartPos[MAX_OCTAVE];
	int m_nWaveLength[MAX_OCTAVE];
	float m_vAmplitude[MAX_OCTAVE];
};

#endif
