#include "constant.hpp"

// Constructor / destructor.
qbRT::Normal::Constant::Constant()
{

}

qbRT::Normal::Constant::~Constant()
{

}

qbVector<double> qbRT::Normal::Constant::ComputePerturbation(const qbVector<double> &normal, const qbVector<double> &uvCoords)
{
	return PerturbNormal(normal, m_displacement);
}


