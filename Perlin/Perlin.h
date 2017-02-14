#ifndef PERLIN_H_
#define PERLIN_H_

class Perlin
{
public:
	Perlin(int sample_size = 256, unsigned int seed = 0);
	~Perlin();

	float Noise(float x);
	float Noise2(float x, float y);
	float Noise3(float x, float y, float z);

protected:
	float Fade(float t);
	float Lerp(float t, float a, float b);
	float Grad(int hash, float x);
	float Grad(int hash, float x, float y);
	float Grad(int hash, float x, float y, float z);

	int sample_size_;
	int* permutations_;
};

#endif // PERLIN_H_

