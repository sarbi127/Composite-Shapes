#include "sdfunc.hpp"

// Sphere
double qbRT::RM::SDF::Sphere(const qbVector<double> &p, const qbVector<double> &center, const qbVector<double> &parms)
{
	return (p - center).norm() - parms.GetElement(0);
}

// Torus
double qbRT::RM::SDF::Torus(const qbVector<double> &p, const qbVector<double> &center, const qbVector<double> &parms)
{
	double x = p.GetElement(0) - center.GetElement(0);
	double y = p.GetElement(1) - center.GetElement(1);
	double z = p.GetElement(2) - center.GetElement(2);
	
	double t1 = sqrtf((x*x) + (y*y)) - parms.GetElement(0);
	double t2 = sqrtf((t1*t1) + (z*z)) - parms.GetElement(1);
	
	return t2;
}

// Box
double qbRT::RM::SDF::Box(const qbVector<double> &p, const qbVector<double> &center, const qbVector<double> &parms)
{
	qbVector<double> location = (p - center);
	double ax = fabs(location.GetElement(0)) - parms.GetElement(0);
	double ay = fabs(location.GetElement(1)) - parms.GetElement(1);
	double az = fabs(location.GetElement(2)) - parms.GetElement(2);
	
	double bx = std::max(ax, 0.0);
	double by = std::max(ay, 0.0);
	double bz = std::max(az, 0.0);
	
	double internalDist = std::min(std::max(ax, std::max(ay, az)), 0.0);
	double externalDist = sqrt((bx*bx)+(by*by)+(bz*bz));
	
	return internalDist + externalDist;
}


