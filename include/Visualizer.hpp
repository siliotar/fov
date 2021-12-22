#pragma once

#ifndef _WIN32
# include <glad/glad.h>
#else
# include <GL/glew.h>
# include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <vector>
#include <iostream>
#include "utils.hpp"
#include "Config.hpp"

class Visualizer
{
private:
	GLFWwindow		*_window;
	unsigned int	_vao;
	unsigned int	_vbo;
	unsigned int	_shader;
	unsigned int	_color;
	float			*_fovVertices;
	float			*_unitsVertices;
	size_t			_fovVCount;
	size_t			_vCount;
	float			_deltaTime;
	int				_screenWidth;
	int				_screenHeight;
	const Config	&_config;
	float			_tempUnit;
	float			_lastFrame;
	bool			_calculate;
	size_t			_tempUnitsDisplay;

	Visualizer(const Visualizer &other);
	Visualizer	&operator=(const Visualizer &other);
	unsigned int	_makeBuffer(unsigned int type, unsigned int size, void* data);
	void	_setLocations();
	std::string	_parseShader(const std::string &filepath);
	unsigned int	_compileShader(unsigned int type, const std::string &source);
	void	_createShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
	void	_calculateDeltaTime();
	void	_drawScene();
	void	_processInput();
	void	_checkResize();
public:
	Visualizer(const Config &config);
	~Visualizer();
	void	visualize();
};
