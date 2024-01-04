
#include "cylinder.hpp"
#include <cmath>

// The default constructor.
qbRT::Cylinder::Cylinder()
{
	// Define the default uv mapping.
	m_uvMapType = qbRT::uvCYLINDER;
	
	// Construct the default bounding box.
	m_boundingBoxTransform.SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
										qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
										qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}});
}

// The destructor.
qbRT::Cylinder::~Cylinder()
{

}

// The function to test for intersections.
bool qbRT::Cylinder::TestIntersection(	const qbRT::Ray &castRay, qbRT::DATA::hitData &hitData)
{
	if (!m_isVisible)
		return false;
		
	// Copy the ray and apply the backwards transform.
	qbRT::Ray bckRay = m_transformMatrix.Apply(castRay, qbRT::BCKTFORM);
	
	// Copy the m_lab vector from bckRay and normalize it.
	qbVector<double> v = bckRay.m_lab;
	v.Normalize();
	
	// Get the start point of the line.
	qbVector<double> p = bckRay.m_point1;
	
	// Compute a, b and c.
	double a = std::pow(v.GetElement(0), 2.0) + std::pow(v.GetElement(1), 2.0);
	double b = 2.0 * (p.GetElement(0) * v.GetElement(0) + p.GetElement(1) * v.GetElement(1));
	double c = std::pow(p.GetElement(0), 2.0) + std::pow(p.GetElement(1), 2.0) - 1.0;
	
	// Compute b^2 - 4ac.
	double numSQRT = sqrtf(std::pow(b, 2.0) - 4 * a * c);
	
	// Test for intersections.
	// First with the cylinder itself.
	std::array<qbVector<double>, 4> poi;
	std::array<double, 4> t;
	bool t1Valid, t2Valid, t3Valid, t4Valid;
	if (numSQRT > 0.0)
	{
		// There was an intersection.
		// Compute the values for t.
		t.at(0) = (-b + numSQRT) / (2 * a);
		t.at(1) = (-b - numSQRT) / (2 * a);
		
		// Compute the points of intersection.
		poi.at(0) = bckRay.m_point1 + (v * t[0]);
		poi.at(1) = bckRay.m_point1 + (v * t[1]);
		
		// Check if any of these are valid.
		if ((t.at(0) > 0.0) && (fabs(poi.at(0).GetElement(2)) < 1.0))
		{
			t1Valid = true;
		}
		else
		{
			t1Valid = false;
			t.at(0) = 100e6;
		}
		
		if ((t.at(1) > 0.0) && (fabs(poi.at(1).GetElement(2)) < 1.0))
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
	
	// And test the end caps.
	if (CloseEnough(v.GetElement(2), 0.0))
	{
		t3Valid = false;
		t4Valid = false;
		t.at(2) = 100e6;
		t.at(3) = 100e6;
	}
	else
	{
		// Compute the values of t.
		t.at(2) = (bckRay.m_point1.GetElement(2) - 1.0) / -v.GetElement(2);
		t.at(3) = (bckRay.m_point1.GetElement(2) + 1.0) / -v.GetElement(2);
		
		// Compute the points of intersection.
		poi.at(2) = bckRay.m_point1 + t.at(2) * v;
		poi.at(3) = bckRay.m_point1 + t.at(3) * v;
		
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
		
		if ((t.at(3) > 0.0) && (sqrtf(std::pow(poi.at(3).GetElement(0), 2.0) + std::pow(poi.at(3).GetElement(1), 2.0)) < 1.0))
		{
			t4Valid = true;
		}
		else
		{
			t4Valid = false;
			t.at(3) = 100e6;
		}
	}
	
	// If no valid intersections found, the we can stop.
	if ((!t1Valid) && (!t2Valid) && (!t3Valid) && (!t4Valid))
		return false;
		
	// Check for the smallest valid value of t.
	int minIndex = 0;
	double minValue = 10e6;
	for (int i=0; i<4; ++i)
	{
		if (t.at(i) < minValue)
		{
			minValue = t.at(i);
			minIndex = i;
		}
	}
	
	/* If minIndex is either 0 or 1, then we have a valid intersection
		with the cylinder itself. */
	qbVector<double> validPOI = poi.at(minIndex);
	if (minIndex < 2)
	{
		// Transform the intersection point back into world coordinates.
		hitData.poi = m_transformMatrix.Apply(validPOI, qbRT::FWDTFORM);
		
		// Compute the local normal.
		qbVector<double> orgNormal {3};
		qbVector<double> newNormal {3};
		qbVector<double> localOrigin {std::vector<double> {0.0, 0.0, 0.0}};
		qbVector<double> globalOrigin = m_transformMatrix.Apply(localOrigin, qbRT::FWDTFORM);
		orgNormal.SetElement(0, validPOI.GetElement(0));
		orgNormal.SetElement(1, validPOI.GetElement(1));
		orgNormal.SetElement(2, 0.0);
		hitData.normal = m_transformMatrix.ApplyNorm(orgNormal);
		hitData.normal.Normalize();
				
		//newNormal = m_transformMatrix.Apply(orgNormal, qbRT::FWDTFORM) - globalOrigin;
		//newNormal.Normalize();
		//localNormal = newNormal;		
		
		// Return the base color.
		hitData.color = m_baseColor;
		
		// Compute the (u,v) coordinates and store for possible later use.
		//double x = validPOI.GetElement(0);
		//double y = validPOI.GetElement(1);
		//double z = validPOI.GetElement(2);
		//double u = atan2(y, x) / M_PI;
		//double v = z;
		//m_uvCoords.SetElement(0, u);
		//m_uvCoords.SetElement(1, v);
		ComputeUV(validPOI, m_uvCoords);
		hitData.uvCoords = m_uvCoords;
		
		// Return a reference to this object.
		//hitData.hitObject = std::make_shared<qbRT::ObjectBase> (*this);			
		hitData.hitObject = this -> shared_from_this();
		
		return true;
	}
	else
	{
		// Otherwise check the end caps.
		if (!CloseEnough(v.GetElement(2), 0.0))
		{
			// Check if we are inside the disk.
			if (sqrtf(std::pow(validPOI.GetElement(0), 2.0) + std::pow(validPOI.GetElement(1), 2.0)) < 1.0)
			{
				// Transform the intersection point back into world coordinates.
				hitData.poi = m_transformMatrix.Apply(validPOI, qbRT::FWDTFORM);
				
				// Compute the local normal.
				//qbVector<double> localOrigin {std::vector<double> {0.0, 0.0, 0.0}};
				//qbVector<double> globalOrigin = m_transformMatrix.Apply(localOrigin, qbRT::FWDTFORM);
				//localNormal = m_transformMatrix.Apply(normalVector, qbRT::FWDTFORM) - globalOrigin;
				//localNormal.Normalize();
				
				qbVector<double> normalVector {std::vector<double> {0.0, 0.0, 0.0 + validPOI.GetElement(2)}};
				hitData.normal = m_transformMatrix.ApplyNorm(normalVector);
				hitData.normal.Normalize();
				
				// Return the base color.
				hitData.color = m_baseColor;
				
				// Compute and store (u,v) coordinates for possible later use.
				//double x = validPOI.GetElement(0);
				//double y = validPOI.GetElement(1);
				//double z = validPOI.GetElement(2);
				//m_uvCoords.SetElement(0, x);
				//m_uvCoords.SetElement(1, y);
				ComputeUV(validPOI, m_uvCoords);
				hitData.uvCoords = m_uvCoords;
				
				// Return a reference to this object.
				//hitData.hitObject = std::make_shared<qbRT::ObjectBase> (*this);	
				hitData.hitObject = this -> shared_from_this();
				
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
