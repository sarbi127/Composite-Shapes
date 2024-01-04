#include "gradient.hpp"

// Constructor.
qbRT::Texture::Gradient::Gradient()
{

}

// Destructor.
qbRT::Texture::Gradient::~Gradient()
{

}

// Function to return the color.
qbVector<double> qbRT::Texture::Gradient::GetColor(const qbVector<double> &uvCoords)
{
	// Apply the local transform to the (u,v) coordinates.
	qbVector<double> inputLoc = uvCoords;
	qbVector<double> newLoc = ApplyTransform(inputLoc);
	double newU = std::min((newLoc.GetElement(1) + 1.0) / 2.0, 1.0);
	return m_colorMap.GetColor(newU);
}

// Function to return the value.
double qbRT::Texture::Gradient::GetValue(const qbVector<double> &uvCoords)
{
	// Apply the local transform to the (u,v) coordinates.
	qbVector<double> inputLoc = uvCoords;
	qbVector<double> newLoc = ApplyTransform(inputLoc);
	return std::min((newLoc.GetElement(0) + 1.0) / 2.0, 1.0);	
}

// Function to set the stops for the color map
void qbRT::Texture::Gradient::SetStop(double position, const qbVector<double> &value)
{
	m_colorMap.SetStop(position, value);
}
