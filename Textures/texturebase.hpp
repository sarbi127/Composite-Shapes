#ifndef TEXTUREBASE_H
#define TEXTUREBASE_H

#include<memory>
#include "../qbMatrix.h"
#include "../qbVector.h"
#include "../ray.hpp"

namespace qbRT
{   
    namespace Texture
    {
          
            class TextureBase
            {
           
                public:
				// Constructor / destructor.
				TextureBase();
				virtual ~TextureBase();
				
				// Function to retrun the color at a given point in the (u,v) coordinate system.
				// Note that the color is returned as a 4-dimensional vector (RGBA).
				virtual qbVector<double> GetColor(const qbVector<double> &uvCoords);
				
				// *** Function to return the actual texture value at a given point in the (u,v) coordinate system.
				virtual double GetValue(const qbVector<double> &uvCoords);				
				
				// Function to set transform.
				void SetTransform(const qbVector<double> &translation, const double &rotation, const qbVector<double> &scale);
				
				// Function to blend RGBA colors, returning a 3-dimensional (RGB) result.
				static qbVector<double> BlendColors(const std::vector<qbVector<double>> &inputColorList);
				
				// Function to apply the local transform to the given input vector.
				qbVector<double> ApplyTransform(const qbVector<double> &inputVector);
				
			private:
			
			private:
				// Initialise the transform matrix to the identity matrix.
				qbMatrix2<double> m_transformMatrix {3, 3, std::vector<double>{1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}};
				
		};
    } 
    

}

#endif