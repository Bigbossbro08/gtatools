/*
 * Mesh.cpp
 *
 *  Created on: 21.10.2010
 *      Author: alemariusnexus
 */

#include "Mesh.h"
#include "Engine.h"
#include "Submesh.h"



Mesh::Mesh(int vertexCount, int flags, const float* vertices, const float* normals, const float* texCoords,
		const uint8_t* vertexColors)
		: vertexCount(vertexCount)
{
	init(flags, vertices, normals, texCoords, vertexColors);
}


Mesh::Mesh(const DFFGeometry& geometry)
		: vertexCount(geometry.getVertexCount())
{
	flags = 0;
	const float* vertices = geometry.getVertices();
	const float* normals = geometry.getNormals();
	const uint8_t* colors = geometry.getVertexColors();
	const float* texCoords = NULL;

	if (geometry.getUVSetCount() > 0) {
		texCoords = geometry.getUVCoordSet(0);
	}

	if (geometry.isTriangleStripFormat()) {
		flags |= MeshTriangleStrips;
	}
	if (normals) {
		flags |= MeshNormals;
	}
	if (texCoords) {
		flags |= MeshTexCoords;
	}
	if (colors) {
		flags |= MeshVertexColors;
	}

	DFFGeometry::ConstMaterialIterator it;
	for (it = geometry.getMaterialBegin() ; it != geometry.getMaterialEnd() ; it++) {
		Material* material = new Material(**it);
		addMaterial(material);
	}

	DFFGeometry::ConstPartIterator pit;
	for (pit = geometry.getPartBegin() ; pit != geometry.getPartEnd() ; pit++) {
		Submesh* submesh = new Submesh(this, **pit);
		addSubmesh(submesh);
	}

	const DFFBoundingSphere* b = geometry.getBounds();
	setBounds(b->x, b->y, b->z, b->radius);

	init(flags, vertices, normals, texCoords, colors);
}


void Mesh::init(int flags, const float* vertices, const float* normals, const float* texCoords,
		const uint8_t* vertexColors)
{
	int bufferSize = 0;

	if (vertices) {
		bufferSize += vertexCount*3*4;
	}
	if (normals) {
		bufferSize += vertexCount*3*4;
	}
	if (texCoords) {
		bufferSize += vertexCount*2*4;
	}
	if (vertexColors) {
		bufferSize += vertexCount*4;
	}

	glGenBuffers(1, &dataBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);

	int offset = 0;

	if (vertices) {
		glBufferSubData(GL_ARRAY_BUFFER, offset, vertexCount*3*4, vertices);
		offset += vertexCount*3*4;
	}
	if (normals) {
		normalOffs = offset;
		glBufferSubData(GL_ARRAY_BUFFER, offset, vertexCount*3*4, normals);
		offset += vertexCount*3*4;
	}
	if (texCoords) {
		texCoordOffs = offset;
		glBufferSubData(GL_ARRAY_BUFFER, offset, vertexCount*2*4, texCoords);
		offset += vertexCount*2*4;
	}
	if (vertexColors) {
		vertexColorOffs = offset;
		glBufferSubData(GL_ARRAY_BUFFER, offset, vertexCount*4, vertexColors);
		offset += vertexCount*4;
	}
}


void Mesh::bindDataBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
}


void Mesh::addSubmesh(Submesh* submesh)
{
	submeshes.push_back(submesh);
}


void Mesh::addMaterial(Material* material)
{
	materials.push_back(material);
}