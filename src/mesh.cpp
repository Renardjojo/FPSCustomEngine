#include "GE/Ressources/mesh.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/Parsers/objParser.hpp"

#include <iostream>

using namespace Engine::Core;
using namespace Engine::Core::Maths;
using namespace Engine::Core::Parsers;
using namespace Engine::Core::Debug;
using namespace Engine::Ressources;


Mesh::Mesh (const char* objPath, bool loadInGPU)
    :   indexVAO_    (0),
        isLoadInGPU_ (false)
{
	Attrib attrib;
	std::vector<Shape> shapes;

    loadObjWithMTL (objPath, &attrib, &shapes, nullptr);

	objName_        = attrib.objName;
	vBuffer_        = attrib.vBuffer;
	vtBuffer_       = attrib.vtBuffer;
	vnBuffer_       = attrib.vnBuffer;

	iBuffer_.reserve(shapes.size());
	idMaterial_.reserve(shapes.size());

	if (vnBuffer_.empty())
	{
		generateNormalAndLoadIndice(shapes);
	}
	else
	{
		for (auto&& shape : shapes)
		{
		iBuffer_.push_back      ({});
		for (size_t i = 0; i < shape.iv.size(); i++)
		{		
			iBuffer_.back().push_back      ({shape.iv[i], shape.ivt[i], shape.ivn[i]});
		}
		
		if (!shape.material_ids.empty())
			idMaterial_.push_back	(shape.material_ids);	
		}	
	}

    if(loadInGPU)
        Mesh::loadInGPU();
}

Mesh::Mesh (MeshConstructorArg meshArg, bool loadInGPU)
    :   indexVAO_       (0),
        isLoadInGPU_    (false),
        objName_        (meshArg.objName),
        vBuffer_        (meshArg.vBuffer),
        vtBuffer_       (meshArg.vtBuffer),
        vnBuffer_       (meshArg.vnBuffer),
        iBuffer_        (meshArg.iBuffer)
{
    if(loadInGPU)
        Mesh::loadInGPU();
}

Mesh::Mesh (const Attrib& attrib, const std::vector<Shape>& shapes, bool loadInGPU)
    :   indexVAO_       (0),
        isLoadInGPU_    (false),
        objName_        (attrib.objName),
        vBuffer_        (attrib.vBuffer),
        vtBuffer_       (attrib.vtBuffer),
        vnBuffer_       (attrib.vnBuffer),
        iBuffer_        (),
		idMaterial_		()
{
	iBuffer_.reserve(shapes.size());
	idMaterial_.reserve(shapes.size());

	if (vnBuffer_.empty())
	{
		generateNormalAndLoadIndice(shapes);
	}
	else
	{
		for (auto&& shape : shapes)
		{
		iBuffer_.push_back      ({});
		for (size_t i = 0; i < shape.iv.size(); i++)
		{		
			iBuffer_.back().push_back      ({shape.iv[i], shape.ivt[i], shape.ivn[i]});
		}
		
		if (!shape.material_ids.empty())
			idMaterial_.push_back	(shape.material_ids);	
		}	
	}

    if(loadInGPU)
        Mesh::loadInGPU();
}


Mesh::~Mesh ()
{
    if (isLoadInGPU_)
        unloadFromGPU();
}

void initializeVertexBuffer(GLuint &buffer, GLenum target,  GLenum usage, const void* data,  int size)
{
	glGenBuffers(1, &buffer);
	glBindBuffer(target, buffer);
	glBufferData(target, size, data, usage);
	glBindBuffer(target, 0);
}

