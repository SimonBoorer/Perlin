#include <boost/program_options.hpp>
using namespace boost::program_options;

#include <iostream>
using namespace std;

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include <algorithm>

#include "Perlin.h"

int main(int argc, char* argv[])
{
	try
	{
		options_description desc("Allowed options");
		desc.add_options()
			("help,h", "produce help message")
			("xres,x", value<int>()->default_value(255),
				"x resolution")
			("yres,y", value<int>()->default_value(255),
				"y resolution")
			("sample-size,s", value<int>()->default_value(256),
				"sample size")
			("seed,r", value<int>()->default_value(0),
				"psuedo-random seed")
			("persistence,p", value<float>()->default_value(0.5f),
				"persistence value")
			("octaves,o", value<int>()->default_value(2),
				"number of octaves")
		;

		options_description hidden("Hidden options");
		hidden.add_options()
			("output-file", value<string>()->required(), "output file")
		;

		options_description all("Allowed options");
		all.add(desc).add(hidden);

		positional_options_description p;
		p.add("output-file", 1);

		variables_map vm;
		store(command_line_parser(argc, argv).options(all)
			.positional(p).run(), vm);

		if (vm.count("help"))
		{
			cout << "Usage: " << argv[0] << " [options] output-file" << endl;
			cout << desc << endl;
			return 0;
		}

		notify(vm);

		string outputfile = vm["output-file"].as<string>();
		ImageOutput* out = ImageOutput::create(outputfile);
		if (!out)
		{
			cerr << "Could not create an ImageOutput for "
				<< outputfile << ", error = "
				<< OpenImageIO::geterror() << endl;
			return 0;
		}

		const int xres = vm["xres"].as<int>();
		const int yres = vm["yres"].as<int>();
		const int channels = 3; // RGB
		ImageSpec outspec(xres, yres, channels, TypeDesc::UINT8);

		if (!out->open(outputfile, outspec))
		{
			cerr << "Could not open " << outputfile
				<< ", error = " << out->geterror() << endl;
			ImageOutput::destroy(out);
			return 0;
		}

		const int sample_size = vm["sample-size"].as<int>();
		const int seed = vm["seed"].as<int>();
		Perlin perlin(sample_size, seed);

		float persistence = vm["persistence"].as<float>();
		int octaves = vm["octaves"].as<int>();

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
	
				total = min<float>(1.0f, max<float>(0.0f, total));
				unsigned int noise = (unsigned int) (total * 255);

				pixels[y * xres * channels + x * channels] = noise;
				pixels[y * xres * channels + x * channels + 1] = noise;
				pixels[y * xres * channels + x * channels + 2] = noise;
			}
		}

		if (!out->write_image(TypeDesc::UINT8, pixels))
		{
			cerr << "Could not write pixels to " << outputfile
				<< ", error = " << out->geterror() << endl;
			ImageOutput::destroy(out);
			return 0;
		}

		ImageOutput::destroy(out);
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
	}
}

