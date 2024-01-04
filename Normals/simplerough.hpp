#ifndef SIMPLEROUGH_H
#define SIMPLEROUGH_H

#include "normalbase.hpp"
#include <random>

namespace qbRT
{
	namespace Normal
	{
		class SimpleRough : public NormalBase
		{
			public:
				// Constructor / Destructor.
				SimpleRough();
				virtual ~SimpleRough() override;
			
				// Function to compute the perturbation.
				virtual qbVector<double> ComputePerturbation(const qbVector<double> &normal, const qbVector<double> &uvCoords) override;
				
			public:
				
			private:
				std::shared_ptr<std::mt19937> m_p_randGen;
				
		};
	}
}

#endif

