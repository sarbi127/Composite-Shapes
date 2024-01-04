#ifndef MARBLE_H
#define MARBLE_H

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "../Noise/grdnoisegenerator.hpp"
#include "texturebase.hpp"
#include "colormap.hpp"

namespace qbRT
{
	namespace Texture
	{
		class Marble : public TextureBase
		{
			public:
				// Constructor / destructor.
				Marble();
				virtual ~Marble() override;
				
				// Function to return the color.
				virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;
				
				// Function to set the color map.
				void SetColorMap(const std::shared_ptr<qbRT::Texture::ColorMap> &colorMap);
				
				// Function to set the amplitude.
				void SetAmplitude(double amplitude1, double amplitude2);
				
				// Function to set the scale.
				void SetScale(int scale1, int scale2);
				
				// Function to set the min and max values.
				void SetMinMax(double minValue, double maxValue);
				
				// Function to set the sine wave parameters.
				void SetSine(double amplitude, double frequency);
				
			public:
				// Store the color map.
				std::shared_ptr<qbRT::Texture::ColorMap> m_colorMap;
				bool m_haveColorMap = false;
				
				// Store a list of noise generator instances.
				std::vector<qbRT::Noise::GrdNoiseGenerator> m_noiseGeneratorList;
				
				// Store the amplitude.
				double m_amplitude1 = 8.0;
				double m_amplitude2 = 8.0;
				
				// Store the scale.
				int m_scale1 = 4;		
				int m_scale2 = 40;
				
				// Define min and max values.
				double m_maxValue = 1.0;
				double m_minValue = -1.0;			
				
				// Define underlying sine wave parameters.
				double m_sineAmplitude = 0.25;
				double m_sineFrequency = 4.0;	
		};
	}
}

#endif

