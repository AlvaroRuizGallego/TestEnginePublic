#include "_RenderingEngine.h"
#include "SpriteSheet.h"

#include "_AudioEngine.h"
#include "Drawer.h"
#include "_SpriteManager.h"
#include "Timer.h"

#include <fstream>
#include <iostream>
#include <string>



_RenderingEngine* _RenderingEngine::_instance = nullptr;

_RenderingEngine::_RenderingEngine()
{
	_initialized = false;

	_camera = nullptr;
	_defaultCamera = new Camera();

	_spriteManager = _SpriteManager::GetInstance();
}


_RenderingEngine::~_RenderingEngine()
{
	_instance = nullptr;

	delete _defaultCamera;
}

_RenderingEngine * _RenderingEngine::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new _RenderingEngine();
	}
	return _instance;
}

void _RenderingEngine::initializeGL()
{
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("\nERROR: _RenderingEngine::initializeGL -> Failed to initialize GLEW");
		fflush(stdout);
		exit(EXIT_FAILURE);
	}

	//glClearColor(0.02, 0.06, 0.12, 1.0);
	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);//Antialiasing

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSwapInterval(0);

	/*
	GLint texSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);

	printf("\nMAX TEXTURE SIZE: %i", texSize);
	*/
}

void _RenderingEngine::prepareGL()
{
	//Vertex buffer
	glGenBuffers(1, &_vertexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_quad.vertices), _quad.vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);


	//UV buffer
	glGenBuffers(1, &_uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);

	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2 * 4, (void*)(0));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2 * 4, (void*)(sizeof(GLfloat) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2 * 4, (void*)(sizeof(GLfloat) * 4));
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2 * 4, (void*)(sizeof(GLfloat) * 6));

	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);


	//Transforms
	glGenBuffers(1, &_transformBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _transformBufferID);

	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);
	glEnableVertexAttribArray(8);

	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(0));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(GLfloat) * 4));
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(GLfloat) * 8));
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(GLfloat) * 12));
	
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);


	//Color buffer
	glGenBuffers(1, &_colorBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);

	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
	
	glVertexAttribDivisor(9, 1);


	//Indices
	GLuint indexBufferID;

	glGenBuffers(1, &indexBufferID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_quad.indices), _quad.indices, GL_STATIC_DRAW);


	//Lines
	//Vertex buffer
	glGenBuffers(1, &_linesVertexBufferID);

	//Textures
	glActiveTexture(GL_TEXTURE0);

	glProgramUniform1i(_programID, glGetUniformLocation(_programID, "tex"), 0);
	glProgramUniform1i(_translucencyProgramID, glGetUniformLocation(_translucencyProgramID, "tex"), 0);
	glProgramUniform1i(_vignetteProgramID, glGetUniformLocation(_vignetteProgramID, "tex"), 0);
	glProgramUniform1i(_vignetteProgramID, glGetUniformLocation(_vignetteProgramID, "framebufferTex"), 1);


	//Intermediate framebuffer & texture
	//Texture
	glGenTextures(1, &_intermediateTextureID);
	glBindTexture(GL_TEXTURE_2D, _intermediateTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _textureFilteringMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _textureFilteringMode);

	glTexImage2D(GL_TEXTURE_2D, 0, _Texture::GetFormat(), _window->GetWidth(), _window->GetHeight(), 0, _Texture::GetFormat(), _Texture::GetDataType(), 0);

	//Framebuffer
	glGenFramebuffers(1, &_framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _intermediateTextureID, 0);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	//Depth buffer
	glGenRenderbuffers(1, &_depthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _window->GetWidth(), _window->GetHeight());

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBufferID);


	//Start parameters
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	Rectangle screenRegion = _camera->GetScreenRegion();
	glViewport(screenRegion.min.x, screenRegion.min.y, screenRegion.GetWidth(), screenRegion.GetHeight());
}


