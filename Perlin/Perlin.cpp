#include "Perlin.h"

#include <ctime>
#include <cstdlib>
#include <cmath>

Perlin::Perlin(int a_iSampleSize, unsigned int a_uiSeed)
	: m_iSampleSize(a_iSampleSize)
{
	if(a_uiSeed == 0)
	{
		// Initialize seed to Unix time
		a_uiSeed = (unsigned int) time(NULL);
	}

	// Seed psuedo-random number generator
	srand(a_uiSeed);

	// Initialize the permutation table
	m_aiPermutations = new int[a_iSampleSize];

	for(int i = 0; i < a_iSampleSize; i++)
		m_aiPermutations[i] = i;

	for(int i = 0; i < a_iSampleSize; i++)
	{
		int j = rand() % a_iSampleSize;

		int k = m_aiPermutations[i];
		m_aiPermutations[i] = m_aiPermutations[j];
		m_aiPermutations[j] = k;
	}
}

Perlin::~Perlin()
{
	delete m_aiPermutations;
}

// Nonlinear fading of t
float Perlin::Fade(float t)
{
	return t * t * (3 - 2 * t);
}

// Linear interpolation between a and b
float Perlin::Lerp(float t, float a, float b)
{
	return a + t * (b - a);
}

float Perlin::Grad(int hash, float x)
{
	return (hash & 1) == 0 ? x : -x;
}

float Perlin::Grad(int hash, float x, float y)
{
	int h = hash & 3;

	float u = (h & 2) == 0 ? x : -x;
	float v = (h & 1) == 0 ? y : -y;

	return u + v;
}

float PerlinNoise::Grad(int hash, float x, float y, float z)
{
	int h = hash & 15;

	float u = h < 8 ? x : y;
	float v = h < 4 ? y : h == 12 || h == 14 ? x : z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise::Noise(float x)
{
	// Compute neighbouring grid coordinates
	int ix;
	ix = (int) x % (m_iSampleSize - 1);

	float fx0, fx1;
	fx0 = x -  floor(x);
	fx1 = fx0 - 1.0f;
     
	// Compute fade curves
	float u;
	u = Fade(fx0);
     
	return Lerp(u, Grad(m_aiPermutations[ix], fx0),
				   Grad(m_aiPermutations[ix + 1], fx1));
}

// Bi-dimensional noise
float Perlin::Noise2(float x, float y)
{
	// Compute neighbouring grid coordinates
	int ix, iy;
	ix = (int) x % (m_iSampleSize - 1);
	iy = (int) y % (m_iSampleSize - 1);

	float fx0, fx1, fy0, fy1;
	fx0 = x -  floor(x);
	fx1 = fx0 - 1.0f;
	fy0 = y - floor(y);
	fy1 = fy0 - 1.0f;

	// Permute hash coordinates
	int A, B;
	A = m_aiPermutations[ix] + iy;
	B = m_aiPermutations[ix + 1] + iy;

	// Compute fade curves
	float u, v;
	u = Fade(fx0);
	v = Fade(fy0);

    // Interpolate between directions
	return Lerp(v, Lerp(u, Grad(m_aiPermutations[A], fx0, fy0),
						   Grad(m_aiPermutations[B], fx1, fy0)),
				   Lerp(u, Grad(m_aiPermutations[A + 1], fx0, fy1),
						   Grad(m_aiPermutations[B + 1], fx1, fy1)));
}

float Perlin::Noise3(float x, float y, float z)
{
	// Compute neighbouring grid coordinates
	int ix, iy, iz;
	ix = (int) x % (m_iSampleSize - 1);
	iy = (int) y % (m_iSampleSize - 1);
	iz = (int) z % (m_iSampleSize - 1);

	float fx0, fx1, fy0, fy1, fz0, fz1;
	fx0 = x -  floor(x);
	fx1 = fx0 - 1.0f;
	fy0 = y - floor(y);
	fy1 = fy0 - 1.0f;
	fz0 = z - floor(z);
	fz1 = fz0 - 1.0f;

	// Permute hash coordinates
	int A, AA, AB, B, BA, BB;
	A = m_aiPermutations[ix] + iy;
	AA = m_aiPermutations[A] + iz;
	AB = m_aiPermutations[A + 1] + iz;
	B = m_aiPermutations[ix + 1] + iy;
	BA = m_aiPermutations[B] + iz;
	BB = m_aiPermutations[B + 1] + iz;

	// Compute fade curves
	float u, v, w;
	u = Fade(fx0);
	v = Fade(fy0);
	w = Fade(fz0);

    // Interpolate between directions
	return Lerp(w, Lerp(v, Lerp(u, Grad(m_aiPermutations[AA], fx0, fy0, fz0),
								   Grad(m_aiPermutations[BA], fx1, fy0, fz0)),
						   Lerp(u, Grad(m_aiPermutations[AB], fx0, fy1, fz0),
								   Grad(m_aiPermutations[BB], fx1, fy1, fz0))),
				   Lerp(v, Lerp(u, Grad(m_aiPermutations[AA + 1], fx0, fy0, fz1),
								   Grad(m_aiPermutations[BA + 1], fx1, fy0, fz1)),
						   Lerp(u, Grad(m_aiPermutations[AB + 1], fx0, fy1, fz1),
								   Grad(m_aiPermutations[BB + 1], fx1, fy1, fz1))));
}
