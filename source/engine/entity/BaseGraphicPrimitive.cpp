#include "BaseGraphicPrimitive.h"

void BaseMesh::setup()
{
}

void BaseMesh::setup(meshType meshType, meshDrawMethod meshDrawMethod, bool calculateNormals, bool calculateTangents)
{
	m_meshType = meshType;
	m_meshDrawMethod = meshDrawMethod;
	m_calculateNormals = calculateNormals;
	m_calculateTangents = calculateTangents;

	if (m_calculateNormals)
	{
		for (auto& l_vertices : m_vertices)
		{
			l_vertices.m_normal = l_vertices.m_pos;
		}
	}
}

const objectStatus & BaseMesh::getStatus() const
{
	return m_objectStatus;
}

meshID BaseMesh::getMeshID()
{
	return m_meshID;
}

void BaseMesh::addVertices(const Vertex & Vertex)
{
	m_vertices.emplace_back(Vertex);
}

void BaseMesh::addVertices(const vec3 & pos, const vec2 & texCoord, const vec3 & normal)
{
	m_vertices.emplace_back(Vertex(pos, texCoord, normal));
}

void BaseMesh::addVertices(double pos_x, double pos_y, double pos_z, double texCoord_x, double texCoord_y, double normal_x, double normal_y, double normal_z)
{
	m_vertices.emplace_back(Vertex(vec3(pos_x, pos_y, pos_z), vec2(texCoord_x, texCoord_y), vec3(normal_x, normal_y, normal_z)));
}

void BaseMesh::addIndices(unsigned int index)
{
	m_indices.emplace_back(index);
}

void BaseMesh::addUnitCube()
{
	Vertex l_VertexData_1;
	l_VertexData_1.m_pos = vec3(1.0f, 1.0f, 1.0f);
	l_VertexData_1.m_texCoord = vec2(1.0f, 1.0f);

	Vertex l_VertexData_2;
	l_VertexData_2.m_pos = vec3(1.0f, -1.0f, 1.0f);
	l_VertexData_2.m_texCoord = vec2(1.0f, 0.0f);

	Vertex l_VertexData_3;
	l_VertexData_3.m_pos = vec3(-1.0f, -1.0f, 1.0f);
	l_VertexData_3.m_texCoord = vec2(0.0f, 0.0f);

	Vertex l_VertexData_4;
	l_VertexData_4.m_pos = vec3(-1.0f, 1.0f, 1.0f);
	l_VertexData_4.m_texCoord = vec2(0.0f, 1.0f);

	Vertex l_VertexData_5;
	l_VertexData_5.m_pos = vec3(1.0f, 1.0f, -1.0f);
	l_VertexData_5.m_texCoord = vec2(1.0f, 1.0f);

	Vertex l_VertexData_6;
	l_VertexData_6.m_pos = vec3(1.0f, -1.0f, -1.0f);
	l_VertexData_6.m_texCoord = vec2(1.0f, 0.0f);

	Vertex l_VertexData_7;
	l_VertexData_7.m_pos = vec3(-1.0f, -1.0f, -1.0f);
	l_VertexData_7.m_texCoord = vec2(0.0f, 0.0f);

	Vertex l_VertexData_8;
	l_VertexData_8.m_pos = vec3(-1.0f, 1.0f, -1.0f);
	l_VertexData_8.m_texCoord = vec2(0.0f, 1.0f);


	m_vertices = { l_VertexData_1, l_VertexData_2, l_VertexData_3, l_VertexData_4, l_VertexData_5, l_VertexData_6, l_VertexData_7, l_VertexData_8 };

	for (auto& l_vertexData : m_vertices)
	{
		l_vertexData.m_normal = l_vertexData.m_pos.normalize();
	}
	m_indices = { 0, 1, 3, 1, 2, 3,
		4, 5, 0, 5, 1, 0,
		7, 6, 4, 6, 5, 4,
		3, 2, 7, 2, 6 ,7,
		4, 0, 7, 0, 3, 7,
		1, 5, 2, 5, 6, 2 };
}

