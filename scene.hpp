#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include "qbImage.hpp"
#include "camera.hpp"
#include "./Primatives/objsphere.hpp"
#include "./Primatives/objplane.hpp"
#include "./Primatives/cylinder.hpp"
#include "./Primatives/cone.hpp"
#include "./Primatives/box.hpp"
#include "./Lights/pointlight.hpp"
#include "./RayMarch/sphere.hpp"
#include "./RayMarch/torus.hpp"
#include "./RayMarch/cube.hpp"
#include "./Normals/simplerough.hpp"
#include "./Normals/texturenormal.hpp"
#include "./Normals/image.hpp"

namespace qbRT
{
	class Scene
	{
		public:
			// The default constructor.
			Scene();		
			
			// Function to perform the rendering.
			bool Render(qbImage &outputImage);

			// Function to cast a ray into the scene.
			bool CastRay(	qbRT::Ray &castRay, std::shared_ptr<qbRT::ObjectBase> &closestObject,
										qbRT::DATA::hitData &closestHitData);
			
		// Private functions.
		private:
		
		// Private members.
		private:
		    qbRT::Camera m_camera;
			
			// The list of object in the scene.
			std::vector<std::shared_ptr<qbRT::ObjectBase>> m_objectList;

			// The list of lights in the scene.
			std::vector<std::shared_ptr<qbRT::LightBase>> m_lightList;
			
	};
}

#endif

