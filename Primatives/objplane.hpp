#ifndef OBJPLANE_H
#define OBJPLANE_H


#include "objectbase.hpp"
#include "../gtfm.hpp"

namespace qbRT
{
   class ObjPlane : public ObjectBase, public std::enable_shared_from_this<qbRT::ObjPlane>
	{
		public:
			// Default constructor.
			ObjPlane();
		
			// Override the destructor.
			virtual ~ObjPlane() override;
		
			// Override the function to test for intersections.
			virtual bool TestIntersection(	const qbRT::Ray &castRay, qbRT::DATA::hitData &hitData) override;
																			
		private:
		
		
	};

}

#endif

