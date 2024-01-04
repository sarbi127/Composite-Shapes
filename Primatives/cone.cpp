#include "cone.hpp"
#include <cmath>

// The default constructor.
qbRT::Cone::Cone()
{

}

// The destructor.
qbRT::Cone::~Cone()
{

}

// The function to test for intersections.
bool qbRT::Cone::TestIntersection(	const qbRT::Ray &castRay, qbRT::DATA::hitData &hitData)
{
	// Copy the ray and apply the backwards transform.
	qbRT::Ray bckRay = m_transformMatrix.Apply(castRay, qbRT::BCKTFORM);
	
	// Copy the m_lab vector from bckRay and normalize it.
	qbVector<double> v = bckRay.m_lab;
	v.Normalize();
	
	// Get the start point of the line.
	qbVector<double> p = bckRay.m_point1;
	
	// Compute a, b and c.
	double a = std::pow(v.GetElement(0), 2.0) + std::pow(v.GetElement(1), 2.0) - std::pow(v.GetElement(2), 2.0);
	double b = 2 * (p.GetElement(0)*v.GetElement(0) + p.GetElement(1)*v.GetElement(1) - p.GetElement(2)*v.GetElement(2));
	double c = std::pow(p.GetElement(0), 2.0) + std::pow(p.GetElement(1), 2.0) - std::pow(p.GetElement(2), 2.0);
	
	// Compute b^2 - 4ac.
	double numSQRT = sqrtf(std::pow(b, 2.0) - 4 * a * c);
	
	std::array<qbVector<double>, 3> poi;
	std::array<double, 3> t;
	bool t1Valid, t2Valid, t3Valid;
	if (numSQRT > 0.0)
	{
		// Compute the values of t.
		t.at(0) = (-b + numSQRT) / (2 * a);
		t.at(1) = (-b - numSQRT) / (2 * a);
		
		// Compute the points of intersection.
		poi.at(0) = bckRay.m_point1 + (v * t[0]);
		poi.at(1) = bckRay.m_point1 + (v * t[1]);
		
		if ((t.at(0) > 0.0) && (poi.at(0).GetElement(2) > 0.0) && (poi.at(0).GetElement(2) < 1.0))
		{
			t1Valid = true;
		}
		else
		{
			t1Valid = false;
			t.at(0) = 100e6;
		}
		
		if ((t.at(1) > 0.0) && (poi.at(1).GetElement(2) > 0.0) && (poi.at(1).GetElement(2) < 1.0))
		{
			t2Valid = true;
		}
		else
		{
			t2Valid = false;
			t.at(1) = 100e6;
		}
	}
	else
	{
		t1Valid = false;
		t2Valid = false;
		t.at(0) = 100e6;
		t.at(1) = 100e6;
	}
	
	// And test the end cap.
	if (CloseEnough(v.GetElement(2), 0.0))
	{
		t3Valid = false;
		t.at(2) = 100e6;
	}
	else
	{	
		// Compute values for t.(on z)
		t.at(2) = (bckRay.m_point1.GetElement(2) - 1.0) / -v.GetElement(2);
		
		// Compute points of intersection.
		poi.at(2) = bckRay.m_point1 + t.at(2) * v;
		
		// Check if these are valid.
		if ((t.at(2) > 0.0) && (sqrtf(std::pow(poi.at(2).GetElement(0), 2.0) + std::pow(poi.at(2).GetElement(1), 2.0)) < 1.0))
		{
			t3Valid = true;
		}
		else
		{
			t3Valid = false;
			t.at(2) = 100e6;
		}						
	}
	
	// If no valid intersections found, then we can stop.
	if ((!t1Valid) && (!t2Valid) && (!t3Valid))
		return false;	
		
	// Check for the smallest valid value of t.
	int minIndex = 0;
	double minValue = 10e6;
	for (int i=0; i<3; ++i)
	{
		if (t.at(i) < minValue)
		{
			minValue = t.at(i);
			minIndex = i;
		}
	}
	
	/* If minIndex is either 0 or 1, then we have a valid intersection
		with the cone itself. */
	qbVector<double> validPOI = poi.at(minIndex);
	if (minIndex < 2)
	{		
		// Transform the intersection point back into world coordinates.
		//intPoint = m_transformMatrix.Apply(validPOI, qbRT::FWDTFORM);		
		hitData.poi = m_transformMatrix.Apply(validPOI, qbRT::FWDTFORM);

		// Compute the local normal.
		qbVector<double> orgNormal {3};
		qbVector<double> newNormal {3};
		qbVector<double> localOrigin {std::vector<double> {0.0, 0.0, 0.0}};
		qbVector<double> globalOrigin = m_transformMatrix.Apply(localOrigin, qbRT::FWDTFORM);		
		
		double tX = validPOI.GetElement(0);
		double tY = validPOI.GetElement(1);
		double tZ = -sqrtf(pow(tX, 2.0) + pow(tY, 2.0));
		
		orgNormal.SetElement(0, tX);
		orgNormal.SetElement(1, tY);
		orgNormal.SetElement(2, tZ);
		
		//orgNormal.Normalize();
		//qbVector<double> normalPoint = validPOI + orgNormal;
		//newNormal = m_transformMatrix.Apply(orgNormal, qbRT::FWDTFORM) - globalOrigin;
		//newNormal = m_transformMatrix.Apply(normalPoint, qbRT::FWDTFORM) - intPoint;
		//newNormal.Normalize();		
		//localNormal = newNormal;

		hitData.normal = m_transformMatrix.ApplyNorm(orgNormal);
		hitData.normal.Normalize();	
			
		// Return the base color.
		hitData.color = m_baseColor;
		
		// Return the local point of intersection.
		hitData.localPOI = validPOI;		
		
		ComputeUV(validPOI, hitData.uvCoords);
		hitData.uvCoords = m_uvCoords;
		
		// Return a reference to this object.	
		hitData.hitObject = std::make_shared<qbRT::ObjectBase> (*this);
	
		return true;
	}
	else
	{
		// Check the end cap.
		if (!CloseEnough(v.GetElement(2), 0.0))
		{
			// Check if we are inside the disk.
			if (sqrtf(std::pow(validPOI.GetElement(0), 2.0) + std::pow(validPOI.GetElement(1), 2.0)) < 1.0)
			{
				// Transform the intersection point back into world coordinates.
				hitData.poi = m_transformMatrix.Apply(validPOI, qbRT::FWDTFORM);				
				
				// Compute the local normal.
				//qbVector<double> normalVector {0.0, 0.0, 1.0};
				qbVector<double> normalVector {std::vector<double> {0.0, 0.0, 1.0}};
				hitData.normal = m_transformMatrix.ApplyNorm(normalVector);
				hitData.normal.Normalize();
						
				// Return the base color.
				hitData.color = m_baseColor;
				
				// Return the local point of intersection.
				hitData.localPOI = validPOI;				
				
				// Compute the (u,v) coordinates.
				ComputeUV(validPOI, hitData.uvCoords);
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
