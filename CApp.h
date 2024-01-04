#ifndef CAPP_H
#define CAPP_H

#include <SDL2/SDL.h>
#include "qbImage.hpp"
#include "Scene.hpp"
#include "camera.hpp"
#include "./Textures/flat.hpp"
#include "./Textures/checker.hpp"
#include "./Textures/image.hpp"

//const int WIDTH = 1200, HEIGHT = 700;

class CApp
{
	public:
		CApp();
		
		int OnExecute();
		bool OnInit();
		void OnEvent(SDL_Event *event);
		void OnLoop();
		void OnRender();
		void OnExit();
	
	private:
		void PrintVector(const qbVector<double> &inputVector);
		
	private:
		// An instance of qbImage to store the image.
		qbImage m_image;

		// An instance of the scene class.
		qbRT::Scene m_scene;
	
		// SDL2 stuff.
		bool isRunning;
		SDL_Window *pWindow;
		SDL_Renderer *pRenderer;

};

#endif