bool _RenderingEngine::checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType, GLchar* errorMessage)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);

	if (status != GL_TRUE)
	{
		GLint infoLogLenght;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLenght);

		GLchar* buffer = new GLchar[infoLogLenght];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLenght, &bufferSize, buffer);
		printf("%s%s\n", errorMessage, buffer);

		delete[] buffer;

		return false;
	}
	return true;
}

bool _RenderingEngine::checkShaderCompilationStatus(GLint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS, "Shader compilation failed:\n");
}

bool _RenderingEngine::checkProgramLinkStatus(GLint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS, "Program link failed:\n");
}

void _RenderingEngine::installShaders()
{
	_programID = installShader("Default.vx", "Default.fg");
	_translucencyProgramID = installShader("Default.vx", "Translucent.fg");
	_linesProgramID = installShader("Lines.vx", "Lines.fg");
	_vignetteProgramID = installShader("Default.vx", "Vignette.fg");
}

std::string _RenderingEngine::readShaderCode(std::string  fileName)
{
	std::ifstream shaderFile(fileName);
	if (!shaderFile.good())
	{
		printf("\nERROR: _RenderingEngine::readShaderCode -> File failed to load: %s", fileName);
		fflush(stdout);
		exit(EXIT_FAILURE);
	}

	std::string result = std::string(std::istreambuf_iterator<char>(shaderFile), std::istreambuf_iterator<char>());

	shaderFile.close();

	return result;
}

GLuint _RenderingEngine::installShader(std::string  vertexShaderFile, std::string  fragmentShaderFile)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderCodeStr = readShaderCode(vertexShaderFile);
	std::string fragmentShaderCodeStr = readShaderCode(fragmentShaderFile);

	const char*  vertexShaderCode = vertexShaderCodeStr.c_str();
	const char*  fragmentShaderCode = fragmentShaderCodeStr.c_str();

	glShaderSource(vertexShaderID, 1, &vertexShaderCode, 0);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderCompilationStatus(vertexShaderID) || !checkShaderCompilationStatus(fragmentShaderID))
	{
		return -1;
	}

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramLinkStatus(programID))
	{
		return -1;
	}

	return programID;
}

void _RenderingEngine::setTexture(_Texture* texture)
{
	glActiveTexture(GL_TEXTURE0);

	auto it = _textureMap.find(texture);
	if (it != _textureMap.end())
	{
		glBindTexture(GL_TEXTURE_2D, it->second);
	}
	else
	{
		printf("\nWARNING: _Renderer::setTexture -> Texture not loaded");
	}
}


void _RenderingEngine::updateRenderData()
{
	//Vertices
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(0, 3, _quad.vertexDataType, GL_FALSE, _quad.vertexSize, 0);
	

	//Transforms + UVs + Colors
	std::vector<glm::mat4> transforms;
	transforms.reserve(_spriteManager->GetSpriteCount());

	std::vector<GLfloat> uvs;
	uvs.reserve(_spriteManager->GetSpriteCount()*8);

	std::vector<glm::vec4> colors;
	colors.reserve(_spriteManager->GetSpriteCount());

	for (int i = 0; i < _spriteManager->GetSpriteBatchVector()->size(); ++i)
	{
		_SpriteBatch* spriteBatch = _spriteManager->GetSpriteBatchVector()->at(i);
		std::vector<_Sprite*>* sprites = spriteBatch->GetSpriteVector(true);

		for (int j = 0; j < sprites->size(); ++j)
		{
			_Sprite* sprite = sprites->at(j);
			glm::mat4 transformMatrix = sprite->IsCameraSpace() ? _camera->GetCameraSpaceTransform()* sprites->at(j)->GetTransform() : _camera->GetTransform()* sprites->at(j)->GetTransform();

			transformMatrix[3][0] *= sprites->at(j)->GetParallax();
			transformMatrix[3][1] *= sprites->at(j)->GetParallax();

			transforms.push_back(transformMatrix);

			const std::vector<GLfloat>* spriteUVs = sprites->at(j)->GetUVs();
			uvs.insert(uvs.end(), spriteUVs->begin(), spriteUVs->end());

			colors.push_back(sprite->GetColor());
		}
	}

	if (transforms.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _transformBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transforms.size(), &transforms[0], GL_DYNAMIC_DRAW);
	}
	if (uvs.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*uvs.size(), &uvs[0], GL_DYNAMIC_DRAW);
	}
	if (colors.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
		glVertexAttribDivisor(9, 1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*colors.size(), &colors[0], GL_DYNAMIC_DRAW);
	}
}

