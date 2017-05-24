#include "Model.h"



Model::Model()
{
}


Model::~Model()
{
}

bool Model::buffer(std::string objFile)
{
	std::vector<vec3> locs;
	std::vector<vec2> uvs;
	std::vector<vec3> norms;
	std::vector<VertInd> verts;

	std::vector<vec3> locsHold;
	std::vector<vec2> uvsHold;
	std::vector<vec3> normsHold;

	std::ifstream inFile;
	std::string line;

	inFile.open(objFile);
	if (inFile.is_open())
	{
		while (getline(inFile, line))
		{
			std::istringstream ss(line);
			std::string label;
			ss >> label;

			if (label == "v")
			{
				vec3 vertex;
				float vx;
				float vy;
				float vz;

				ss >> vx >> vy >> vz;
				vertex.x = vx;
				vertex.y = vy;
				vertex.z = vz;
				locsHold.push_back(vertex);
				// finding max values
				if (vx > xMax)
				{
					xMax = vx;
				}
				if (vy > yMax)
				{
					yMax = vy;
				}
				if (vz > zMax)
				{
					zMax = vz;
				}

				// finding min values
				if (vx < xMin)
				{
					xMin = vx;
				}
				if (vy < yMin)
				{
					yMin = vy;
				}
				if (vz < zMin)
				{
					zMin = vz;
				}
			}
			else if (label == "vt")
			{
				vec2 uv;
				float vx;
				float vy;
				ss >> vx >> vy;
				uv.x = vx;
				uv.y = vy;
				uvsHold.push_back(uv);
			}
			else if (label == "vn")
			{
				vec3 normal;
				float vx;
				float vy;
				float vz;
				ss >> vx >> vy >> vz;
				normal.x = vx;
				normal.y = vy;
				normal.z = vz;
				normsHold.push_back(normal);
			}
			else if (label == "f")
			{
				unsigned int vertexIndex[3];
				unsigned int uvIndex[3];
				unsigned int normalIndex[3];
				unsigned int vx;
				unsigned int vy;
				unsigned int vz;

				char dasha;
				char dashb;
				for (int i = 0; i < 3; i++)
				{
					ss >> vx >> dasha >> vy >> dashb >> vz;
					vertexIndex[i] = vx;
					uvIndex[i] = vy;
					normalIndex[i] = vz;

					VertInd tempVert;
					tempVert.locIndex = vertexIndex[i];
					tempVert.uvIndex = uvIndex[i];
					tempVert.normIndex = normalIndex[i];
					verts.push_back(tempVert);
				}
			}
		}

		inFile.close();
	}
	else return false;

	vertCount = verts.size();
	std::vector<Vertex> vertBufData(vertCount);

	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = { locsHold[verts[i].locIndex - 1], uvsHold[verts[i].uvIndex - 1], normsHold[verts[i].normIndex - 1] };
	}

	// Generating a Buffer

	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	// Bind the buffer
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	// Storing data in the buffer
	glBufferData(GL_ARRAY_BUFFER,  // WHere to copy to 
		sizeof(Vertex) * vertCount, // # of bytes to copy
		&vertBufData[0],			// Where to copy from
		GL_STATIC_DRAW);			// "hint: to OpenGL

	//loc								// Step 4: Describe the BUffer Layout
	glEnableVertexAttribArray(0); // Attribut index - 0

	//loc							  // set up the attribute
	glVertexAttribPointer(
		0,					// Attribute index
		3,					// Number of components (x, y, z) 
		GL_FLOAT,			// Type of data
		GL_FALSE,			// Should we normalize the data? 
		sizeof(Vertex),		// Stride (bytes per vertex)
		0);					// offset to this attribute
	// uv
	glEnableVertexAttribArray(3);
	// uv
	glVertexAttribPointer(
		3,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(vec3)));

	// norm
	glEnableVertexAttribArray(4);
	// norm
	glVertexAttribPointer(
		4,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(vec3) + sizeof(vec2)));


	glBindVertexArray(0);
	return true;

}

void Model::render()
{
	glUniform3f(7, objColor.x, objColor.y, objColor.z);
	// Render Game objects
	glBindVertexArray(vertArr);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
	glBindVertexArray(0);
}
