#include "qbImage.hpp"
#include <fstream>

// The default constructor.
qbImage::qbImage()
{
	m_xSize = 0;
	m_ySize = 0;
	m_pTexture = NULL;
}

// The destructor.
qbImage::~qbImage()
{
	if (m_pTexture != NULL)
		SDL_DestroyTexture(m_pTexture);
}

// Function to initialize.
void qbImage::Initialize(const int xSize, const int ySize, SDL_Renderer* pRenderer)
{
	// Resize image data arrays.
	m_rChannel.resize(xSize, std::vector<double>(ySize, 0));
	m_gChannel.resize(xSize, std::vector<double>(ySize, 0));
	m_bChannel.resize(xSize, std::vector<double>(ySize, 0));
	
	// Store the dimensions.
	m_xSize = xSize;
	m_ySize = ySize;
	
	// Store the pointer to the renderer.
	m_pRenderer = pRenderer;
	
	// Initialise the texture.
	InitTexture();	
}

// Function to set pixels.
void qbImage::SetPixel(const int x, const int y, const double red, const double green, const double blue)
{
	m_rChannel.at(x).at(y) = red;
	m_gChannel.at(x).at(y) = green;
	m_bChannel.at(x).at(y) = blue;
}

// Functions to return the dimensions of the image.
int qbImage::GetXSize()
{
	return m_xSize;
}
int qbImage::GetYSize()
{
	return m_ySize;
}

// Function to generate the display.
void qbImage::Display()
{
	// Compute maximum values.
	ComputeMaxValues();

	// Allocate memory for a pixel buffer.
	Uint32 *tempPixels = new Uint32[m_xSize * m_ySize];
	
	// Clear the pixel buffer.
	memset(tempPixels, 0, m_xSize * m_ySize * sizeof(Uint32));
	
	for (int x=0; x<m_xSize; ++x)
	{
		for (int y=0; y<m_ySize; ++y)
		{
			tempPixels[(y*m_xSize)+x] = ConvertColor(m_rChannel.at(x).at(y), m_gChannel.at(x).at(y), m_bChannel.at(x).at(y));
		}
	}
	
	// Update the texture with the pixel buffer.
	SDL_UpdateTexture(m_pTexture, NULL, tempPixels, m_xSize * sizeof(Uint32));	
	
	// Destroy the pixel buffer.
	delete[] tempPixels;
	
	// Copy the texture to the renderer.
	SDL_Rect srcRect, bounds;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = m_xSize;
	srcRect.h = m_ySize;
	bounds = srcRect;
	SDL_RenderCopy(m_pRenderer, m_pTexture, &srcRect, &bounds);	
}

// Function to return the image as an SDL2 texture.
void qbImage::InitTexture()
{
	// Initialise the texture.
	Uint32 rmask, gmask, bmask, amask;
	
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
	#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
	#endif
	
	// Delete any previously created texture before we create a new one.
	if (m_pTexture != NULL)
		SDL_DestroyTexture(m_pTexture);
	
	// Create the texture that will store the image.
	SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, m_xSize, m_ySize, 32, rmask, gmask, bmask, amask);
	m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);	
}

// Function to convert color to Uint32
Uint32 qbImage::ConvertColor(const double red, const double green, const double blue)
{
	// Convert colours to unsigned char.
	unsigned char r = static_cast<unsigned char>((red / m_overallMax) * 255.0);
	unsigned char g = static_cast<unsigned char>((green / m_overallMax) * 255.0);
	unsigned char b = static_cast<unsigned char>((blue / m_overallMax) * 255.0);

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		Uint32 pixelColor = (b << 24) + (g << 16) + (r << 8) + 255;
	#else
		Uint32 pixelColor = (255 << 24) + (r << 16) + (g << 8) + b;
	#endif
	
	return pixelColor;
 }

// Function to compute maximum values.
void qbImage::ComputeMaxValues()
{
	m_maxRed = 0.0;
	m_maxGreen = 0.0;
	m_maxBlue = 0.0;
	m_overallMax = 0.0;
	for (int x=0; x<m_xSize; ++x)
	{
		for (int y=0; y<m_ySize; ++y)
		{
			double redValue		= m_rChannel.at(x).at(y);
			double greenValue	= m_gChannel.at(x).at(y);
			double blueValue	= m_bChannel.at(x).at(y);
			
			if (redValue > m_maxRed)
				m_maxRed = redValue;
				
			if (greenValue > m_maxGreen)
				m_maxGreen = greenValue;
				
			if (blueValue > m_maxBlue)
				m_maxBlue = blueValue;
				
			if (m_maxRed > m_overallMax)
				m_overallMax = m_maxRed;
			
			if (m_maxGreen > m_overallMax)
				m_overallMax = m_maxGreen;
				
			if (m_maxBlue > m_overallMax)
				m_overallMax = m_maxBlue;
		}
	}
}





