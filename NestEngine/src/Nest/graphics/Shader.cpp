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

	Shader::Shader(const std::string &filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);

		m_rendererID = glCreateProgram();
		std::vector<unsigned int> shaderIDs;
		for (const auto &shaderSource : shaderSources)
		{
			unsigned int shader = CompileShader(shaderSource.second, shaderSource.first);
			glAttachShader(m_rendererID, shader);
			shaderIDs.push_back(shader);
		}
		glLinkProgram(m_rendererID);

		int linked;
		glGetProgramiv(m_rendererID, GL_LINK_STATUS, &linked);
		if (linked == GL_FALSE)
		{
			int msgLen;
			glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &msgLen);
			std::vector<GLchar> msg(msgLen);
			glGetProgramInfoLog(m_rendererID, msgLen, &msgLen, msg.data());

			glDeleteProgram(m_rendererID);

			NE_ERROR(std::string(msg.data()));
			NE_ASSERT(0, "Shader link failed.");
		}

		for (unsigned int i : shaderIDs)
		{
			glDeleteShader(i);
		}
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

	static unsigned int shaderTokenToType(const std::string &token)
	{
		if (token == "vertex")
			return GL_VERTEX_SHADER;
		if (token == "geometry")
			return GL_GEOMETRY_SHADER;
		if (token == "fragment" || token == "pixel")
			return GL_FRAGMENT_SHADER;
		if (token == "compute")
			return GL_COMPUTE_SHADER;

		NE_ASSERT(0, "Unidentified shader type.");
		return 0;
	}

	std::unordered_map<unsigned int, std::string> Shader::PreProcess(const std::string &source)
	{
		std::unordered_map<unsigned int, std::string> shaderSources;

		const char *typeToken = "#type";
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\n\r", pos);
			size_t begin = pos + typeTokenLen + 1;
			std::string type = source.substr(begin, eol - begin);

			size_t nextLineBegin = source.find_first_not_of("\n\r", eol);
			pos = source.find(typeToken, nextLineBegin);

			shaderSources[shaderTokenToType(type)] = (pos == std::string::npos) ? source.substr(nextLineBegin) : source.substr(nextLineBegin, pos - nextLineBegin);
		}

		return shaderSources;
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