void Mesh::loadInGPU () noexcept
{
    if (isLoadInGPU_)
    {
        functWarning((std::string("Mesh name's : ") + objName_.c_str() + " already load in GPU").c_str());
        return;
    }

    std::vector<Vec3>  vVBO_;
    std::vector<Vec2>  vtVBO_;
    std::vector<Vec3>  vnVBO_;

    unsigned int nbVertex = 0;
    for (size_t part = 0; part < iBuffer_.size(); part++)
    {
		nbVertex += iBuffer_[part].size();;
	}

    vVBO_.  reserve(nbVertex);
    vtVBO_. reserve(nbVertex);
    vnVBO_. reserve(nbVertex);

    for (size_t part = 0; part < iBuffer_.size(); part++)
    {
		for (size_t i = 0; i < iBuffer_[part].size(); i++)
		{
			vVBO_. push_back(vBuffer_[iBuffer_[part][i].iv]);
			vtVBO_.push_back(vtBuffer_[iBuffer_[part][i].ivt]);
			vnVBO_.push_back(vnBuffer_[iBuffer_[part][i].ivn]);
		}
	}

    //Init VBOs and VAO
    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    //GLuint EBOid;
    initializeVertexBuffer(vertexbuffer, GL_ARRAY_BUFFER, GL_STATIC_DRAW, &vVBO_[0], vVBO_.size() * sizeof(Vec3));
    initializeVertexBuffer(uvbuffer, GL_ARRAY_BUFFER, GL_STATIC_DRAW, &vtVBO_[0], vtVBO_.size() * sizeof(Vec2));
    initializeVertexBuffer(normalbuffer, GL_ARRAY_BUFFER, GL_STATIC_DRAW, &vnVBO_[0], vnVBO_.size() * sizeof(Vec3));

    //Generate VAO
    glGenVertexArrays(1, &indexVAO_);
    glBindVertexArray(indexVAO_);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 2nd attribute buffer : normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 3nd attribute buffer : UVs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    isLoadInGPU_ = true;
    SLog::log((std::string("Mesh ") + objName_.c_str() + " load in GPU").c_str());
}

void Mesh::unloadFromGPU () noexcept
{
    if (!isLoadInGPU_)
    {
        functWarning((std::string("Mesh name's : ") + objName_.c_str() + " isn't load in GPU").c_str());
        return;
    }

    //search all VBO attach to the current VAO and destroy it.
    GLint nAttr = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttr);
    glBindVertexArray(indexVAO_);
    for (int iAttr = 0; iAttr < nAttr; ++iAttr)
    {
        GLuint vboId = 0;
        glGetVertexAttribiv(iAttr, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, (GLint*)&vboId);
        if (vboId > 0) 
        {
            glDeleteBuffers(1, &vboId);
        }
    }

	glDeleteBuffers(1, &indexVAO_);
	indexVAO_ = 0;

    isLoadInGPU_ = false;
}

void Mesh::draw () const noexcept
{
    glBindVertexArray(indexVAO_);

	unsigned int first = 0;
    for (size_t part = 0; part < iBuffer_.size(); part++)
    {
		glDrawArrays(GL_TRIANGLES, first, iBuffer_[part].size());
		first += iBuffer_[part].size();
	}
}

