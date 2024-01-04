#include "raymarchbase.hpp"
#include "../qbutils.hpp"

// Constructor.
qbRT::RM::RayMarchBase::RayMarchBase()
{
	// Define the tolerance.
	m_epsilon = 1e-3;
	
	// Define the maximum number of steps allowed.
	m_maxSteps = 100;
}

// Destructor.
qbRT::RM::RayMarchBase::~RayMarchBase()
{

}

// Test for intersections.
bool qbRT::RM::RayMarchBase::TestIntersection(	const qbRT::Ray &castRay, qbRT::DATA::hitData &hitData	)
{
	// Check if an object function has been defined.
	if (m_haveObjectFcn)
	{
		// Check if the object is visible.
		if (!m_isVisible)
			return false;
			
		// Copy the ray and apply the backwards transform.
		qbRT::Ray bckRay = m_transformMatrix.Apply(castRay, qbRT::BCKTFORM);
		
		// Test for intersections with the bounding box.
		//qbVector<double> boundPOI			{3};
		//qbVector<double> boundNormal	{3};
		//qbVector<double> boundColor		{3};
		qbRT::DATA::hitData boundHit;
		if (m_boundingBox.TestIntersection(bckRay, boundHit))
		{
			// Extract ray direction.
			qbVector<double> vhat = bckRay.m_lab;
			vhat.Normalize();		
		
			qbVector<double> currentLoc = bckRay.m_point1;
			int stepCount = 0;
			double dist = EvaluateSDF(&currentLoc, &m_parms);
			
			// Main loop
			while ((dist > m_epsilon) && (stepCount < m_maxSteps))
			{
				currentLoc = currentLoc + (vhat * dist);
				
				dist = EvaluateSDF(&currentLoc, &m_parms);
				if (dist > 1e6)
				{
					stepCount = m_maxSteps;
					break;
				}
				
				stepCount++;
			}
			
			// If m_maxSteps exceeded, then no valid intersection found.
			if (stepCount == m_maxSteps)
			{
				return false;
			}
		
			// Otherwise, we have a valid intersection at currentLoc.
			// Transform the intersection point back into world coordinates.
			hitData.poi = m_transformMatrix.Apply(currentLoc, qbRT::FWDTFORM);
			
			// Compute the local normal.
			qbVector<double> surfaceNormal {3};

			/*
			 Note the extra code here to compute an offset location from which
			to compute the normal. The reason to do this is that in some cases,
			especially with more complex distance functions, the point of intersection
			can be computed as being very slightly inside the surface. This would
			obviously result in a normal vector pointing in the wrong direction.
			By tracing back along the intersecting ray a short distance and then using
			that location instead, we can avoid this problem.
			*/

			// Determine an offset point.
			qbVector<double> normalLoc = currentLoc - (vhat * 0.01);
			
			qbVector<double> x1 = normalLoc - m_xDisp;
			qbVector<double> x2 = normalLoc + m_xDisp;
			qbVector<double> y1 = normalLoc - m_yDisp;
			qbVector<double> y2 = normalLoc + m_yDisp;
			qbVector<double> z1 = normalLoc - m_zDisp;
			qbVector<double> z2 = normalLoc + m_zDisp;
			surfaceNormal.SetElement(0, EvaluateSDF(&x2, &m_parms) - EvaluateSDF(&x1, &m_parms));
			surfaceNormal.SetElement(1, EvaluateSDF(&y2, &m_parms) - EvaluateSDF(&y1, &m_parms));
			surfaceNormal.SetElement(2, EvaluateSDF(&z2, &m_parms) - EvaluateSDF(&z1, &m_parms));
		
			// Transform the local normal.
			surfaceNormal.Normalize();
			hitData.normal = m_transformMatrix.ApplyNorm(surfaceNormal);
			hitData.normal.Normalize();
			
			// Return the base color.
			hitData.color = m_baseColor;
			
			// Return a pointer to this object.
			hitData.hitObject = std::make_shared<qbRT::ObjectBase> (*this);			
			
			// Return the local point of intersection.
			hitData.localPOI = currentLoc;			
			
			// Compute UV.		
			ComputeUV(currentLoc, m_uvCoords);
			hitData.uvCoords = m_uvCoords;				
		
			return true;
		}
	
	}
	
	return false;
}

// Function to set the object function.
void qbRT::RM::RayMarchBase::SetObjectFcn( std::function<double(qbVector<double>*, qbVector<double>*)> objectFcn )
{
	m_objectFcn = objectFcn;
	m_haveObjectFcn = true;
}

// Function to evaluate the Signed Distance Function (SDF) at the given coordinates.
double qbRT::RM::RayMarchBase::EvaluateSDF(	qbVector<double> *location, qbVector<double> *parms )
{
	return m_objectFcn(location, parms);
}

