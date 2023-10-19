#pragma once

#include <string>
#include <unordered_map>

#include <maths/SquareMatrix.h>
#include <geometry/Vector2.h>
#include <geometry/Vector4.h>

#include "../Core.h"

namespace Nest
{
	class Shader
	{
		unsigned int m_rendererID;
		std::unordered_map<std::string, int> m_uniformLocations;

	public:
		Shader();
		Shader(const std::string &filepath);
		Shader(const std::string &vertexPath, const std::string &fragPath);
		Shader(const Shader&) = delete;
		Shader(Shader&&) = default;
		~Shader();

		void bind() const;
		void unbind() const;

		void setUniform1f(const std::string &name, float v);
		void setUniform2f(const std::string &name, float v0, float v1);
		void setUniform2f(const std::string &name, chcl::Vector2<float> v);
		void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
		void setUniform4f(const std::string &name, chcl::Vector4<float> v);

		void setUniformMat4(const std::string &name, const chcl::Mat4 &mat);

		Shader& operator=(Shader&&) = default;

	private:
		int getUniformLocation(const std::string &name);

		static std::string ReadFile(const std::string &path);
		static std::unordered_map<unsigned int, std::string> PreProcess(const std::string &source);
		static unsigned int CompileShader(const std::string &source, unsigned int type);
	};
}