void BaseMesh::addUnitSphere()
{
	unsigned int X_SEGMENTS = 64;
	unsigned int Y_SEGMENTS = 64;
	double PI = 3.14159265359;

	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			double xSegment = (double)x / (double)X_SEGMENTS;
			double ySegment = (double)y / (double)Y_SEGMENTS;
			double xPos = cos(xSegment * 2.0 * PI) * sin(ySegment * PI);
			double yPos = cos(ySegment * PI);
			double zPos = sin(xSegment * 2.0 * PI) * sin(ySegment * PI);

			Vertex l_VertexData;
			l_VertexData.m_pos = vec3(xPos, yPos, zPos);
			l_VertexData.m_texCoord = vec2(xSegment, ySegment);
			l_VertexData.m_normal = vec3(xPos, yPos, zPos).normalize();
			//l_VertexData.m_tangent = glm::normalize(glm::cross(glm::vec3(0.0, 0.0, 1.0), l_VertexData.m_normal));
			//l_VertexData.m_bitangent = glm::normalize(glm::cross(l_VertexData.m_tangent, l_VertexData.m_normal));
			m_vertices.emplace_back(l_VertexData);
		}
	}

	bool oddRow = false;
	for (unsigned y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (unsigned x = 0; x <= X_SEGMENTS; ++x)
			{
				m_indices.push_back(y       * (X_SEGMENTS + 1) + x);
				m_indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				m_indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				m_indices.push_back(y       * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
}

void BaseMesh::addUnitQuad()
{
	Vertex l_VertexData_1;
	l_VertexData_1.m_pos = vec3(1.0f, 1.0f, 0.0f);
	l_VertexData_1.m_texCoord = vec2(1.0f, 1.0f);

	Vertex l_VertexData_2;
	l_VertexData_2.m_pos = vec3(1.0f, -1.0f, 0.0f);
	l_VertexData_2.m_texCoord = vec2(1.0f, 0.0f);

	Vertex l_VertexData_3;
	l_VertexData_3.m_pos = vec3(-1.0f, -1.0f, 0.0f);
	l_VertexData_3.m_texCoord = vec2(0.0f, 0.0f);

	Vertex l_VertexData_4;
	l_VertexData_4.m_pos = vec3(-1.0f, 1.0f, 0.0f);
	l_VertexData_4.m_texCoord = vec2(0.0f, 1.0f);

	m_vertices = { l_VertexData_1, l_VertexData_2, l_VertexData_3, l_VertexData_4 };
	m_indices = { 0, 1, 3, 1, 2, 3 };
}

void BaseTexture::setup()
{
}


void BaseTexture::setup(textureType textureType, textureColorComponentsFormat textureColorComponentsFormat, texturePixelDataFormat texturePixelDataFormat, textureWrapMethod textureWrapMethod, textureFilterMethod textureMinFilterMethod, textureFilterMethod textureMagFilterMethod, int textureWidth, int textureHeight, texturePixelDataType texturePixelDataType, const std::vector<void*>& textureData)
{
	m_textureType = textureType;
	m_textureColorComponentsFormat = textureColorComponentsFormat;
	m_texturePixelDataFormat = texturePixelDataFormat;
	m_textureWrapMethod = textureWrapMethod;
	m_textureMinFilterMethod = textureMinFilterMethod;
	m_textureMagFilterMethod = textureMagFilterMethod;
	m_textureWidth = textureWidth;
	m_textureHeight = textureHeight;
	m_texturePixelDataType = texturePixelDataType;
	m_textureData = textureData;
}

const objectStatus & BaseTexture::getStatus() const
{
	return m_objectStatus;
}

textureID BaseTexture::getTextureID()
{
	return m_textureID;
}

int BaseTexture::getTextureWidth() const
{
	return m_textureWidth;
}

int BaseTexture::getTextureHeight() const
{
	return m_textureHeight;
}

void BaseShader::setup()
{
}

void BaseShader::setup(shaderType shaderType, const std::string& shaderFilePath, const std::vector<std::string>& attributions)
{
	m_shaderType = shaderType;
	m_shaderFilePath = shaderFilePath;
	m_attributions = attributions;

	m_objectStatus = objectStatus::ALIVE;
}

const objectStatus & BaseShader::getStatus() const
{
	return m_objectStatus;
}

const std::string & BaseShader::getShaderFilePath() const
{
	return m_shaderFilePath;
}

const std::vector<std::string> & BaseShader::getAttributions() const
{
	return m_attributions;
}

void BaseShader::parseAttribution()
{
}

void BaseShaderProgram::setup()
{
}

void BaseShaderProgram::setup(shaderTuple GLShaders)
{
	for (auto& i : GLShaders)
	{
		if (std::get<0>(i) == shaderType::VERTEX)
		{
			m_vertexShader->setup(std::get<0>(i), std::get<1>(i), std::get<2>(i));
		}
		else if (std::get<0>(i) == shaderType::GEOMETRY)
		{
			m_geometryShader->setup(std::get<0>(i), std::get<1>(i), std::get<2>(i));
		}
		else if (std::get<0>(i) == shaderType::FRAGMENT)
		{
			m_fragmentShader->setup(std::get<0>(i), std::get<1>(i), std::get<2>(i));
		}
	}

	m_objectStatus = objectStatus::ALIVE;
}

void BaseShaderProgram::update()
{
	g_pLogSystem->printLog("BaseShaderProgram: Warning: use the update() with parameter!");
}

const objectStatus & BaseShaderProgram::getStatus() const
{
	return m_objectStatus;
}

void BaseFrameBuffer::setup()
{
}

void BaseFrameBuffer::setup(vec2 renderBufferStorageResolution, frameBufferType frameBufferType, renderBufferType renderBufferType, unsigned int renderTargetTextureNumber)
{
	m_renderBufferStorageResolution = renderBufferStorageResolution;
	m_frameBufferType = frameBufferType;
	m_renderBufferType = renderBufferType;
	m_renderTargetTextureNumber = renderTargetTextureNumber;
}

const objectStatus & BaseFrameBuffer::getStatus() const
{
	return m_objectStatus;
}

void BaseFrameBufferWIP::setup()
{
}

void BaseFrameBufferWIP::setup(frameBufferType frameBufferType, renderBufferType renderBufferType, const std::vector<vec2>& renderBufferStorageSize, const std::vector<BaseTexture*>& renderTargetTextures, const std::vector<BaseShaderProgram*>& shaderPrograms, BaseFrameBufferWIP* previousBaseFrameBuffer)
{
	m_frameBufferType = frameBufferType;
	m_renderBufferType = renderBufferType;
	m_renderBufferStorageSize = renderBufferStorageSize;
	m_renderTargetTextures = renderTargetTextures;
	m_shaderPrograms = shaderPrograms;
	m_previousBaseFrameBuffer = previousBaseFrameBuffer;
}

const unsigned int BaseFrameBufferWIP::getRenderTargetNumber() const
{
	return m_renderTargetTextures.size();
}

const objectStatus & BaseFrameBufferWIP::getStatus() const
{
	return m_objectStatus;
}
