#include "materialbase.hpp"

// Constructor / destructor.
qbRT::MaterialBase::MaterialBase()
{
	m_maxReflectionRays = 3;
	m_reflectionRayCount = 0;
}

qbRT::MaterialBase::~MaterialBase()
{

}

// Function to compute the color of the material.
qbVector<double> qbRT::MaterialBase::ComputeColor(	const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
																										const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList,
																										const std::shared_ptr<qbRT::ObjectBase> &currentObject,
																										const qbVector<double> &intPoint, const qbVector<double> &localNormal,
																										const qbRT::Ray &cameraRay)
{
	// Define an initial material color.
	qbVector<double> matColor	{3};
	
	return matColor;
}

// Function to compute the diffuse color.
qbVector<double> qbRT::MaterialBase::ComputeDiffuseColor(	const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
																													const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList,
																													const std::shared_ptr<qbRT::ObjectBase> &currentObject,
																													const qbVector<double> &intPoint, const qbVector<double> &localNormal,
																													const qbVector<double> &baseColor)
{
	// Compute the color due to diffuse illumination.
	qbVector<double> diffuseColor	{3};
	double intensity;
	qbVector<double> color {3};
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;
	bool validIllum = false;
	bool illumFound = false;
	for (auto currentLight : lightList)
	{
		validIllum = currentLight -> ComputeIllumination(intPoint, localNormal, objectList, NULL, color, intensity);
		if (validIllum)
		{
			illumFound = true;
			red += color.GetElement(0) * intensity;
			green += color.GetElement(1) * intensity;
			blue += color.GetElement(2) * intensity;
		}
	}
	
	if (illumFound)
	{
		diffuseColor.SetElement(0, red * baseColor.GetElement(0));
		diffuseColor.SetElement(1, green * baseColor.GetElement(1));
		diffuseColor.SetElement(2, blue * baseColor.GetElement(2));
	}
	else
	{
		// The ambient light condition.
		for (int i=0; i<3; ++i)
			diffuseColor.SetElement(i, (m_ambientColor.GetElement(i) * m_ambientIntensity) * baseColor.GetElement(i));
	}
	
	// Return the material color.
	return diffuseColor;
	
}

// Function to compute the color due to reflection.
qbVector<double> qbRT::MaterialBase::ComputeReflectionColor(	const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
																															const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList,
																															const std::shared_ptr<qbRT::ObjectBase> &currentObject,
																															const qbVector<double> &intPoint, const qbVector<double> &localNormal,
																															const qbRT::Ray &incidentRay)
{
	qbVector<double> reflectionColor {3};
	
	// Compute the reflection vector.
	qbVector<double> d = incidentRay.m_lab;
	qbVector<double> reflectionVector = d - (2.0 * qbVector<double>::dot(d, localNormal) * localNormal);
	
	// Construct the reflection ray.
	qbVector<double> startPoint = intPoint + (localNormal * 0.001);
	qbRT::Ray reflectionRay (startPoint, startPoint + reflectionVector);
	
	/* Cast this ray into the scene and find the closest object that it intersects with. */
	std::shared_ptr<qbRT::ObjectBase> closestObject;
	qbRT::DATA::hitData closestHitData;
	bool intersectionFound = CastRay(reflectionRay, objectList, NULL, closestObject, closestHitData);
	
	/* Compute illumination for closest object assuming that there was a
		valid intersection. */
	qbVector<double> matColor	{3};
	if ((intersectionFound) && (m_reflectionRayCount < m_maxReflectionRays))
	{
		// Increment the reflectionRayCount.
		m_reflectionRayCount++;
		
		// Check if a material has been assigned.
		//if (closestObject -> m_hasMaterial)
		if (closestHitData.hitObject -> m_hasMaterial)
		{
            // Use the material to compute the color.
			/*
			matColor = closestObject -> m_pMaterial -> ComputeColor(objectList, lightList, 
																	closestObject, closestHitData.poi, 
																	closestHitData.normal, reflectionRay);
			*/
			matColor = closestHitData.hitObject -> m_pMaterial -> ComputeColor(	objectList, lightList, 
																				closestHitData.hitObject, closestHitData.poi, 
																				closestHitData.normal, reflectionRay);

		}
		else
		{
			/*
			matColor = qbRT::MaterialBase::ComputeDiffuseColor(	objectList, lightList, 
																closestObject, closestHitData.poi, 
																closestHitData.normal, closestObject->m_baseColor);
			*/
			matColor = qbRT::MaterialBase::ComputeDiffuseColor(	objectList, lightList, closestHitData.hitObject, 
																closestHitData.poi, closestHitData.normal, 
															    closestObject->m_baseColor);	

		}
	}
	else
	{
		// Leave matColor as it is.
	}
	
	reflectionColor = matColor;
	return reflectionColor;
}

