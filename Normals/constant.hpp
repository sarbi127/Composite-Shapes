#ifndef Constant_H
#define Constant_H

#include "normalbase.hpp"
#include <random>

namespace qbRT
{
	namespace Normal
	{
		class Constant : public NormalBase
		{
			public:
				// Constructor / Destructor.
				Constant();
				virtual ~Constant() override;
			
				// Function to compute the perturbation.
				virtual qbVector<double> ComputePerturbation(const qbVector<double> &normal, const qbVector<double> &uvCoords) override;
				
			public:
				qbVector<double> m_displacement {3};
				
			private:
				
				
		};
	}
}

#endif

