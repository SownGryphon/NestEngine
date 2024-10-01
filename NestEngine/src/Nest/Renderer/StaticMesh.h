#pragma once

#include <string>
#include <vector>

#include <geometry/Vector3.h>

#include "Nest/Core/Buffer.h"
#include "Nest/renderer/VertexArray.h"
#include "Nest/renderer/IndexBuffer.h"

namespace Nest
{
	class StaticMesh
	{
	public:
		StaticMesh();

		/**
		 * @brief Mesh from vertices and indices
		 * @param objVertices Array of vertices makig up the mesh
		 * @param faceIndices Array of vertex indices makking up mesh faces
		 */
		StaticMesh(const std::vector<chcl::Vector3<float>> &objVertices, const std::vector<chcl::Vector3<unsigned int>> &faceIndices);

		/**
		 * @brief Mesh from arbitrary data buffer
		 * @param buf Buffer of vertex data
		 * @param hasUVs Whether the buffer contains texture UV coordinates
		 * @param hasNormals Whether the buffer contains model normal vectors
		 * @param faceIndices Optional index buffer. If not provided, mesh is drawn unindexed.
		 * @param vbLayout Optional layout of data in buffer. Default layout is (vertex position, 3 floats) [texture UVs, 2 floats] [mesh normals, 3 floats]
		 */
		StaticMesh(
			const Buffer &buf, bool hasUVs, bool hasNormals,
			const std::vector<chcl::Vector3<unsigned int>> &faceIndices = {},
			const Ref<VertexBufferLayout> vbLayout = nullptr
		);

		inline Ref<VertexArray> getVA() { return m_meshVA; }
		inline Ref<const VertexArray> getVA() const { return m_meshVA; }
		inline Ref<IndexBuffer> getIB() { return m_meshIB; }
		inline Ref<const IndexBuffer> getIB() const { return m_meshIB; }

		static StaticMesh LoadOBJ(const std::string &filepath, bool loadUVs = true, bool loadNormals = true);

	private:
		bool m_hasUV = false, m_hasNormals = false;
		Ref<VertexArray> m_meshVA;
		Ref<IndexBuffer> m_meshIB;

		static std::vector<std::string> SplitString(const std::string &str, char delim);
	};
}