#include "Shader.h"

#include <fstream>

#include <glad/glad.h>

#include "Nest/Core/Core.h"
#include "Nest/Renderer/Renderer.h"

namespace Nest
{
	unsigned int Shader::s_currentShader = 0;

	const std::string g_widthUniformName = "u_wWidth",
		g_heightUniformName = "u_wHeight";

	Shader::Shader()
	{
		m_rendererID = 0;
	}

	Shader::Shader(const ShaderSource &rawSource)
	{
		auto shaderSources = SplitSourceByType(rawSource);

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

	Shader::Shader(const std::vector<ShaderSource> &rawSources)
	{
		std::unordered_map<unsigned int, ShaderSource> shaderSources;
		for (const ShaderSource &source : rawSources)
		{
			auto fileShaderSources = SplitSourceByType(source);
			shaderSources.insert(fileShaderSources.begin(), fileShaderSources.end());
		}

		m_rendererID = glCreateProgram();
		std::vector<unsigned int> shaderIDs;
		for (const auto &shaderSource : shaderSources)
		{
			unsigned int shader = CompileShader(shaderSource.second, shaderSource.first);
			glAttachShader(m_rendererID, shader);
			shaderIDs.push_back(shader);
		}
		glLinkProgram(m_rendererID);
		glLinkProgram(m_rendererID);
		glValidateProgram(m_rendererID);

		for (unsigned int i : shaderIDs)
		{
			glDeleteShader(i);
		}
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_rendererID);
	}

	Ref<Shader> Shader::FromFile(const std::string &sourcePath)
	{
		return createRef<Shader>(ReadFile(sourcePath));
	}

	Ref<Shader> Shader::FromFile(std::vector<std::string> sourceFiles)
	{
		std::vector<ShaderSource> shaderSources;
		shaderSources.reserve(sourceFiles.size());
		for (int i = 0; i < sourceFiles.size(); ++i)
		{
			shaderSources.push_back(std::move(ReadFile(sourceFiles[i])));
		}
		return createRef<Shader>(shaderSources);
	}

	void Shader::bind()
	{
		if (s_currentShader != m_rendererID)
		{
			s_currentShader = m_rendererID;
			glUseProgram(m_rendererID);

			setUniform1ui(g_widthUniformName, Renderer::getWindowWidth());
			setUniform1ui(g_heightUniformName, Renderer::getWindowHeight());
		}
	}

	void Shader::unbind() const
	{
		if (s_currentShader != 0)
		{
			s_currentShader = 0;
			glUseProgram(0);
		}
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

	void Shader::setUniform3f(const std::string &name, float v0, float v1, float v2)
	{
		glUniform3f(getUniformLocation(name), v0, v1, v2);
	}

	void Shader::setUniform3f(const std::string &name, chcl::Vector3<float> v)
	{
		glUniform3f(getUniformLocation(name), v.x, v.y, v.z);
	}

	void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
	{
		glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
	}

	void Shader::setUniform4f(const std::string &name, chcl::Vector4<float> v)
	{
		glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
	}

	void Shader::setUniform1i(const std::string &name, int v)
	{
		glUniform1i(getUniformLocation(name), v);
	}

	void Shader::setUniform1ui(const std::string &name, unsigned int v)
	{
		glUniform1ui(getUniformLocation(name), v);
	}

	void Shader::setUniform3fArr(const std::string &name, unsigned int count, std::vector<chcl::Vector3<float>>& arr)
	{
		// Vector position values are stored contiguously, so this is safe
		glUniform3fv(getUniformLocation(name), count, (float*) arr.data());
	}

	void Shader::setUniformMat3(const std::string & name, const chcl::Mat3 &mat)
	{
		glUniformMatrix3fv(getUniformLocation(name), 1, true, mat.data());
	}

	void Shader::setUniformMat4(const std::string &name, const chcl::Mat4 &mat)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, true, mat.data());
	}

	void Nest::Shader::bindBuffer(Ref<ShaderStorageBuffer> buf, unsigned int binding)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, buf->getID());
	}

	int Shader::getUniformLocation(const std::string &name)
	{
		if (m_uniformLocations[name])
			return m_uniformLocations[name];

		int location = glGetUniformLocation(m_rendererID, name.c_str());
		m_uniformLocations[name] = location;
		if (location == -1)
			NE_WARN("Uniform location for '{}' not found.", name);

		return location;
	}

	Shader::ShaderSource Shader::ReadFile(const std::string &path)
	{
		std::ifstream file(path);
		std::string result, line;
		while (std::getline(file, line))
		{
			result += line;
			result += '\n';
		}

		size_t nameBegin = path.find_last_of("/\\");
		if (nameBegin == std::string::npos)
			nameBegin = 0;
		size_t nameEnd = path.find('.', nameBegin);
		size_t count = (nameEnd == std::string::npos) ? std::string::npos : nameEnd - nameBegin;

		std::string shaderName = path.substr(nameBegin, count);

		return { result, shaderName };
	}

	static unsigned int shaderTokenToType(const std::string &token)
	{
		if (token == "vertex" || token == "vert")
			return GL_VERTEX_SHADER;
		if (token == "geometry" || token == "geom")
			return GL_GEOMETRY_SHADER;
		if (token == "fragment" || token == "frag" || token == "pixel")
			return GL_FRAGMENT_SHADER;
		if (token == "compute" || token == "comp")
			return GL_COMPUTE_SHADER;

		NE_ASSERT(0, "Unidentified shader type.");
		return 0;
	}

	std::unordered_map<unsigned int, Shader::ShaderSource> Shader::SplitSourceByType(const ShaderSource &source)
	{
		std::unordered_map<unsigned int, ShaderSource> shaderSources;

		const char *typeToken = "@shader";
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = source.content.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.content.find_first_of("\n\r", pos);
			size_t begin = pos + typeTokenLen + 1;
			std::string typeStr = source.content.substr(begin, eol - begin);
			unsigned int shaderType = shaderTokenToType(typeStr);

			size_t nextLineBegin = source.content.find_first_not_of("\n\r", eol);
			pos = source.content.find(typeToken, nextLineBegin);

			if (pos == std::string::npos)
			{
				shaderSources[shaderType].content = source.content.substr(nextLineBegin);
			}
			else
			{
				shaderSources[shaderType].content = source.content.substr(nextLineBegin, pos - nextLineBegin);
			}
			shaderSources[shaderType].name = source.name;
			PreProcess(shaderSources[shaderType]);
		}

		return shaderSources;
	}

	void Shader::PreProcess(ShaderSource &source)
	{
		static const char *defaultUniformToken = "@neDefaultUniforms";
		static const char *defaultUniformValue = "uniform uint u_wWidth;\nuniform uint u_wHeight;";

		size_t defaultUniformLen = std::strlen(defaultUniformToken);
		size_t pos = source.content.find(defaultUniformToken);
		while (pos != std::string::npos)
		{
			source.content.replace(pos, defaultUniformLen, defaultUniformValue);
			pos = source.content.find(defaultUniformToken);
		}
	}

	unsigned int Shader::CompileShader(const ShaderSource &source, unsigned int type)
	{
		unsigned int shader = glCreateShader(type);
		const char* src = source.content.c_str();
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
			NE_ASSERT(0, "Failed to compile shader \"{}\".", source.name);
		}

		return shader;
	}
}