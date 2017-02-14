#include <iostream>
#include <algorithm>
#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include "Perlin.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: perlin outputfile" << std::endl;
		return 0;
	}

	char* outputfile = argv[1];
	ImageOutput* out = ImageOutput::create(outputfile);
	if (!out)
	{
		std::cerr << "Could not create an ImageOutput for "
			<< outputfile << ", error = "
			<< OpenImageIO::geterror() << std::endl;
		return 0;
	}

	const int xres = 256;
	const int yres = 256;
	const int channels = 3; // RGB
	ImageSpec outspec(xres, yres, channels, TypeDesc::UINT8);

	if (!out->open(outputfile, outspec))
	{
		std::cerr << "Could not open " << outputfile
			<< ", error = " << out->geterror() << std::endl;
		ImageOutput::destroy(out);
		return 0;
	}

	const int sample_size = 256;
	Perlin perlin(sample_size);

	float persistence = 0.5;
	int octaves = 2;

	unsigned char pixels[xres * yres * channels];
	for (int y = 0; y < yres; y++)
	{
		for (int x = 0; x < xres; x++)
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

			pixels[y * xres * channels + x * channels] = noise;
			pixels[y * xres * channels + x * channels + 1] = noise;
			pixels[y * xres * channels + x * channels + 2] = noise;
		}
	}

	if (!out->write_image(TypeDesc::UINT8, pixels))
	{
		std::cerr << "Could not write pixels to " << outputfile
			<< ", error = " << out->geterror() << std::endl;
		ImageOutput::destroy(out);
		return 0;
	}

	ImageOutput::destroy(out);

	return 0;
}

