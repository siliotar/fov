#include "Visualizer.hpp"

Visualizer::Visualizer(const Config &config):
_screenWidth(1366), _screenHeight(768), _config(config), _tempUnit(0.0f), _lastFrame(0.0f), _calculate(true)
{
	if (!glfwInit())
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

#ifdef __linux__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	_window = glfwCreateWindow(_screenWidth, _screenHeight, "fov", 0, 0);
	if (!_window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(_window);

	glfwSwapInterval(1);

#ifndef _WIN32
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
#else
	if (glewInit() != GLEW_OK)
#endif
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	_fovVCount = ceilf(_config.getFov() * 180.0f / M_PI);
	_vCount = _config.getUnits().size();
	_fovVertices = new float[(_fovVCount + 2) * 2];
	_unitsVertices = new float[_vCount * 2];

	if (_vCount < _fovVCount)
		_vbo = _makeBuffer(GL_ARRAY_BUFFER, (_fovVCount + 2) * sizeof(float) * 2, nullptr);
	else
		_vbo = _makeBuffer(GL_ARRAY_BUFFER, _vCount * sizeof(float) * 2, nullptr);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	_createShader("res/shaders/VertexBasic.shader", "res/shaders/FragmentBasic.shader");

	glUseProgram(_shader);

	_setLocations();

	glEnable(GL_DEPTH_TEST);
}

void	Visualizer::_setLocations()
{
	_color = glGetUniformLocation(_shader, "color");
}

unsigned int	Visualizer::_makeBuffer(unsigned int type, unsigned int size, void* data)
{
	unsigned int	buffer;

	glGenBuffers(1, &buffer);
	glBindBuffer(type, buffer);
	glBufferData(type, size, data, GL_DYNAMIC_DRAW);
	return buffer;
}

Visualizer::~Visualizer()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteProgram(_shader);
	glfwDestroyWindow(_window);
	glfwTerminate();
	delete[] _fovVertices;
	delete[] _unitsVertices;
}

std::string	Visualizer::_parseShader(const std::string &filepath)
{
	std::string	ret;

	ret = readFile(filepath);
	if (ret.size() < 12)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
#ifdef __linux__
	ret[9] = '3';
	ret[10] = '3';
	ret[11] = '0';
#else
	ret[9] = '4';
	ret[10] = '1';
	ret[11] = '0';
#endif
	return ret;
}

unsigned int	Visualizer::_compileShader(unsigned int type, const std::string &source)
{
	unsigned int	id = glCreateShader(type);
	const char	*src = source.c_str();
	glShaderSource(id, 1, &src, 0);
	glCompileShader(id);

	int	result = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int	length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char	message[512];
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile shader\n" << message << std::endl;
		glDeleteShader(id);
		exit(EXIT_FAILURE);
	}
	return id;
}

void	Visualizer::_createShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
	std::string	vertexShader = _parseShader(vertexShaderPath);

	std::string	fragmentShader = _parseShader(fragmentShaderPath);

	_shader = glCreateProgram();
	unsigned int	vs = _compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int	fs = _compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(_shader, vs);
	glAttachShader(_shader, fs);
	glLinkProgram(_shader);
	glValidateProgram(_shader);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void	Visualizer::_calculateDeltaTime()
{
	float	currentFrame;

	currentFrame = glfwGetTime();
	_deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;
}

