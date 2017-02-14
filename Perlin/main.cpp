#include <SDL.h>
#include <algorithm>

#include "Perlin.h"

int main()
{
	int sample_size = 256;
	Perlin perlin(sample_size);

	SDL_Surface *surface  = SDL_CreateRGBSurface(SDL_SWSURFACE, sample_size, sample_size, 32,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	SDL_LockSurface(surface);

	float persistence = 0.99;
	int octaves = 2;

	unsigned int *pixels = (unsigned int *) surface->pixels;
	for (int y = 0; y < sample_size; ++y)
	{
		for (int x = 0; x < sample_size; ++x)
		{
			float frequency, amplitude;
			float total = 0.0f;

			for (int i = 1; i <= octaves; ++i)
			{
				frequency = pow(2.0f, i);
				amplitude = pow(persistence, i);

				total += (perlin.Noise2(frequency * x / sample_size,
					frequency * y / sample_size) + 1)/ 2.0f * amplitude;
			}
	
			total = std::min<float>(1.0f, std::max<float>(0.0f, total));
			unsigned int noise = (unsigned int) (total * 255);

			pixels[y * sample_size + x] =
				SDL_MapRGB(surface->format, noise, noise, noise);
		}
	}

	SDL_UnlockSurface(surface);
	SDL_SaveBMP(surface, "Perlin.bmp");

	return 0;
}

