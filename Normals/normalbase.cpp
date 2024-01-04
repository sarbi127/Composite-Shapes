#include "normalbase.hpp"

// Constructor / destructor.
qbRT::Normal::NormalBase::NormalBase()
{

}

qbRT::Normal::NormalBase::~NormalBase()
{

}

qbVector<double> qbRT::Normal::NormalBase::ComputePerturbation(const qbVector<double> &normal, const qbVector<double> &uvCoords)
{
	// The default response.
	return qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}};
}

qbVector<double> qbRT::Normal::NormalBase::PerturbNormal(const qbVector<double> &normal, const qbVector<double> &perturbation)
{
	// Decide upon an appropriate up vector.
	qbVector<double> newUpVector = std::vector<double> {0.0, 0.0, -1.0};
	if ((normal.GetElement(2) > 0.99) || (normal.GetElement(2) < -0.99))
		newUpVector = std::vector<double> {1.0, 0.0, 0.0};

	// Compute the directions (based on the tangent plane).
	qbVector<double> pV = qbVector<double>::cross(newUpVector, normal);
	pV.Normalize();
	qbVector<double> pU = qbVector<double>::cross(normal, pV);
	pU.Normalize();
	
	// Apply the perturbation.
	qbVector<double> output = normal + (pU * perturbation.GetElement(0)) + (pV * perturbation.GetElement(1)) + (normal * perturbation.GetElement(2));
	
	// Normalize the output.
	output.Normalize();
	
	// And return.
	return output;
}

void qbRT::Normal::NormalBase::SetAmplitude(double amplitude)
{
	m_amplitudeScale = amplitude;
}

// Function to perform numerical differentiation of a texture in UV space.
qbVector<double> qbRT::Normal::NormalBase::TextureDiff(const std::shared_ptr<qbRT::Texture::TextureBase> &inputTexture, const qbVector<double> &uvCoords)
{
	// We will use the symmetric difference quotient to estimate the partial derivatives of the texture
	// at the point f(u,v).
	// uGrad = f(u+h, v) - f(u-h, v) / 2h
	// vGrad = f(u, v+h) - f(u, v-h) / 2h
	double h = 0.001;
	qbVector<double> uDisp = std::vector<double> {h, 0.0};
	qbVector<double> vDisp = std::vector<double> {0.0, h};
	double uGrad = (inputTexture->GetValue(uvCoords + uDisp) - inputTexture->GetValue(uvCoords - uDisp)) / (2.0 * h);
	double vGrad = (inputTexture->GetValue(uvCoords + vDisp) - inputTexture->GetValue(uvCoords - vDisp)) / (2.0 * h);
	
	/* Form a vector for the output. 
	*/
	qbVector<double> output = std::vector<double> {uGrad, vGrad};
	return output;
}

// Function to apply the transform.
qbVector<double> qbRT::Normal::NormalBase::ApplyTransform(const qbVector<double> &inputVector)
{
	// Copy the input vector and modify to have three elements.
	qbVector<double> newInput {3};
	newInput.SetElement(0, inputVector.GetElement(0));
	newInput.SetElement(1, inputVector.GetElement(1));
	
	// Apply the transform.
	qbVector<double> result = m_transformMatrix * newInput;
	
	// Produce the output.
	qbVector<double> output {2};
	output.SetElement(0, result.GetElement(0));
	output.SetElement(1, result.GetElement(1));
	
	return output;
}

// Function to set the transform matrix.
void qbRT::Normal::NormalBase::SetTransform(const qbVector<double> &translation, const double &rotation, const qbVector<double> &scale)
{
	// Build the transform matrix.
	qbMatrix2<double> rotationMatrix = {3, 3, std::vector<double> {
																			cos(rotation), -sin(rotation), 0.0,
																			sin(rotation), cos(rotation), 0.0,
																			0.0, 0.0, 1.0}};
																			
	qbMatrix2<double> scaleMatrix = {	3, 3, std::vector<double> {
																		scale.GetElement(0), 0.0, 0.0,
																		0.0, scale.GetElement(1), 0.0,
																		0.0, 0.0, 1.0}};
																		
	qbMatrix2<double> translationMatrix = {	3, 3, std::vector<double> {
																					1.0, 0.0, translation.GetElement(0),
																					0.0, 1.0, translation.GetElement(1),
																					0.0, 0.0, 1.0}};
																					
	// And combine to form the final transform matrix.
	m_transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}
