#ifndef CONE_H
#define CONE_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <array>
#include "objectbase.hpp"
#include "../gtfm.hpp"

namespace qbRT
{
	class Cone : public ObjectBase
	{
		public:
			// Default constructor.
			Cone();
			
			// Override the destructor.
			virtual ~Cone() override;
			
			// Override the function to test for intersections.
			virtual bool TestIntersection(	const qbRT::Ray &castRay, qbRT::DATA::hitData &hitData) override;			
	};
}

#endif
