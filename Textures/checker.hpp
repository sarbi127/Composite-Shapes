#ifndef CHECKER_H
#define CHECKER_H

#include "texturebase.hpp"

namespace qbRT
{
	namespace Texture
	{
		class Checker : public TextureBase
		{
			public:
				// Constructor / destructor.
				Checker();
				virtual ~Checker() override;
			
				// Function to return the color.
				virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;
			
				// Function to set the colors.
				void SetColor(const qbVector<double> &inputColor1, const qbVector<double> &inputColor2);
				void SetColor(const std::shared_ptr<qbRT::Texture::TextureBase> &inputColor1, const std::shared_ptr<qbRT::Texture::TextureBase> &inputColor2);
			
		private:
			std::shared_ptr<qbRT::Texture::TextureBase> m_p_color1;
			std::shared_ptr<qbRT::Texture::TextureBase> m_p_color2;
			
		};
	}
}

#endif




