#ifndef H_BOX
#define H_BOX

#include <array>
#include "objectbase.hpp"
#include "../gtfm.hpp"

namespace qbRT
{
	class Box : public ObjectBase
	{
		public:
			/* The default constructor.
				Note that this will define a unit sphere at the origin. */
			Box();
			
			// Override the destructor.
			virtual ~Box() override;
			
			// Override the function to test for intersections.
			virtual bool TestIntersection(const qbRT::Ray &castRay, qbRT::DATA::hitData &hitData) override;
			
		private:
			std::array<double, 6> t;
			std::array<double, 6> u;
			std::array<double, 6> v;
	};
}

#endif
