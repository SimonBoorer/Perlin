#include <SDL.h>
#include <algorithm>

#include "Perlin.h"

int main(int argc, char *args[])
{
	int iSampleSize = 256;
	Perlin oPerlin(iSampleSize);

	SDL_Surface *pSurface  = SDL_CreateRGBSurface(SDL_SWSURFACE, iSampleSize, iSampleSize, 32,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	SDL_LockSurface(pSurface);

	float fPersistence = 0.99;
	int iOctaves = 2;

	unsigned int *pPixels = (unsigned int *) pSurface->pixels;
	for(int y = 0; y < iSampleSize; ++y)
	{
		for(int x = 0; x < iSampleSize; ++x)
		{
			float fFrequency, fAmplitude;
			float fTotal = 0.0f;

			for(int i = 1; i <= iOctaves; ++i)
			{
				fFrequency = pow(2.0f, i);
				fAmplitude = pow(fPersistence, i);

				fTotal += (oPerlin.Noise2(fFrequency * x / iSampleSize,
					fFrequency * y / iSampleSize) + 1)/ 2.0f * fAmplitude;
			}
	
			fTotal = std::min<float>(1.0f, std::max<float>(0.0f, fTotal));
			unsigned int uiNoise = (unsigned int) (fTotal * 255);

			pPixels[y * iSampleSize + x] =
				SDL_MapRGB(pSurface->format, uiNoise, uiNoise, uiNoise);
		}
	}

	SDL_UnlockSurface(pSurface);
	SDL_SaveBMP(pSurface, "Perlin.bmp");

 	return 0;
}
