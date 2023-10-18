#include "Shader.h"

#include <fstream>

#include <glad/glad.h>

#include "../log/Logger.h"

namespace Nest
{
	Shader::Shader()
	{
		m_rendererID = 0;
	}

	Shader::Shader(const std::string &vertexPath, const std::string &fragPath)
	{
		unsigned int vShader = CompileShader(ReadFile(vertexPath), GL_VERTEX_SHADER);
		unsigned int fShader = CompileShader(ReadFile(fragPath), GL_FRAGMENT_SHADER);

		m_rendererID = glCreateProgram();
		glAttachShader(m_rendererID, vShader);
		glAttachShader(m_rendererID, fShader);
		glLinkProgram(m_rendererID);
		glValidateProgram(m_rendererID);

		glDeleteShader(vShader);
		glDeleteShader(fShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_rendererID);
	}

	void Shader::bind() const
	{
		glUseProgram(m_rendererID);
	}

	void Shader::unbind() const
	{
		glUseProgram(0);
	}

	void Shader::setUniform1f(const std::string &name, float v)
	{
		glUniform1f(getUniformLocation(name), v);
	}

	void Shader::setUniform2f(const std::string &name, float v0, float v1)
	{
		glUniform2f(getUniformLocation(name), v0, v1);
	}

	void Shader::setUniform2f(const std::string &name, chcl::Vector2<float> v)
	{
		glUniform2f(getUniformLocation(name), v.x, v.y);
	}

	void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
	{
		glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
	}

	void Shader::setUniform4f(const std::string &name, chcl::Vector4<float> v)
	{
		glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
	}

	void Shader::setUniformMat4(const std::string &name, const chcl::Mat4 &mat)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, true, mat.values());
	}

	int Shader::getUniformLocation(const std::string &name)
	{
		if (m_uniformLocations[name])
			return m_uniformLocations[name];

		int location = glGetUniformLocation(m_rendererID, name.c_str());
		m_uniformLocations[name] = location;
		if (location == -1)
			NE_WARN("Uniform location for {} not found.");

		return location;
	}

	std::string Shader::ReadFile(const std::string &path)
	{
		std::ifstream file(path);
		std::string result, line;
		while (std::getline(file, line))
		{
			result += line;
			result += '\n';
		}
		return result;
	}

	unsigned int Shader::CompileShader(const std::string &source, unsigned int type)
	{
		unsigned int shader = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char *msg = (char*)_malloca(length);
			glGetShaderInfoLog(shader, length, &length, msg);
			NE_ERROR(msg);
		}

		return shader;
	}
}