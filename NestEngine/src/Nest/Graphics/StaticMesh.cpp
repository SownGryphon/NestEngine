#include "StaticMesh.h"

#include <fstream>
#include <sstream>

Nest::StaticMesh::StaticMesh() :
	m_meshVA(), m_meshIB()
{}

Nest::StaticMesh::StaticMesh(const std::vector<chcl::Vector3<float>> &objVertices, const std::vector<chcl::Vector3<unsigned int>> &faceIndices)
{
	m_meshIB = createRef<IndexBuffer>(3 * faceIndices.size(), (unsigned int*)faceIndices.data());

	Ref<VertexBufferLayout> vbLayout = createRef<VertexBufferLayout>();
	vbLayout->push<float>(3);

	Ref<VertexBuffer> meshVB = createRef<VertexBuffer>(sizeof(chcl::Vector3<float>) * objVertices.size(), (void*)objVertices.data());

	m_meshVA = createRef<VertexArray>();
	m_meshVA->bindBuffers(meshVB, vbLayout);
}

Nest::StaticMesh::StaticMesh(
		const Buffer &buf, bool hasUVs, bool hasNormals,
		const std::vector<chcl::Vector3<unsigned int>> &faceIndices,
		const Ref<VertexBufferLayout> vbLayout
) :
	m_hasUV(hasUVs), m_hasNormals(hasNormals)
{
	m_meshIB = createRef<IndexBuffer>(3 * faceIndices.size(), (unsigned int*)faceIndices.data());

	Ref<VertexBuffer> meshVB = createRef<VertexBuffer>(buf.size(), buf.data());

	m_meshVA = createRef<VertexArray>();
	if (vbLayout)
		m_meshVA->bindBuffers(meshVB, vbLayout);
	else
	{
		Ref<VertexBufferLayout> vbLayoutDefault = createRef<VertexBufferLayout>();
		vbLayoutDefault->push<float>(3);
		if (m_hasUV)
			vbLayoutDefault->push<float>(2);
		if (m_hasNormals)
			vbLayoutDefault->push<float>(3);
		m_meshVA->bindBuffers(meshVB, vbLayoutDefault);
	}
}

Nest::StaticMesh Nest::StaticMesh::LoadOBJ(const std::string &filepath, bool loadUVs, bool loadNormals)
{
	size_t positionCount = 0, faceCount = 0, uvCount = 0, normalCount = 0;

	std::ifstream objFile{ filepath };

	std::string line;
	while (!objFile.eof())
	{
		std::getline(objFile, line);

		if (line == "")
			continue;

		if (line.find("v ") == 0)
		{
			++positionCount;
			continue;
		}

		if (loadUVs && line.find("vt ") == 0)
		{
			++uvCount;
			continue;
		}

		if (loadNormals && line.find("vn ") == 0)
		{
			++normalCount;
			continue;
		}

		if (line.find("f ") == 0)
		{
			++faceCount;
			continue;
		}
	}

	objFile.clear();
	objFile.seekg(0, std::ifstream::beg);

	size_t bufferSize = sizeof(chcl::Vector3<float>) * positionCount;
	if (uvCount || normalCount)
	{
		bufferSize = sizeof(chcl::Vector3<float>);
		if (uvCount)
			bufferSize += sizeof(chcl::Vector2<float>);
		if (normalCount)
			bufferSize += sizeof(chcl::Vector3<float>);
		bufferSize *= 3 * faceCount;
	}

	std::vector<chcl::Vector3<float>> vertexPos;
	vertexPos.reserve(positionCount);
	std::vector<chcl::Vector2<float>> vertexUVs;
	vertexUVs.reserve(uvCount);
	std::vector<chcl::Vector3<float>> vertexNormals;
	vertexNormals.reserve(normalCount);

	Buffer meshBuffer{ bufferSize };
	std::vector<chcl::Vector3<unsigned int>> faceIndices;
	faceIndices.reserve(faceCount);

	std::unordered_map<chcl::Vector3<int>, unsigned int> createdVertices;

	while (!objFile.eof())
	{
		std::getline(objFile, line);

		size_t commentBegin = line.find('#');
		line = line.substr(0, commentBegin);

		if (line == "")
			continue;

		std::vector<std::string> lineComponents = SplitString(line, ' ');

		// Strange blank lines or indented comments
		if (lineComponents.size() == 0)
			continue;

		if (lineComponents[0] == "v")
		{
			chcl::Vector3<float> pos;
			pos.x = std::stof(lineComponents[1]);
			pos.y = std::stof(lineComponents[2]);
			pos.z = std::stof(lineComponents[3]);

			vertexPos.push_back(pos);
			continue;
		}

		if (loadUVs && lineComponents[0] == "vt")
		{
			chcl::Vector2<float> uv;
			uv.x = std::stof(lineComponents[1]);
			uv.y = std::stof(lineComponents[2]);

			vertexUVs.push_back(uv);
			continue;
		}

		if (loadNormals && lineComponents[0] == "vn")
		{
			chcl::Vector3<float> normal;
			normal.x = std::stof(lineComponents[1]);
			normal.y = std::stof(lineComponents[2]);
			normal.z = std::stof(lineComponents[3]);

			vertexNormals.push_back(normal);
			continue;
		}

		if (lineComponents[0] == "f")
		{
			chcl::Vector3<unsigned int> indices;

			for (size_t i = 1; i < lineComponents.size(); ++i)
			{
				std::vector<std::string> indexComponents = SplitString(lineComponents[i], '/');

				int posIndex = std::stoi(indexComponents[0]), uvIndex = 0, normalIndex = 0;
				if (posIndex > 0)
					--posIndex;
				else
					posIndex = (int)vertexPos.size() + posIndex;
				if (uvCount && indexComponents[1] != "")
				{
					uvIndex = std::stoi(indexComponents[1]);
					if (uvIndex > 0)
						--uvIndex;
					else
						uvIndex = (int)vertexUVs.size() + uvIndex;
				}
				if (normalCount && indexComponents[2] != "")
				{
					normalIndex = std::stoi(indexComponents[2]);
					if (normalIndex > 0)
						--normalIndex;
					else
						normalIndex = (int)vertexNormals.size() + normalIndex;
				}

				chcl::Vector3<int> vertexComponentIndices{ posIndex, uvIndex, normalIndex };
				unsigned int vertIndex = 0;
				if (createdVertices.contains(vertexComponentIndices))
				{
					vertIndex = createdVertices[vertexComponentIndices];
				}
				else
				{
					vertIndex = (unsigned int)createdVertices.size();
					meshBuffer.push_back(vertexPos[posIndex]);
					if (uvCount)
						meshBuffer.push_back(vertexUVs[uvIndex]);
					if (normalCount)
						meshBuffer.push_back(vertexNormals[normalIndex]);
					createdVertices[vertexComponentIndices] = vertIndex;
				}

				if (i <= 3)
				{
					indices[i - 1] = vertIndex;
				}
				else
				{
					faceIndices.push_back(indices);
					indices.y = indices.z;
					indices.z = vertIndex;
				}
			}

			faceIndices.push_back(indices);
			continue;
		}
	}

	objFile.close();

	return StaticMesh(meshBuffer, uvCount, normalCount, faceIndices);
}

std::vector<std::string> Nest::StaticMesh::SplitString(const std::string &str, char delim)
{
	std::vector<std::string> components;
	std::istringstream lineSplit{ str };

	while (!lineSplit.eof())
	{
		components.push_back("");
		std::getline(lineSplit, components.back(), delim);
		if (components.back() == "")
			components.pop_back();
	}

	return components;
}