#ifndef OBJSPHERE_H
#define OBJSPHERE_H

#define _USE_MATH_DEFINES
#include <cmath>

#include "objectbase.hpp"
#include "../gtfm.hpp"

namespace qbRT
{
	class ObjSphere : public ObjectBase
	{
		public:
			/* The default constructor.
				Note that this will define a unit sphere at the origin. */
			ObjSphere();
			
			// Override the destructor.
			virtual ~ObjSphere() override;
			
			// Override the function to test for intersections.
			virtual bool TestIntersection(const qbRT::Ray &castRay, qbRT::DATA::hitData &hitData) override;
			
		private:
		
		
	};
}

#endif

