#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <maths/SquareMatrix.h>
#include <geometry/Vector2.h>
#include <geometry/Vector3.h>
#include <geometry/Vector4.h>

#include "Nest/Core/Core.h"

namespace Nest
{
	class Shader
	{
		unsigned int m_rendererID;
		std::unordered_map<std::string, int> m_uniformLocations;

	public:
		Shader();
		Shader(const std::string &source);
		Shader(const std::string &vertexSource, const std::string &fragmentSource);
		Shader(const Shader&) = delete;
		Shader(Shader&&) = default;
		~Shader();

		static Ref<Shader> FromFile(const std::string &sourcePath);
		static Ref<Shader> FromFile(const std::string &vertexPath, const std::string &fragmentPath);

		void bind() const;
		void unbind() const;

		void setUniform1f(const std::string &name, float v);
		void setUniform2f(const std::string &name, float v0, float v1);
		void setUniform2f(const std::string &name, chcl::Vector2<float> v);
		void setUniform3f(const std::string &name, float v0, float v1, float v2);
		void setUniform3f(const std::string &name, chcl::Vector3<float> v);
		void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
		void setUniform4f(const std::string &name, chcl::Vector4<float> v);

		void setUniform3fArr(const std::string &name, unsigned int count, std::vector<chcl::Vector3<float>> &arr);

		void setUniformMat3(const std::string &name, const chcl::Mat3 &mat);
		void setUniformMat4(const std::string &name, const chcl::Mat4 &mat);

		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) = default;

	private:
		/**
		 * @brief Returns shader uniform location
		 * @param name Uniform name.
		 * @return The shader location. Returns -1 if the location was not found.
		*/
		int getUniformLocation(const std::string &name);

		/** @brief Returns file contents as a string
		 *	@param path Filepath.
		 *	@returns File contents as string.
		 */
		static std::string ReadFile(const std::string &path);

		/**
		 * @brief Splits shader file into shader types
		 * 
		 * Uses the #type token in shader files to split into individual shader sources.
		 * Currently supported tokens:
		 *	- frag, fragment - Fragment shaders
		 *	- vert, vertex - Vertex shaders
		 *	- comp, compute - Compute shaders
		 * 
		 * @param source Shader source (not filepath)
		 * @return A map containing the shader source for each shader type
		*/
		static std::unordered_map<unsigned int, std::string> PreProcess(const std::string &source);
		static unsigned int CompileShader(const std::string &source, unsigned int type);
	};
}