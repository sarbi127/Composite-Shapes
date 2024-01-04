#ifndef FLAT_H
#define FLAT_H

#include "texturebase.hpp"

namespace qbRT
{
	namespace Texture
	{
		class Flat : public TextureBase
		{
			public:
				// Constructor / destructor.
				Flat();
				virtual ~Flat() override;
				
				// Function to return the color.
				virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;
				
				// Function to set the color.
				void SetColor(const qbVector<double> &inputColor);
				
			private:
				qbVector<double> m_color {4};
				
		};
	}
}

#endif


