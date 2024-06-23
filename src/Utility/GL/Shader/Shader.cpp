#include "Shader.hpp"

#include <fmt/format.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

Shader::Shader(const std::string& _vertexFilePath, const std::string& _fragmentFilePath):
    vertexFilePath(_vertexFilePath),
    fragmentFilePath(_fragmentFilePath),
    logger("Shader", Logger::FGColors::BLUE) {}

Shader::~Shader() {}

void Shader::loadShaders() {
    // Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertexFilePath, std::ios::in);

	if (vertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << vertexShaderStream.rdbuf();
		vertexShaderCode = sstr.str();
		vertexShaderStream.close();
	} else {
		logger.error(fmt::format("Could not open file {}, are you in the right directory?", vertexFilePath));
	}

	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragmentFilePath, std::ios::in);

	if (fragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << fragmentShaderStream.rdbuf();
		fragmentShaderCode = sstr.str();
		fragmentShaderStream.close();
	} else {
		logger.error(fmt::format("Could not open file {}, are you in the right directory?", fragmentFilePath));
	}

	GLint result = GL_FALSE;
	int infoLogLength;

    logger.info(fmt::format("Compiling shader program..."));

	// Compile Vertex Shader
	char const* vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0){
		std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);

        logger.error(fmt::format("Vertex Shader Compile Failure! File: {}\nError Below:\n{}\n", vertexFilePath, &vertexShaderErrorMessage[0]));
	}

	// Compile Fragment Shader
	char const* fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if ( infoLogLength > 0 ){
		std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);

        logger.error(fmt::format("Fragment Shader Compile Failure! File: {}\nError Below:\n{}\n", fragmentFilePath, &fragmentShaderErrorMessage[0]));
	}

	// Link the program
	logger.info("Linking Shader Program...");

	program = glCreateProgram();
	glAttachShader(program, vertexShaderID);
	glAttachShader(program, fragmentShaderID);
	glLinkProgram(program);

	// Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0){
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(program, infoLogLength, NULL, &programErrorMessage[0]);

        logger.error(fmt::format("Shader Program Linking Failure! Error Below:\n{}\n", &programErrorMessage[0]));
	}

	glDetachShader(program, vertexShaderID);
	glDetachShader(program, fragmentShaderID);
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

    shaderLoaded  = true;
	logger.info("Shader Program loaded!");
}
void Shader::unloadShader() {
    if (shaderLoaded) {
        glDeleteProgram(program);
        shaderLoaded = false;
    }
}

bool Shader::isShaderLoaded() const {
    return shaderLoaded;
}

void Shader::useShader() {
    glUseProgram(program);
}