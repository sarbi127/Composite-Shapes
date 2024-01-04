#ifndef RAYMARCHBASE_H
#define RAYMARCHBASE_H

#include <functional>
#include "sdfunc.hpp"
#include "../Primatives/objectbase.hpp"
#include "../Primatives/box.hpp"

namespace qbRT
{
	namespace RM
	{
		class RayMarchBase : public ObjectBase
		{
			public:
				// Default constructor.
				RayMarchBase();
				
				// Destructor.
				virtual ~RayMarchBase() override;
				
				// Override the function to test for intersections.
				virtual bool TestIntersection(	const qbRT::Ray &castRay, qbRT::DATA::hitData &hitData) override;
				
				// Function to set the object function.
				void SetObjectFcn( std::function<double(qbVector<double>*, qbVector<double>*)> objectFcn);
				
				// Function to evaluate the Signed Distance Function (SDF) at the given coordinates.
				double EvaluateSDF(	qbVector<double> *location, qbVector<double> *parms );
												
			public:
				// Bounding box.
				qbRT::Box m_boundingBox = qbRT::Box();
				
				// Parameters.
				qbVector<double> m_parms {3};
				
			private:
				// Pointer to object function.
				std::function<double(qbVector<double> *, qbVector<double> *)> m_objectFcn;			
			
				bool m_haveObjectFcn = false;
				double m_epsilon;
				int m_maxSteps;
				
				const double m_h = 0.001;
				qbVector<double> m_xDisp {std::vector<double>{m_h, 0.0, 0.0}};
				qbVector<double> m_yDisp {std::vector<double>{0.0, m_h, 0.0}};
				qbVector<double> m_zDisp {std::vector<double>{0.0, 0.0, m_h}};
																						
		};
	}
}

#endif

