#ifndef GRADIENT_H
#define GRADIENT_H

#include "texturebase.hpp"
#include "colormap.hpp"

namespace qbRT
{
	namespace Texture
	{
		class Gradient : public TextureBase
		{
			public:
				// Constructor / destructor.
				Gradient();
				virtual ~Gradient() override;
				
				// Function to return the color.
				virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;
				
				// *** Function to return the value.
				virtual double GetValue(const qbVector<double> &uvCoords) override;				
				
				// Function to set stops for the color map.
				void SetStop(double position, const qbVector<double> &value);
				
			private:
				qbRT::Texture::ColorMap m_colorMap;
		};		
	}
}

#endif

