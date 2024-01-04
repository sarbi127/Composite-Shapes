#ifndef COLORMAP_H
#define COLORMAP_H

#include "../qbVector.h"

namespace qbRT
{
	namespace Texture
	{
		class ColorMap
		{
			public:
				// Constructor / destructor.
				ColorMap();
				~ColorMap();
				
				// Function to set a stop as a color.
				void SetStop(double position, const qbVector<double> &value);
				
				// Function to get the color at a particular position.
				qbVector<double> GetColor(double position);
				
			private:
				std::vector<double> m_stopPositions;
				std::vector<qbVector<double>> m_stopValues;
		};
	}
}

#endif

