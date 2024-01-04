
#include "objplane.hpp"
#include <cmath>

qbRT::ObjPlane::ObjPlane ()
{

}
   
qbRT:: ObjPlane ::~ObjPlane ()
{

}

// The function to test for intersections.
bool qbRT::ObjPlane::TestIntersection(	const qbRT::Ray &castRay, qbRT::DATA::hitData &hitData)
{
	// Copy the ray and apply the backwards transform.
	qbRT::Ray bckRay = m_transformMatrix.Apply(castRay, qbRT::BCKTFORM);
	
	// Copy the m_lab vector from bckRay and normalize it.
	qbVector<double> k = bckRay.m_lab;
	k.Normalize();
	
	/* Check if there is an intersection, ie. if the castRay is not parallel
		to the plane. */
	if (!CloseEnough(k.GetElement(2), 0.0))
	{
		// There is an intersection.
		double t = bckRay.m_point1.GetElement(2) / -k.GetElement(2);
		
		/* If t is negative, then the intersection point must be behind
			the camera and we can ignore it. */
		if (t > 0.0)
		{
			// Compute the values for u and v.
			double u = bckRay.m_point1.GetElement(0) + (k.GetElement(0) * t);
			double v = bckRay.m_point1.GetElement(1) + (k.GetElement(1) * t);
			
			/* If the magnitude of both u and v is less than or equal to one
				then we must be in the plane. */
			if ((abs(u) < 1.0) && (abs(v) < 1.0))
			{
				// Compute the point of intersection.
				qbVector<double> poi = bckRay.m_point1 + t * k;
				
				// Transform the intersection point back into world coordinates.
				hitData.poi = m_transformMatrix.Apply(poi, qbRT::FWDTFORM);
								
				qbVector<double> normalVector {std::vector<double> {0.0, 0.0, -1.0}};
				hitData.normal = m_transformMatrix.ApplyNorm(normalVector);
				hitData.normal.Normalize();
				
				// Return the base color.
				hitData.color = m_baseColor;
				
				// Return the local point of intersection.
				hitData.localPOI = poi;				
				
				// Compute the UV coordinates.
				//m_uvCoords.SetElement(0, u);
				//m_uvCoords.SetElement(1, v);
				ComputeUV(poi, hitData.uvCoords);
				hitData.uvCoords = m_uvCoords;
				
				// Return a reference to this object.
				hitData.hitObject = std::make_shared<qbRT::ObjectBase> (*this);
				
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	
	return false;
}