void	Visualizer::_drawScene()
{
	glBindVertexArray(_vao);
	glUseProgram(_shader);

	// draw units

	const Unit &mainUnit = _config.getUnits()[_tempUnit];

	float	aspectRatio = (float)_screenWidth / _screenHeight;
	
	if (_calculate)
	{
		const std::vector<Unit>	&units = _config.getUnits();
		size_t	unitsSize = units.size();
		
		float	xMaxDist = _config.getDistance() * 2.0f;
		float	yMaxDist = _config.getDistance() * 2.0f;
		if (aspectRatio > 1.0f)
			xMaxDist *= aspectRatio;
		if (aspectRatio < 1.0f)
			yMaxDist /= aspectRatio;

		float	maxX = mainUnit.pos.x + xMaxDist;
		float	minX = mainUnit.pos.x - xMaxDist;
		float	maxY = mainUnit.pos.y + yMaxDist;
		float	minY = mainUnit.pos.y - yMaxDist;

		_tempUnitsDisplay = 0;

		for (size_t i = 0; i < unitsSize; ++i)
		{
			const Unit &tempUnit = units[i];
			if (tempUnit.pos.x > maxX || tempUnit.pos.x < minX || \
				tempUnit.pos.y > maxY || tempUnit.pos.y < minY)
				continue;
			_unitsVertices[_tempUnitsDisplay * 2] = (tempUnit.pos.x - mainUnit.pos.x) / xMaxDist;
			_unitsVertices[_tempUnitsDisplay * 2 + 1] = (tempUnit.pos.y - mainUnit.pos.y) / yMaxDist;
			++_tempUnitsDisplay;
		}
	}

	glBufferSubData(GL_ARRAY_BUFFER, 0, _tempUnitsDisplay * 2 * sizeof(float), _unitsVertices);

	glUniform3f(_color, 0.0f, 0.65f, 1.0f);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(4.0f);

	glDrawArrays(GL_POINTS, 0, _tempUnitsDisplay);

	// draw fov

	if (_calculate)
	{
		_fovVertices[0] = 0.0f;
		_fovVertices[1] = 0.0f;

		Point	vec(1.0f, 0.0f);
		float	scalar = mainUnit.direction.x * vec.x + mainUnit.direction.y * vec.y;
		float	modul = mainUnit.dirLength * getVecLength(vec);
		float	cosinus = scalar / modul;
		float	angle = acos(cosinus);
		float	startCorner = angle - (_config.getFov() / 2.0f);

		for (size_t i = 1; i <= _fovVCount; ++i)
		{
			float	tmp = startCorner + (float) (i - 1) / _fovVCount * _config.getFov();
			_fovVertices[i * 2] = 0.5f * cos(tmp);
			if (aspectRatio > 1.0f)
				_fovVertices[i * 2] /= aspectRatio;
			_fovVertices[i * 2 + 1] = 0.5f * sin(tmp);
			if (aspectRatio < 1.0f)
				_fovVertices[i * 2 + 1] *= aspectRatio;
		}

		_calculate = false;
	}

	glBufferSubData(GL_ARRAY_BUFFER, 0, (_fovVCount + 1) * 2 * sizeof(float), _fovVertices);

	glUniform3f(_color, 1.0f, 0.65f, 0.0f);

	glDrawArrays(GL_TRIANGLE_FAN, 0, _fovVCount + 1);
}

void	Visualizer::_processInput()
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, 1);

	size_t	oldUnitPos = _tempUnit;
	if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		_tempUnit += _deltaTime * 10;
	else if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS)
		_tempUnit -= _deltaTime * 10;
	if (_tempUnit < 0.0f)
		_tempUnit = 0.0f;
	else if (_tempUnit >= _config.getUnits().size())
		_tempUnit = _config.getUnits().size() - 1.0f;
	if (oldUnitPos != (size_t)_tempUnit)
		_calculate = true;
}

void	Visualizer::_checkResize()
{
	int	w, h;

	w = _screenWidth;
	h = _screenHeight;
	glfwGetWindowSize(_window, &_screenWidth, &_screenHeight);
	if (w != _screenWidth || h != _screenHeight)
	{
		glfwGetFramebufferSize(_window, &w, &h);
		glViewport(0, 0, w, h);
	}
}

void	Visualizer::visualize()
{
	std::cout << "\n\n";
	std::cout << "Press the right arrow to switch to the next unit.\n";
	std::cout << "Press the left arrow to switch to the previous unit.\n";
	while (!glfwWindowShouldClose(_window))
	{
		_checkResize();
		_calculateDeltaTime();
		glClearColor(0.52f, 0.52f, 0.52f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_processInput();

		_drawScene();

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}
