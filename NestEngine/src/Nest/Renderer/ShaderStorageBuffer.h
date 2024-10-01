#pragma once

#include "Nest/Core/Buffer.h"

namespace Nest
{
	/// Class for OpenGL SSBOs
	class ShaderStorageBuffer
	{
	public:
		ShaderStorageBuffer();
		ShaderStorageBuffer(const char* data, size_t dataSize);
		ShaderStorageBuffer(const Buffer& buffer);

		ShaderStorageBuffer(const ShaderStorageBuffer&) = delete;
		ShaderStorageBuffer(ShaderStorageBuffer&& other) noexcept;

		~ShaderStorageBuffer();

		void bind() const;
		void unbind() const;

		ShaderStorageBuffer& operator=(const ShaderStorageBuffer&) = delete;
		ShaderStorageBuffer& operator=(ShaderStorageBuffer&&) noexcept;

		inline unsigned int getID() const { return m_rendererID; }

	private:

		unsigned int m_rendererID;
	};
}