void _RenderingEngine::updateLinesRenderData()
{
	std::vector<glm::vec4>* vertices = Drawer::GetInstance()->getVertices();
	std::vector<GLfloat> processedVertices;

	for (int i = 0; i < vertices->size(); ++i)
	{
		vertices->at(i) = _camera->GetTransform() * vertices->at(i);

		processedVertices.push_back(vertices->at(i).x);
		processedVertices.push_back(vertices->at(i).y);
		processedVertices.push_back(vertices->at(i).z);
	}

	if (processedVertices.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _linesVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*processedVertices.size(), &processedVertices[0], GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	}

	std::vector<glm::vec4>* colors = Drawer::GetInstance()->getColors();

	if (colors->size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
		glVertexAttribDivisor(9, 0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*colors->size(), &colors->at(0), GL_DYNAMIC_DRAW);
	}
}

void _RenderingEngine::updateVignetteData()
{
	//Vertices
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(0, 3, _quad.vertexDataType, GL_FALSE, _quad.vertexSize, 0);


	//Sprite
	_Sprite* vignetteSprite = _camera->_vignette;

	//Transform
	glm::mat4 transform = _camera->GetCameraSpaceTransform()* vignetteSprite->GetTransform();
	glBindBuffer(GL_ARRAY_BUFFER, _transformBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &transform, GL_DYNAMIC_DRAW);

	//Uvs
	std::vector<GLfloat> uvs;
	const std::vector<GLfloat>* spriteUVs = vignetteSprite->GetUVs();
	uvs.insert(uvs.end(), spriteUVs->begin(), spriteUVs->end());
	glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*uvs.size(), &uvs[0], GL_DYNAMIC_DRAW);

	//Color
	glm::vec4 color = vignetteSprite->GetColor();
	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
	glVertexAttribDivisor(9, 1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4), &color, GL_DYNAMIC_DRAW);
}

void _RenderingEngine::render()
{
	GLint baseInstance = 0;
	GLsizei spriteCount;
	for (int i = 0; i < _spriteManager->GetSpriteBatchVector()->size(); i++)
	{
		_SpriteBatch* spriteBatch = _spriteManager->GetSpriteBatchVector()->at(i);

		setTexture(spriteBatch->GetTexture());

		spriteCount = (GLsizei)spriteBatch->GetSpriteVector()->size();

		glDrawElementsInstancedBaseInstance(_quad.renderMode, _quad.indexCount, _quad.indexDataType, 0, spriteCount, baseInstance);

		baseInstance += spriteCount;
	}
}

void _RenderingEngine::linesRender()
{
	int lineWidth, nLines;
	int baseVertex = 0;
	int size;

	for (auto it = Drawer::GetInstance()->_lineMap.begin(); it != Drawer::GetInstance()->_lineMap.end(); ++it)
	{
		lineWidth = it->first;
		nLines = it->second.size();
		size = nLines * 2;

		glLineWidth(lineWidth);
		glDrawArrays(GL_LINES, baseVertex, size);

		baseVertex += size;
	}
}

void _RenderingEngine::renderVignette()
{
	//Set vignette texture
	setTexture(_camera->_vignette->GetTexture());

	//Set framebuffer texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _intermediateTextureID);

	glDrawElementsInstanced(_quad.renderMode, _quad.indexCount, _quad.indexDataType, 0, 1);
}