MeshConstructorArg Mesh::createPlane	(float textureRepetition, unsigned int indexTextureX, unsigned int indexTextureY, Axis towardAxis)
{
	MeshConstructorArg mesh;
    mesh.objName = "Plane";

  	//plane contain 4 triangle, 4 vertex 4 texture coordonate and 2 normal
    mesh.vBuffer    .reserve(4);
    mesh.vtBuffer   .reserve(4);
    mesh.vnBuffer   .reserve(2);
	mesh.iBuffer	.push_back({});
    mesh.iBuffer    .reserve(2);

	//Face 1
	mesh.iBuffer	[0].emplace_back(Indice{0, 0, 0});
    mesh.iBuffer	[0].emplace_back(Indice{1, 1, 0});
    mesh.iBuffer	[0].emplace_back(Indice{3, 2, 0});
	mesh.iBuffer	[0].emplace_back(Indice{1, 1, 0});
    mesh.iBuffer	[0].emplace_back(Indice{2, 3, 0});
    mesh.iBuffer	[0].emplace_back(Indice{3, 2, 0});

	//Face 2
	mesh.iBuffer	[0].emplace_back(Indice{0, 0, 1});
    mesh.iBuffer	[0].emplace_back(Indice{1, 1, 1});
    mesh.iBuffer	[0].emplace_back(Indice{3, 2, 1});
	mesh.iBuffer	[0].emplace_back(Indice{1, 1, 1});
    mesh.iBuffer	[0].emplace_back(Indice{2, 3, 1});
    mesh.iBuffer	[0].emplace_back(Indice{3, 2, 1});

	//initialize vertex :

	switch (towardAxis)
	{
	case Axis::X:
		mesh.vBuffer	.push_back({0.f, -0.5f, -0.5});
		mesh.vBuffer	.push_back({0.f,  0.5f, -0.5});
		mesh.vBuffer	.push_back({0.f,  0.5f,  0.5});
		mesh.vBuffer	.push_back({0.f, -0.5f,  0.5});
		break;

	case Axis::Y:
		mesh.vBuffer	.push_back({-0.5f, 0.f, -0.5});
		mesh.vBuffer	.push_back({ 0.5f, 0.f, -0.5});
		mesh.vBuffer	.push_back({ 0.5f, 0.f,  0.5});
		mesh.vBuffer	.push_back({-0.5f, 0.f,  0.5});
		break;
		
	case Axis::Z:
		mesh.vBuffer	.push_back({-0.5f, -0.5, 0.f});
		mesh.vBuffer	.push_back({ 0.5f, -0.5, 0.f});
		mesh.vBuffer	.push_back({ 0.5f,  0.5, 0.f});
		mesh.vBuffer	.push_back({-0.5f,  0.5, 0.f});
		break;
	
	default:
		functWarning(std::string("Other axis not implemented"))
		break;
	}

	//initialize texture coord : 
	float shiftX = indexTextureX * textureRepetition;
	float shiftY = indexTextureY * textureRepetition;
	mesh.vtBuffer	.push_back({ shiftX, shiftY});
	mesh.vtBuffer	.push_back({ shiftX + textureRepetition, shiftY});
	mesh.vtBuffer	.push_back({ shiftX , shiftY + textureRepetition});
	mesh.vtBuffer	.push_back({ shiftX + textureRepetition, shiftY + textureRepetition});

	//initialize normal :
	mesh.vnBuffer	.push_back({ 0.f, -1.f,  0.f});
	mesh.vnBuffer	.push_back({ 0.f, 1.f,  0.f});

	return mesh;
}

