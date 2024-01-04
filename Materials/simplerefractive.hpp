#ifndef SIMPLEREFRACTIVE_H
#define SIMPLEREFRACTIVE_H

#include "materialbase.hpp"

namespace qbRT
{
     class SimpleRefractive : public MaterialBase
     {

            public:
			// Constructor / destructor.
			SimpleRefractive();
			virtual ~SimpleRefractive() override;
			
			// Function to return the color.
			virtual qbVector<double> ComputeColor(	const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
																							const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList,
																							const std::shared_ptr<qbRT::ObjectBase> &currentObject,
																							const qbVector<double> &intPoint, const qbVector<double> &localNormal,
																							const qbRT::Ray &cameraRay) override;
																							
			// Function to compute specular highlights.
			qbVector<double> ComputeSpecular(	const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
																				const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList,
																				const qbVector<double> &intPoint, const qbVector<double> &localNormal,
																				const qbRT::Ray &cameraRay);
																				
		 	// Function to compute translucency.
		 	qbVector<double> ComputeTranslucency(	const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
																						const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList,
																						const std::shared_ptr<qbRT::ObjectBase> &currentObject,
																						const qbVector<double> &intPoint, const qbVector<double> &localNormal,
																						const qbRT::Ray &incidentRay);
																						
		public:
			qbVector<double> m_baseColor {std::vector<double> {1.0, 0.0, 1.0}};
			double m_reflectivity = 0.0;
			double m_shininess = 0.0;
			double m_translucency = 0.0;
			double m_ior = 1.0;
																						
	};

}

#endif