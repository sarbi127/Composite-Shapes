#include "simplerough.hpp"

// Constructor / destructor.
qbRT::Normal::SimpleRough::SimpleRough()
{
	std::random_device randDev;
	std::seed_seq seed{randDev(), randDev(), randDev(), randDev()};
	m_p_randGen = std::make_shared<std::mt19937> (seed);	
}

qbRT::Normal::SimpleRough::~SimpleRough()
{

}

qbVector<double> qbRT::Normal::SimpleRough::ComputePerturbation(const qbVector<double> &normal, const qbVector<double> &uvCoords)
{
	std::uniform_real_distribution<double> randomDist (-m_amplitudeScale, m_amplitudeScale);
	double x = randomDist(*m_p_randGen);
	double y = randomDist(*m_p_randGen);
	double z = randomDist(*m_p_randGen);
	
	//double x = uvCoords.GetElement(0) * 0.5;
	//double y = 0.0;
	//double z = 0.0;
	
	qbVector<double> perturbation = std::vector<double> {x, y, z};
	return PerturbNormal(normal, perturbation);
}