// Function to cast a ray into the scene.
bool qbRT::MaterialBase::CastRay( const qbRT::Ray &castRay, const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
																	const std::shared_ptr<qbRT::ObjectBase> &thisObject,
																	std::shared_ptr<qbRT::ObjectBase> &closestObject,
																	qbRT::DATA::hitData &closestHitData)
{
	// Test for intersections with all of the objects in the scene.
	//qbVector<double> intPoint			{3};
	//qbVector<double> localNormal	{3};
	//qbVector<double> localColor		{3};
	qbRT::DATA::hitData hitData;
	
	double minDist = 1e6;
	bool intersectionFound = false;
	for (auto currentObject : objectList)
	{
		if (currentObject != thisObject)
		{
			bool validInt = currentObject -> TestIntersection(castRay, hitData);
			
			// If we have a valid intersection.
			if (validInt)
			{
				// Set the flag to show that we found an intersection.
				intersectionFound = true;
				
				// Compute the distance between the source and the intersection point.
				double dist = (hitData.poi - castRay.m_point1).norm();
				
				// Store a reference to this object if it is the closest.
				if (dist < minDist)
				{
					minDist = dist;
					closestObject = currentObject;
					//closestIntPoint = intPoint;
					//closestLocalNormal = localNormal;
					//closestLocalColor = localColor;
					closestHitData = hitData;
				}
			}
		}
	}
	
	return intersectionFound;
}

// Function to assign a texture.
void qbRT::MaterialBase::AssignTexture(const std::shared_ptr<qbRT::Texture::TextureBase> &inputTexture)
{
	m_textureList.push_back(inputTexture);
	m_hasTexture = true;
}

// *** Function to assign a normal map.
void qbRT::MaterialBase::AssignNormalMap(const std::shared_ptr<qbRT::Normal::NormalBase> &inputNormalMap)
{
	m_normalMapList.push_back(inputNormalMap);
	m_hasNormalMap = true;
}

// Function to return the color due to textures at the given (u,v) coordinate.
qbVector<double> qbRT::MaterialBase::GetTextureColor(const qbVector<double> &uvCoords)
{
	qbVector<double> outputColor (4);
	
	if (m_textureList.size() > 1)
	{
		outputColor = m_textureList.at(0)->GetColor(uvCoords);
		for (int i=1; i<m_textureList.size(); ++i)
		{
			BlendColors(outputColor, m_textureList.at(i)->GetColor(uvCoords));
		}
	}
	else
	{
		outputColor = m_textureList.at(0)->GetColor(uvCoords);
	}
	
	return outputColor;
}

// Function to blend colors.
void qbRT::MaterialBase::BlendColors(qbVector<double> &color1, const qbVector<double> &color2)
{
	color1 = (color2 * color2.GetElement(3)) + (color1 * (1.0 - color2.GetElement(3)));
}

// *** Function to perturb the object normal to give the material normal.
qbVector<double> qbRT::MaterialBase::PerturbNormal(const qbVector<double> &normal, const qbVector<double> &uvCoords, const qbVector<double> &upVector)
{
	// Copy the original normal.
	qbVector<double> newNormal = normal;
	
	// Perturb the new normal with each normal map in turn.
	for (int i=0; i<m_normalMapList.size(); ++i)
	{
		newNormal = m_normalMapList.at(i) -> ComputePerturbation(newNormal, uvCoords);
	}

	// And return the output.
	return newNormal;
}
