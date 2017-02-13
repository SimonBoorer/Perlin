#ifndef _PERLIN_H_
#define _PERLIN_H_

class Perlin
{
public:
	Perlin(int a_iSampleSize = 256, unsigned int a_uiSeed = 0);
	~Perlin();

	float Noise2(float x, float y);

protected:
	float Fade(float t);
	float Lerp(float t, float a, float b);
	float Grad(int hash, float x, float y);

	int m_iSampleSize;
	int *m_aiPermutations;
};

#endif // _PERLIN_H_
