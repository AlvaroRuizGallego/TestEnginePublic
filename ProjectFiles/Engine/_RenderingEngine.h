#ifndef _RENDERING_ENGINE_H
#define _RENDERING_ENGINE_H

#include <glew.h>
#include <glfw3.h>

#include "_Window.h"
#include "_Quad.h"
#include "_Texture.h"
#include "Camera.h"

#include <map>

class _SpriteManager;

class _RenderingEngine
{
private:

	//Instance
	static _RenderingEngine* _instance;

	//Window
	_Window* _window;

	//Quad struct, it stores the quad vertices and indices information, and some auxiliar variables
	_Quad _quad;

	//_Sprite manager
	_SpriteManager* _spriteManager;

	//ID of the buffers
	GLuint _transformBufferID;
	GLuint _vertexBufferID;
	GLuint _uvBufferID;
	GLuint _colorBufferID;
	GLuint _linesVertexBufferID;


	//Id of the current program
	GLuint _programID;
	GLuint _linesProgramID;
	GLuint _translucencyProgramID;
	GLuint _vignetteProgramID;

	//ID of the intermediate framebuffer & texture
	GLuint _framebufferID;
	GLuint _intermediateTextureID;
	GLuint _depthBufferID;

	//Camera
	Camera* _camera;
	Camera* _defaultCamera;

	//Control variables
	bool _initialized;

	
	//Textures
	std::map<_Texture*, GLuint> _textureMap;
	
	//Other
	GLint _textureFilteringMode = GL_NEAREST;


	//Constructor
	_RenderingEngine();

	//Open gl initialization (glew)
	void initializeGL();

	//Prepare everything needed to draw
	void prepareGL();

	//Debug functions
	bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType, GLchar * errorMessage);
	bool checkShaderCompilationStatus(GLint shaderID);
	bool checkProgramLinkStatus(GLint programID);

	//Shader management functions
	void installShaders();
	std::string readShaderCode(std::string  fileName);
	GLuint installShader(std::string  vertexShaderFile, std::string  fragmentShaderFile);

	//Texture
	void setTexture(_Texture* texture);

	//Sends data to open gl (transform matrices of sprites)
	void updateRenderData();
	void updateLinesRenderData();
	void updateVignetteData();

	//Render
	void render();
	void linesRender();
	void renderVignette();

	//Error
	void checkErrors(std::string errorID = "");

public:

	~_RenderingEngine();

	//Instance
	static _RenderingEngine* GetInstance();

	//Initialize and terminate functions
	void Initialize(_Window* window, GLint textureFilteringMode);

	//Update
	void Update();

	//Camera
	void SetCamera(Camera* camera);
	//Camera* GetCamera();

	//Clear
	void Clear();

	void LoadTexture(_Texture* texture);
};


#endif