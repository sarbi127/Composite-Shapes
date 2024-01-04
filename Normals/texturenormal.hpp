#ifndef TEXTURENORMAL_H
#define TEXTURENORMAL_H

#include "normalbase.hpp"
#include <random>

namespace qbRT
{
	namespace Normal
	{
		class TextureNormal : public NormalBase
		{
			public:
				// Constructor / Destructor.
				TextureNormal();
				virtual ~TextureNormal() override;
			
				// Function to compute the perturbation.
				virtual qbVector<double> ComputePerturbation(const qbVector<double> &normal, const qbVector<double> &uvCoords) override;
				
				// Function to assign the base texture.
				void AssignBaseTexture(const std::shared_ptr<qbRT::Texture::TextureBase> &inputTexture);
				
			public:
				double m_scale = 1.0;
				bool m_reverse = false;
				
			private:
				bool m_haveTexture = false;
				std::shared_ptr<qbRT::Texture::TextureBase> m_p_baseTexture;
				
		};
	}
}

#endif


