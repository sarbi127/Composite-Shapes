#ifndef CYLINDER_H
#define CYLINDER_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <array>
#include "objectbase.hpp"
#include "../gtfm.hpp"

namespace qbRT
{
	class Cylinder : public ObjectBase, public std::enable_shared_from_this<qbRT::Cylinder>
	{
		public:
			// Default constructor.
			Cylinder();
			
			// Override the destructor.
			virtual ~Cylinder() override;
			
			// Override the function to test for intersections.
			virtual bool TestIntersection(	const qbRT::Ray &castRay, qbRT::DATA::hitData &hitData) override;
	};
}

#endif


