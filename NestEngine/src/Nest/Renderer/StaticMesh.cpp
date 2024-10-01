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

	Ref<VertexBuffer> meshVB = createRef<VertexBuffer>(buf.size(), buf.begin());

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
		bufferSize *= 3 * faceCount + 1;
	}

	std::vector<chcl::Vector3<float>> vertexPos;
	vertexPos.reserve(positionCount + 1);
	vertexPos.push_back({ 0.f, 0.f, 0.f });
	std::vector<chcl::Vector2<float>> vertexUVs;
	vertexUVs.reserve(uvCount + 1);
	vertexUVs.push_back({ 0.f, 0.f });
	std::vector<chcl::Vector3<float>> vertexNormals;
	vertexNormals.reserve(normalCount + 1);
	vertexNormals.push_back({ 0.f, 0.f, 0.f });

	Buffer meshBuffer{ bufferSize };
	meshBuffer.push_back(vertexPos[0]);
	if (uvCount)
		meshBuffer.push_back(vertexUVs[0]);
	if (normalCount)
		meshBuffer.push_back(vertexNormals[0]);
	std::vector<chcl::Vector3<unsigned int>> faceIndices;
	faceIndices.reserve(faceCount);

	std::unordered_map<chcl::Vector3<int>, unsigned int> createdVertices;

	while (!objFile.eof())
	{
		#if 0
		char nextChar = objFile.get();

		switch (nextChar)
		{
			case '#':
				break;
			case 'v':
			{
				nextChar = objFile.get();
				switch (nextChar)
				{
					case 'n':
					{
						chcl::Vector3<float> normal;
						objFile >> normal.x >> normal.y >> normal.z;
						vertexNormals.push_back(normal);
						break;
					}
					case 't':
					{
						chcl::Vector2<float> uv;
						objFile >> uv.x >>  uv.y;
						vertexUVs.push_back(uv);
						break;
					}
					case 'p':
					{
						break;
					}
					case '\n':
						objFile.unget();
						break;
					default:
					{
						objFile.unget();
						chcl::Vector3<float> pos;
						objFile >> pos.x >> pos.y >> pos.z;
						vertexPos.push_back(pos);
						break;
					}
				}
				break;
			}
			case 'f':
			{
				chcl::Vector3<unsigned int> indices;

				unsigned int indicesRead = 0;
				while (objFile.peek() != '\n')
				{
					int posIndex = 0, uvIndex = 0, normalIndex = 0;
					objFile >> posIndex;

					if (posIndex < 0)
						posIndex = (int)vertexPos.size() + posIndex;

					nextChar = objFile.get();
					if (nextChar == '/')
					{
						objFile >> uvIndex;
						if (objFile.fail())
							objFile.clear();

						if (uvIndex < 0)
							uvIndex = (int)vertexUVs.size() + uvIndex;

						if (!uvCount)
							uvIndex = 0;
					}
					else
						objFile.unget();

					nextChar = objFile.get();
					if (nextChar == '/')
					{
						objFile >> normalIndex;
						if (normalIndex < 0)
							normalIndex = (int)vertexNormals.size() + normalIndex;

						if (!normalCount)
							normalIndex = 0;
					}
					else
						objFile.unget();

					chcl::Vector3<int> vertexComponentIndices{ posIndex, uvIndex, normalIndex };
					unsigned int vertIndex = 0;
					if (createdVertices.contains(vertexComponentIndices))
					{
						vertIndex = createdVertices[vertexComponentIndices];
					}
					else
					{
						vertIndex = (unsigned int)createdVertices.size() + 1;
						meshBuffer.push_back(vertexPos[posIndex]);
						if (uvCount)
							meshBuffer.push_back(vertexUVs[uvIndex]);
						if (normalCount)
							meshBuffer.push_back(vertexNormals[normalIndex]);
						createdVertices[vertexComponentIndices] = vertIndex;
					}

					if (indicesRead < 3)
					{
						indices[indicesRead] = vertIndex;
					}
					else
					{
						faceIndices.push_back(indices);
						indices.y = indices.z;
						indices.z = vertIndex;
					}
					++indicesRead;
	
					if (indicesRead >= 3)
						faceIndices.push_back(indices);

					while (std::isspace(objFile.peek()) && objFile.peek() != '\n')
						objFile.get();
				}

				break;
			}
			case '\n':
				objFile.unget();
				break;
		}

		objFile >> std::ws;
		#endif

		#if 1
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
				if (posIndex < 0)
					posIndex = (int)vertexPos.size() + posIndex;

				if (uvCount && indexComponents[1] != "")
				{
					uvIndex = std::stoi(indexComponents[1]);
					if (uvIndex < 0)
						uvIndex = (int)vertexUVs.size() + uvIndex;
				}

				if (normalCount && indexComponents[2] != "")
				{
					normalIndex = std::stoi(indexComponents[2]);
					if (normalIndex < 0)
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
					vertIndex = (unsigned int)createdVertices.size() + 1;
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
		#endif
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