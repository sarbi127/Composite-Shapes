#ifndef BASICNOISE_H
#define BASICNOISE_H

#include "../Noise/grdnoisegenerator.hpp"
#include "../Noise/valnoisegenerator.hpp"
#include "texturebase.hpp"
#include "colormap.hpp"

namespace qbRT
{
	namespace Texture
	{
		class BasicNoise : public TextureBase
		{
			public:
				// Constructor / destructor.
				BasicNoise();
				virtual ~BasicNoise() override;
				
				// Function to return the color.
				virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;
				
				// Function to set the color map.
				void SetColorMap(const std::shared_ptr<qbRT::Texture::ColorMap> &colorMap);
				
				// Function to set the amplitude.
				void SetAmplitude(double amplitude);
				
				// Function to set the scale.
				void SetScale(int scale);
				
			public:
				// Store the color map.
				std::shared_ptr<qbRT::Texture::ColorMap> m_colorMap;
				bool m_haveColorMap = false;
				
				// We need a NoiseGenerator instance.
				qbRT::Noise::GrdNoiseGenerator m_noiseGenerator;
				
				// Store the amplitude.
				double m_amplitude = 8.0;
				
				// Store the scale.
				int m_scale = 3;
				
		};
	}
}

#endif

