#ifndef NORMALBASE_H
#define NORMALBASE_H

#include <memory>
#include "../qbMatrix.h"
#include "../qbVector.h"
#include "../ray.hpp"
#include "../Textures/texturebase.hpp"

namespace qbRT
{
	namespace Normal
	{
		class NormalBase
		{
			public:
				// Constructor / destructor.
				NormalBase();
				virtual ~NormalBase();
				
				// Function to compute the perturbation.
				virtual qbVector<double> ComputePerturbation(const qbVector<double> &normal, const qbVector<double> &uvCoords);
				
				// Function to perturb the given normal.
				qbVector<double> PerturbNormal(const qbVector<double> &normal, const qbVector<double> &perturbation);
				
				// *** Function to perform numerical differentiation of a texture in UV space.
				qbVector<double> TextureDiff(const std::shared_ptr<qbRT::Texture::TextureBase> &inputTexture, const qbVector<double> &uvCoords);				
				
				// Function to set the amplitude scale.
				void SetAmplitude(double amplitude);
				
				// Function to set transform.
				void SetTransform(const qbVector<double> &translation, const double &rotation, const qbVector<double> &scale);				
				
				// Function to apply the local transform to the given input vector.
				qbVector<double> ApplyTransform(const qbVector<double> &inputVector);				
				
			public:
				// Store the amplitude scale factor.
				double m_amplitudeScale = 1.0;	
			
			private:
				// Initialise the transform matrix to the identity matrix.
				qbMatrix2<double> m_transformMatrix {3, 3, std::vector<double>{1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}};
				
		};
	}
}

#endif