MeshConstructorArg Mesh::createCube	(float textureRepetition)
{
	MeshConstructorArg mesh;
    mesh.objName = "Cube";

	//cube contain 12 triangle, 8 vertex 4 texture coordonate and 6 normal
    mesh.vBuffer    .reserve(8);
    mesh.vtBuffer   .reserve(4);
    mesh.vnBuffer   .reserve(6);
	mesh.iBuffer.push_back({});
    mesh.iBuffer    .reserve(6);

	
	//initialize the index of cube :
	//Face 1
	mesh.iBuffer	[0].emplace_back(Indice{0, 0, 0});
    mesh.iBuffer	[0].emplace_back(Indice{1, 1, 0});
    mesh.iBuffer	[0].emplace_back(Indice{2, 2, 0});
	mesh.iBuffer	[0].emplace_back(Indice{2, 2, 0});
    mesh.iBuffer	[0].emplace_back(Indice{1, 1, 0});
    mesh.iBuffer	[0].emplace_back(Indice{3, 3, 0});

	//Face 2
	mesh.iBuffer	[0].emplace_back(Indice{2, 0, 1});
    mesh.iBuffer	[0].emplace_back(Indice{3, 1, 1});
    mesh.iBuffer	[0].emplace_back(Indice{4, 2, 1});
	mesh.iBuffer	[0].emplace_back(Indice{4, 2, 1});
    mesh.iBuffer	[0].emplace_back(Indice{3, 1, 1});
    mesh.iBuffer	[0].emplace_back(Indice{5, 3, 1});

	//Face 3
	mesh.iBuffer	[0].emplace_back(Indice{4, 3, 2});
    mesh.iBuffer	[0].emplace_back(Indice{5, 2, 2});
    mesh.iBuffer	[0].emplace_back(Indice{6, 1, 2});
	mesh.iBuffer	[0].emplace_back(Indice{6, 1, 2});
    mesh.iBuffer	[0].emplace_back(Indice{5, 2, 2});
    mesh.iBuffer	[0].emplace_back(Indice{7, 0, 2});

	//Face 4
	mesh.iBuffer	[0].emplace_back(Indice{6, 0, 3});
    mesh.iBuffer	[0].emplace_back(Indice{7, 1, 3});
    mesh.iBuffer	[0].emplace_back(Indice{0, 2, 3});
	mesh.iBuffer	[0].emplace_back(Indice{0, 2, 3});
    mesh.iBuffer	[0].emplace_back(Indice{7, 1, 3});
    mesh.iBuffer	[0].emplace_back(Indice{1, 3, 3});

	//Face 5
	mesh.iBuffer	[0].emplace_back(Indice{1, 0, 4});
    mesh.iBuffer	[0].emplace_back(Indice{7, 1, 4});
    mesh.iBuffer	[0].emplace_back(Indice{3, 2, 4});
	mesh.iBuffer	[0].emplace_back(Indice{3, 2, 4});
    mesh.iBuffer	[0].emplace_back(Indice{7, 1, 4});
    mesh.iBuffer	[0].emplace_back(Indice{5, 3, 4});

	//Face 6
	mesh.iBuffer	[0].emplace_back(Indice{6, 0, 5});
    mesh.iBuffer	[0].emplace_back(Indice{0, 1, 5});
    mesh.iBuffer	[0].emplace_back(Indice{4, 2, 5});
	mesh.iBuffer	[0].emplace_back(Indice{4, 2, 5});
    mesh.iBuffer	[0].emplace_back(Indice{0, 1, 5});
    mesh.iBuffer	[0].emplace_back(Indice{2, 3, 5});

	//initialize vertex :
	mesh.vBuffer	.push_back({-0.5f, -0.5f,  0.5});
	mesh.vBuffer	.push_back({ 0.5f, -0.5f,  0.5});
	mesh.vBuffer	.push_back({-0.5f,  0.5f,  0.5});
	mesh.vBuffer	.push_back({ 0.5f,  0.5f,  0.5});
	mesh.vBuffer	.push_back({-0.5f,  0.5f, -0.5});
	mesh.vBuffer	.push_back({ 0.5f,  0.5f, -0.5});
	mesh.vBuffer	.push_back({-0.5f, -0.5f, -0.5});
	mesh.vBuffer	.push_back({ 0.5f, -0.5f, -0.5});

	//initialize texture coord : 
	mesh.vtBuffer	.push_back({ 0.f, 0.f});
	mesh.vtBuffer	.push_back({ textureRepetition, 0.f});
	mesh.vtBuffer	.push_back({ 0.f, textureRepetition});
	mesh.vtBuffer	.push_back({ textureRepetition, textureRepetition});

	//initialize normal :
	mesh.vnBuffer	.push_back({ 0.f,  0.f,  1.f});
	mesh.vnBuffer	.push_back({ 0.f,  1.f,  0.f});
	mesh.vnBuffer	.push_back({ 0.f,  0.f, -1.f});
	mesh.vnBuffer	.push_back({ 0.f, -1.f,  0.f});
	mesh.vnBuffer	.push_back({ 1.f,  0.f,  0.f});
	mesh.vnBuffer	.push_back({-1.f,  0.f,  0.f});

	return mesh;
}

