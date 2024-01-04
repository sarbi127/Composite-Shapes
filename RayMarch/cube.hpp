#ifndef RM_CUBE
#define RM_CUBE

#include "raymarchbase.hpp"

namespace qbRT
{
	namespace RM
	{
		class Cube : public RayMarchBase
		{
			public:
				// Constructor.
				Cube();
				
				// Destructor.
				virtual ~Cube() override;
				
			private:
				// Private object function.
				double ObjectFcn(qbVector<double> *location, qbVector<double> *parms);				
		};
	}
}

#endif

