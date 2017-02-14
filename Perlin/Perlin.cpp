#include "Perlin.h"

#include <ctime>
#include <cstdlib>
#include <cmath>

Perlin::Perlin(int sample_size, unsigned int seed)
	: sample_size_(sample_size)
{
	if (seed == 0)
	{
		// Initialize seed to Unix time
		seed = (unsigned int) time(NULL);
	}

	// Seed psuedo-random number generator
	srand(seed);

	// Initialize the permutation table
	permutations_ = new int[sample_size];

	for (int i = 0; i < sample_size; i++)
		permutations_[i] = i;

	for (int i = 0; i < sample_size; i++)
	{
		int j = rand() % sample_size;

		int k = permutations_[i];
		permutations_[i] = permutations_[j];
		permutations_[j] = k;
	}
}

Perlin::~Perlin()
{
	delete permutations_;
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

float Perlin::Grad(int hash, float x, float y, float z)
{
	int h = hash & 15;

	float u = h < 8 ? x : y;
	float v = h < 4 ? y : h == 12 || h == 14 ? x : z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float Perlin::Noise(float x)
{
	// Compute neighbouring grid coordinates
	int ix;
	ix = (int) x % (sample_size_ - 1);

	float fx0, fx1;
	fx0 = x -  floor(x);
	fx1 = fx0 - 1.0f;

	// Compute fade curves
	float u;
	u = Fade(fx0);

	return Lerp(u,	Grad(permutations_[ix], fx0),
			Grad(permutations_[ix + 1], fx1));
}

// Bi-dimensional noise
float Perlin::Noise2(float x, float y)
{
	// Compute neighbouring grid coordinates
	int ix, iy;
	ix = (int) x % (sample_size_ - 1);
	iy = (int) y % (sample_size_ - 1);

	float fx0, fx1, fy0, fy1;
	fx0 = x -  floor(x);
	fx1 = fx0 - 1.0f;
	fy0 = y - floor(y);
	fy1 = fy0 - 1.0f;

	// Permute hash coordinates
	int A, B;
	A = permutations_[ix] + iy;
	B = permutations_[ix + 1] + iy;

	// Compute fade curves
	float u, v;
	u = Fade(fx0);
	v = Fade(fy0);

	// Interpolate between directions
	return Lerp(v,	Lerp(u,	Grad(permutations_[A], fx0, fy0),
				Grad(permutations_[B], fx1, fy0)),
			Lerp(u,	Grad(permutations_[A + 1], fx0, fy1),
				Grad(permutations_[B + 1], fx1, fy1)));
}

float Perlin::Noise3(float x, float y, float z)
{
	// Compute neighbouring grid coordinates
	int ix, iy, iz;
	ix = (int) x % (sample_size_ - 1);
	iy = (int) y % (sample_size_ - 1);
	iz = (int) z % (sample_size_ - 1);

	float fx0, fx1, fy0, fy1, fz0, fz1;
	fx0 = x -  floor(x);
	fx1 = fx0 - 1.0f;
	fy0 = y - floor(y);
	fy1 = fy0 - 1.0f;
	fz0 = z - floor(z);
	fz1 = fz0 - 1.0f;

	// Permute hash coordinates
	int A, AA, AB, B, BA, BB;
	A = permutations_[ix] + iy;
	AA = permutations_[A] + iz;
	AB = permutations_[A + 1] + iz;
	B = permutations_[ix + 1] + iy;
	BA = permutations_[B] + iz;
	BB = permutations_[B + 1] + iz;

	// Compute fade curves
	float u, v, w;
	u = Fade(fx0);
	v = Fade(fy0);
	w = Fade(fz0);

	// Interpolate between directions
	return Lerp(w,	Lerp(v,	Lerp(u,	Grad(permutations_[AA], fx0, fy0, fz0),
					Grad(permutations_[BA], fx1, fy0, fz0)),
				Lerp(u,	Grad(permutations_[AB], fx0, fy1, fz0),
					Grad(permutations_[BB], fx1, fy1, fz0))),
			Lerp(v,	Lerp(u,	Grad(permutations_[AA + 1], fx0, fy0, fz1),
					Grad(permutations_[BA + 1], fx1, fy0, fz1)),
				Lerp(u,	Grad(permutations_[AB + 1], fx0, fy1, fz1),
					Grad(permutations_[BB + 1], fx1, fy1, fz1))));
}