MeshConstructorArg Mesh::createSphere(int latitudeCount, int longitudeCount)
{	
    GE_assert(latitudeCount > 2 && longitudeCount > 2);

	MeshConstructorArg mesh;
    mesh.objName = "Sphere";
	mesh.iBuffer.push_back({});

	latitudeCount *= 2.f;

	float latitudeStep = 2.f * M_PI / latitudeCount;
	float longitudeStep = M_PI / longitudeCount;

	//found each point of sphere in function of latitude and longitude count in parameter
	//souce to more informaiton : http://www.songho.ca/opengl/gl_sphere.html
	{
		float xy;
		float latitudeAngle, longitudeAngle;
		float radius = 0.5f; 			// radius of 1
		float posX, posY, posZ; 		//position of point
		float s, t;						//few constant for texture position

		for(unsigned int i = 0; i <= static_cast<unsigned int>(longitudeCount); i++)
		{
			longitudeAngle = M_PI / 2.f - i * longitudeStep;    // starting from pi/2 to -pi/2
			xy = radius * cosf(longitudeAngle);
			posZ = radius * sinf(longitudeAngle);

			for(unsigned int j = 0; j <= static_cast<unsigned int>(latitudeCount); j++)
			{
				latitudeAngle = j * latitudeStep;           // starting from 0 to 2pi
				posX = xy * cosf(latitudeAngle);
				posY = xy * sinf(latitudeAngle);
				s = (float)j / latitudeCount;
				t = (float)i / longitudeCount;

				// vertex position (x, y, z)
				mesh.vBuffer		.push_back({ posX, posY, posZ});
				mesh.vnBuffer		.push_back({ posX * radius, posY * radius, posZ * radius});
				mesh.vtBuffer   	.push_back({s, t});
				
				mesh.vnBuffer.back().normalize();
			}
		}
	}

	//generate each indices in function of point create upper
	{
		unsigned int ver1, ver2; //vertices with differente longitude. ver2 = longitude +1

		for(unsigned int i = 0; i < static_cast<unsigned int>(longitudeCount); i++)
		{
			ver1 = i * (latitudeCount + 1);
			ver2 = ver1 + latitudeCount + 1;  //ver2 = longitude + 1
 
			for(unsigned int j = 0; j < static_cast<unsigned int>(latitudeCount); j++, ver1++, ver2++)
			{
				if(i != 0)
				{
					// ver1 -> ver2 -> ver1+1 : like this shema
					// ver1	 <-	ver1+1
					//	|	   /	 
					// ver2	 

					//to create triangle like shema
				    mesh.iBuffer[0].push_back(Indice{ver1, ver1, ver1});
                    mesh.iBuffer[0].push_back(Indice{ver2, ver2, ver2});
                    mesh.iBuffer[0].push_back(Indice{ver1 + 1, ver1 + 1, ver1 + 1});
				}

				if(i != static_cast<unsigned int>(longitudeCount)-1)
				{
					// ver1+1 -> ver2 -> ver2+1 : like this shema
					//			ver1+1
					//		 /	  |
					// ver2	 ->	ver2+1

                    mesh.iBuffer[0].push_back(Indice{ver1 + 1, ver1 + 1, ver1 + 1});
                    mesh.iBuffer[0].push_back(Indice{ver2, ver2, ver2});
                    mesh.iBuffer[0].push_back(Indice{ver2 + 1, ver2 + 1, ver2 + 1});
				}
			}
		}
	}
	
	return mesh;
}

