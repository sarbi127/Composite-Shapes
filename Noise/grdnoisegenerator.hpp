#ifndef GRDNOISEGENERATOR_H
#define GRDNOISEGENERATOR_H

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <vector>
#include <random>
#include "noisebase.hpp"

namespace qbRT
{
	namespace Noise
	{
		class GrdNoiseGenerator : public NoiseBase
		{
			public:
				// Constructor destructor.
				GrdNoiseGenerator();
				virtual ~GrdNoiseGenerator() override;
				
				// Function to get the value at a specific location.
				virtual double GetValue(double x, double y) override;
				
				// Function to setup the grid.
				virtual void SetupGrid(int scale) override;
				
			private:				
				// Normalize vector.
				std::vector<double> ComputeNormDisp(double x1, double y1, double x2, double y2);
				
			/* Note that these are declared public for debug purposes only. */
			public:
				// Store the grid of vectors.
				std::vector<std::vector<double>> m_vectorGridX;
				std::vector<std::vector<double>> m_vectorGridY;
				
				bool m_wrap = false;

		};
	}
}

#endif