void _RenderingEngine::checkErrors(std::string errorID)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("\n----- %s ---------", errorID.c_str());
		switch (err)
		{
		case GL_INVALID_ENUM:
			printf("\nRENDERER ERROR: GL_INVALID_ENUM");
			break;

		case GL_INVALID_VALUE:
			printf("\nRENDERER ERROR: GL_INVALID_VALUE");
			break;

		case GL_INVALID_OPERATION:
			printf("\nRENDERER ERROR: GL_INVALID_OPERATION");
			break;

		case GL_STACK_OVERFLOW:
			printf("\nRENDERER ERROR: GL_STACK_OVERFLOW");
			break;

		case GL_STACK_UNDERFLOW:
			printf("\nRENDERER ERROR: GL_STACK_UNDERFLOW");
			break;

		case GL_OUT_OF_MEMORY:
			printf("\nRENDERER ERROR: GL_OUT_OF_MEMORY");
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			printf("\nRENDERER ERROR: GL_INVALID_FRAMEBUFFER_OPERATION");
			break;

		case GL_CONTEXT_LOST:
			printf("\nRENDERER ERROR: GL_CONTEXT_LOST");
			break;

		case GL_TABLE_TOO_LARGE:
			printf("\nRENDERER ERROR: GL_TABLE_TOO_LARGE");
			break;

		default:
			printf("\nRENDERER ERROR: UNKNOWN ERROR");
			break;
		}
	}
}

void _RenderingEngine::LoadTexture(_Texture * texture)
{
	if (_textureMap.find(texture) != _textureMap.end()) return;

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _textureFilteringMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _textureFilteringMode);

	glTexImage2D(GL_TEXTURE_2D, 0, _Texture::GetFormat(), texture->GetWidth(), texture->GetHeight(), 0, _Texture::GetFormat(), _Texture::GetDataType(), texture->GetPixels());

	_textureMap.insert(std::pair<_Texture*, GLuint>(texture, textureID));
}


void _RenderingEngine::Initialize(_Window* window, GLint textureFilteringMode)
{
	if (_initialized) return;
	_initialized = true;

	_window = window;
	_window->MakeContextCurrent();
	_textureFilteringMode = textureFilteringMode;

	initializeGL();
	installShaders();
	prepareGL();

	SetCamera(_defaultCamera);
}


void _RenderingEngine::Update()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (_camera->_vignette != nullptr)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthBufferID);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	//SPRITES
	updateRenderData();

	//Default (opaque)
	glUseProgram(_programID);
	render();

	//Translucent
	glDepthMask(GL_FALSE);
	glUseProgram(_translucencyProgramID);
	render();

	glDisable(GL_DEPTH_TEST);

	//VIGNETTE
	if (_camera->_vignette != nullptr)
	{
		glViewport(0, 0, _window->GetWidth(), _window->GetHeight());

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		updateVignetteData();

		glUseProgram(_vignetteProgramID);
		renderVignette();

		Rectangle screenRegion = _camera->GetScreenRegion();
		glViewport(screenRegion.min.x, screenRegion.min.y, screenRegion.GetWidth(), screenRegion.GetHeight());
	}

	//LINES
	updateLinesRenderData();

	glUseProgram(_linesProgramID);
	linesRender();

	Drawer::GetInstance()->Clear();


	//Swap buffers
	//printf("\nRENDER UPDATE A");//TEMP
	_window->SwapBuffers();
	//printf("\nRENDER UPDATE B");//TEMP
}

void _RenderingEngine::SetCamera(Camera * camera)
{
	_camera = camera;
	Camera::setActiveCamera(_camera);
}

void _RenderingEngine::Clear()
{
	_spriteManager->Clear();

	SetCamera(_defaultCamera);

	//Clear textures
	std::vector<GLuint> textureIDs;

	for (auto it = _textureMap.begin(); it != _textureMap.end(); it++)
	{
		textureIDs.push_back(it->second);
	}

	glDeleteTextures(textureIDs.size(), &textureIDs[0]);
	_textureMap.clear();
}