MeshConstructorArg Mesh::createCylindre(unsigned int prescision)
{
    GE_assert(prescision > 2);

	MeshConstructorArg mesh;
    mesh.objName = "Cylindre";
	mesh.iBuffer.push_back({});

	// Cylindre contain prescision * 2 + 2
	mesh.vBuffer. reserve(prescision * 2 + 2);
	mesh.vnBuffer. reserve(prescision * 2 + 2);
	mesh.vtBuffer. reserve(prescision * 2 + 2);

	float angleRad = M_PI * 2 / static_cast<float>(prescision);

	//near face

	//middle of near face is in front
	mesh.vBuffer.push_back({ 0.f, 0.f, 0.5f});
    mesh.vnBuffer.push_back({ 0.f, 0.f, 1.f});
    mesh.vtBuffer.push_back({ 0.f, 1.f});
    
	for(unsigned int i = 0; i < prescision; i++)
	{
        mesh.vBuffer.push_back({0.5f * cosf(i * angleRad), 0.5f * sinf(i * angleRad), 0.5f});
        mesh.vnBuffer.push_back({cosf(i * angleRad), sinf(i * angleRad), 1.f});
        mesh.vtBuffer.push_back({ 0.f, 1.f});

		mesh.vnBuffer.back().normalize();
	}
	
	//far face
	for(unsigned int i = 0; i < prescision; i++)
	{
        mesh.vBuffer.push_back({0.5f * cosf(i * angleRad), 0.5f * sinf(i * angleRad), -0.5f});
        mesh.vnBuffer.push_back({cosf(i * angleRad), sinf(i * angleRad), -1.f});
        mesh.vtBuffer.push_back({ 0.f, 1.f});

		mesh.vnBuffer.back().normalize();
	}

	//middle of far face is in back
    mesh.vBuffer.push_back({ 0.f, 0.f, -0.5f});
    mesh.vnBuffer.push_back({ 0.f, 0.f, -1.f});
    mesh.vtBuffer.push_back({ 0.f, 1.f});

	//calcul indice of mesh : 

	//near face triangle indice
	for(unsigned int i = 1; i < prescision; i++)
	{
		mesh.iBuffer[0].push_back(Indice{0, 0, 0});
		mesh.iBuffer[0].push_back(Indice{i, i, i});
		mesh.iBuffer[0].push_back(Indice{i + 1, i + 1, i + 1});
	}
		
	mesh.iBuffer[0].push_back(Indice{0, 0, 0});
	mesh.iBuffer[0].push_back(Indice{prescision, prescision, prescision});
	mesh.iBuffer[0].push_back(Indice{1, 1, 1});

	//side face triangle indice
	for(unsigned int i = 1; i < prescision ; i++)
	{
		//face is blit in 2 triangle : 
		mesh.iBuffer[0].push_back(Indice{i, i, i});
		mesh.iBuffer[0].push_back(Indice{i + 1, i + 1, i + 1});
		mesh.iBuffer[0].push_back(Indice{i + prescision, i + prescision, i + prescision});

		//triangle 2 : 
		mesh.iBuffer[0].push_back(Indice{i + prescision, i + prescision, i + prescision});
		mesh.iBuffer[0].push_back(Indice{i + prescision + 1, i + prescision + 1, i + prescision + 1});
		mesh.iBuffer[0].push_back(Indice{i + 1, i + 1, i + 1});
	}

	//face is blit in 2 triangle : 
	mesh.iBuffer[0].push_back(Indice{prescision, prescision, prescision});
	mesh.iBuffer[0].push_back(Indice{1, 1, 1});
	mesh.iBuffer[0].push_back(Indice{prescision + prescision, prescision + prescision, prescision + prescision});

	//triangle 2 : 
	mesh.iBuffer[0].push_back(Indice{prescision + prescision, prescision + prescision, prescision + prescision});
	mesh.iBuffer[0].push_back(Indice{prescision + 1, prescision + 1, prescision + 1});
	mesh.iBuffer[0].push_back(Indice{1, 1, 1});

	unsigned int middleFarPointIndice = prescision * 2 + 1;

	//far face triangle indice
	for(unsigned int i = prescision + 1; i < prescision + prescision; i++)
	{
		mesh.iBuffer[0].push_back(Indice{middleFarPointIndice, middleFarPointIndice, middleFarPointIndice});
		mesh.iBuffer[0].push_back(Indice{i, i, i});
		mesh.iBuffer[0].push_back(Indice{i + 1, i + 1, i + 1});
	}

	mesh.iBuffer[0].push_back(Indice{middleFarPointIndice, middleFarPointIndice, middleFarPointIndice});
	mesh.iBuffer[0].push_back(Indice{prescision + prescision, prescision + prescision, prescision + prescision});
	mesh.iBuffer[0].push_back(Indice{prescision + 1, prescision + 1, prescision + 1});

	return mesh;
}

void Mesh::generateNormalAndLoadIndice (const std::vector<Shape>& shapes) noexcept
{
	unsigned int count = 0;
	for (auto&& shape : shapes)
	{
		count ++;
		iBuffer_.push_back      ({});
		for (size_t i = 0; i < shape.iv.size(); i++)
		{
			iBuffer_.back().push_back ({shape.iv[i], shape.ivt[i], count});
		}
		
		if (!shape.material_ids.empty())
			idMaterial_.push_back	(shape.material_ids);	
	}

	for (auto &&i : iBuffer_)
	{
		for (size_t i2 = 0; i2 < i.size(); i2 += 3)
		{
			Vec3 v1 = vBuffer_[i[i2 + 0].iv];
			Vec3 v2 = vBuffer_[i[i2 + 1].iv];
			Vec3 v3 = vBuffer_[i[i2 + 2].iv];

			//comput normal with cross product
			vnBuffer_.push_back((v2 - v1).getCross(v3 - v1).normalize());
			vnBuffer_.push_back((v1 - v2).getCross(v3 - v2).normalize());
			vnBuffer_.push_back((v1 - v3).getCross(v2 - v3).normalize());
		}
	}
}