#include "SceneObject.hpp"
#include <cassert>


void SceneObject::set_color(Color c)
{
	render_item->color = c;
}

SceneObject::SceneObject(physx::PxScene* s, PxShape* shape, config& c)
	:GameObject(s,c.go_config)
{
	render_item = std::make_unique<RenderItem>(shape,&global_transform, c.color);
	//RegisterRenderItem(render_item.get());
}

SceneObject::SceneObject(physx::PxScene* s, config& c)
	: GameObject(s,c.go_config), render_item(nullptr)
{
}

SceneObject::~SceneObject()
{
	if((render_item)!=nullptr)
		render_item->release();
}

void SceneObject::render3D()
{
	GameObject::render3D();

	auto objTransform = render_item->transform;
	if (!objTransform)
	{
		auto actor = render_item->actor;
		if (actor)
		{
			render_shape(*render_item->shape, actor->getGlobalPose(), render_item->color);
			return;
		}
	}
	render_shape(*render_item->shape, objTransform ? *objTransform : physx::PxTransform(PxIdentity), render_item->color);
}

void SceneObject::render_shape(const PxShape& shape, const PxTransform& transform, const PxVec4& color)
{
	PxGeometryHolder h = shape.getGeometry();

	if (shape.getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render object
	glPushMatrix();
	PxMat44 mtx(transform);
	glMultMatrixf(reinterpret_cast<const float*>(&mtx));
	assert(glGetError() == GL_NO_ERROR);
	glColor4f(color.x, color.y, color.z, 1.0f);
	assert(glGetError() == GL_NO_ERROR);
	render_geometry(h, color.w < 0.999f);
	assert(glGetError() == GL_NO_ERROR);
	glPopMatrix();
	assert(glGetError() == GL_NO_ERROR);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	assert(glGetError() == GL_NO_ERROR);
}

static float gCylinderData[] = {
	1.0f,0.0f,1.0f,1.0f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
	0.866025f,0.500000f,1.0f,0.866025f,0.500000f,1.0f,0.866025f,0.500000f,0.0f,0.866025f,0.500000f,0.0f,
	0.500000f,0.866025f,1.0f,0.500000f,0.866025f,1.0f,0.500000f,0.866025f,0.0f,0.500000f,0.866025f,0.0f,
	-0.0f,1.0f,1.0f,-0.0f,1.0f,1.0f,-0.0f,1.0f,0.0f,-0.0f,1.0f,0.0f,
	-0.500000f,0.866025f,1.0f,-0.500000f,0.866025f,1.0f,-0.500000f,0.866025f,0.0f,-0.500000f,0.866025f,0.0f,
	-0.866025f,0.500000f,1.0f,-0.866025f,0.500000f,1.0f,-0.866025f,0.500000f,0.0f,-0.866025f,0.500000f,0.0f,
	-1.0f,-0.0f,1.0f,-1.0f,-0.0f,1.0f,-1.0f,-0.0f,0.0f,-1.0f,-0.0f,0.0f,
	-0.866025f,-0.500000f,1.0f,-0.866025f,-0.500000f,1.0f,-0.866025f,-0.500000f,0.0f,-0.866025f,-0.500000f,0.0f,
	-0.500000f,-0.866025f,1.0f,-0.500000f,-0.866025f,1.0f,-0.500000f,-0.866025f,0.0f,-0.500000f,-0.866025f,0.0f,
	0.0f,-1.0f,1.0f,0.0f,-1.0f,1.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,
	0.500000f,-0.866025f,1.0f,0.500000f,-0.866025f,1.0f,0.500000f,-0.866025f,0.0f,0.500000f,-0.866025f,0.0f,
	0.866026f,-0.500000f,1.0f,0.866026f,-0.500000f,1.0f,0.866026f,-0.500000f,0.0f,0.866026f,-0.500000f,0.0f,
	1.0f,0.0f,1.0f,1.0f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f
};
#define MAX_NUM_MESH_VEC3S  1024
static PxVec3 gVertexBuffer[MAX_NUM_MESH_VEC3S];

void SceneObject::render_geometry(const PxGeometryHolder& h, bool wireframe)
{
	auto geoType = h.getType();
	switch (geoType)
	{
	case PxGeometryType::eBOX:
	{
		glScalef(h.box().halfExtents.x, h.box().halfExtents.y, h.box().halfExtents.z);
		glutSolidCube(2.0);
	}
	break;
	case PxGeometryType::eSPHERE:
	{
		if (wireframe)
			glutWireSphere(GLdouble(h.sphere().radius), 10, 10);
		else
			glutSolidSphere(GLdouble(h.sphere().radius), 10, 10);
	}
	break;
	case PxGeometryType::eCAPSULE:
	{

		const PxF32 radius = h.capsule().radius;
		const PxF32 halfHeight = h.capsule().halfHeight;

		//Sphere
		glPushMatrix();
		glTranslatef(halfHeight, 0.0f, 0.0f);
		glScalef(radius, radius, radius);
		glutSolidSphere(1, 10, 10);
		glPopMatrix();

		//Sphere
		glPushMatrix();
		glTranslatef(-halfHeight, 0.0f, 0.0f);
		glScalef(radius, radius, radius);
		glutSolidSphere(1, 10, 10);
		glPopMatrix();

		//Cylinder
		glPushMatrix();
		glTranslatef(-halfHeight, 0.0f, 0.0f);
		glScalef(2.0f * halfHeight, radius, radius);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_FLOAT, 2 * 3 * sizeof(float), gCylinderData);
		glNormalPointer(GL_FLOAT, 2 * 3 * sizeof(float), gCylinderData + 3);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 13 * 2);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glPopMatrix();
	}
	break;
	case PxGeometryType::eCONVEXMESH:
	{

		//Compute triangles for each polygon.
		const PxVec3 scale = h.convexMesh().scale.scale;
		PxConvexMesh* mesh = h.convexMesh().convexMesh;
		const PxU32 nbPolys = mesh->getNbPolygons();
		const PxU8* polygons = mesh->getIndexBuffer();
		const PxVec3* verts = mesh->getVertices();
		PxU32 nbVerts = mesh->getNbVertices();
		PX_UNUSED(nbVerts);

		PxU32 numTotalTriangles = 0;
		for (PxU32 i = 0; i < nbPolys; i++)
		{
			PxHullPolygon data;
			mesh->getPolygonData(i, data);

			const PxU32 nbTris = PxU32(data.mNbVerts - 2);
			const PxU8 vref0 = polygons[data.mIndexBase + 0];
			PX_ASSERT(vref0 < nbVerts);
			for (PxU32 j = 0; j < nbTris; j++)
			{
				const PxU32 vref1 = polygons[data.mIndexBase + 0 + j + 1];
				const PxU32 vref2 = polygons[data.mIndexBase + 0 + j + 2];

				//generate face normal:
				PxVec3 e0 = verts[vref1] - verts[vref0];
				PxVec3 e1 = verts[vref2] - verts[vref0];

				PX_ASSERT(vref1 < nbVerts);
				PX_ASSERT(vref2 < nbVerts);

				PxVec3 fnormal = e0.cross(e1);
				fnormal.normalize();

				if (numTotalTriangles * 6 < MAX_NUM_MESH_VEC3S)
				{
					gVertexBuffer[numTotalTriangles * 6 + 0] = fnormal;
					gVertexBuffer[numTotalTriangles * 6 + 1] = verts[vref0];
					gVertexBuffer[numTotalTriangles * 6 + 2] = fnormal;
					gVertexBuffer[numTotalTriangles * 6 + 3] = verts[vref1];
					gVertexBuffer[numTotalTriangles * 6 + 4] = fnormal;
					gVertexBuffer[numTotalTriangles * 6 + 5] = verts[vref2];
					numTotalTriangles++;
				}
			}
		}
		glPushMatrix();
		glScalef(scale.x, scale.y, scale.z);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glNormalPointer(GL_FLOAT, 2 * 3 * sizeof(float), gVertexBuffer);
		glVertexPointer(3, GL_FLOAT, 2 * 3 * sizeof(float), gVertexBuffer + 1);
		glDrawArrays(GL_TRIANGLES, 0, int(numTotalTriangles * 3));
		glPopMatrix();
	}
	break;
	case PxGeometryType::eTRIANGLEMESH:
	{
		const PxTriangleMeshGeometry& triGeom = h.triangleMesh();
		const PxTriangleMesh& mesh = *triGeom.triangleMesh;
		const PxVec3 scale = triGeom.scale.scale;

		const PxU32 triangleCount = mesh.getNbTriangles();
		const PxU32 has16BitIndices = mesh.getTriangleMeshFlags() & PxTriangleMeshFlag::e16_BIT_INDICES;
		const void* indexBuffer = mesh.getTriangles();

		const PxVec3* vertexBuffer = mesh.getVertices();

		const PxU32* intIndices = reinterpret_cast<const PxU32*>(indexBuffer);
		const PxU16* shortIndices = reinterpret_cast<const PxU16*>(indexBuffer);
		PxU32 numTotalTriangles = 0;
		for (PxU32 i = 0; i < triangleCount; ++i)
		{
			PxVec3 triVert[3];

			if (has16BitIndices)
			{
				triVert[0] = vertexBuffer[*shortIndices++];
				triVert[1] = vertexBuffer[*shortIndices++];
				triVert[2] = vertexBuffer[*shortIndices++];
			}
			else
			{
				triVert[0] = vertexBuffer[*intIndices++];
				triVert[1] = vertexBuffer[*intIndices++];
				triVert[2] = vertexBuffer[*intIndices++];
			}

			PxVec3 fnormal = (triVert[1] - triVert[0]).cross(triVert[2] - triVert[0]);
			fnormal.normalize();

			if (numTotalTriangles * 6 < MAX_NUM_MESH_VEC3S)
			{
				gVertexBuffer[numTotalTriangles * 6 + 0] = fnormal;
				gVertexBuffer[numTotalTriangles * 6 + 1] = triVert[0];
				gVertexBuffer[numTotalTriangles * 6 + 2] = fnormal;
				gVertexBuffer[numTotalTriangles * 6 + 3] = triVert[1];
				gVertexBuffer[numTotalTriangles * 6 + 4] = fnormal;
				gVertexBuffer[numTotalTriangles * 6 + 5] = triVert[2];
				numTotalTriangles++;
			}
		}
		glPushMatrix();
		glScalef(scale.x, scale.y, scale.z);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glNormalPointer(GL_FLOAT, 2 * 3 * sizeof(float), gVertexBuffer);
		glVertexPointer(3, GL_FLOAT, 2 * 3 * sizeof(float), gVertexBuffer + 1);
		glDrawArrays(GL_TRIANGLES, 0, int(numTotalTriangles * 3));
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glPopMatrix();
	}
	break;
	case PxGeometryType::eINVALID:
	case PxGeometryType::eHEIGHTFIELD:
	case PxGeometryType::eGEOMETRY_COUNT:
	case PxGeometryType::ePLANE:
		break;
	}
}

SphereObject::SphereObject(physx::PxScene* s, config& c)
	: SceneObject(s,CreateShape(PxSphereGeometry(c.radius)), c.so_config) {}
/*
SphereObject::SphereObject(SceneObject::config c, PxShape* shape)
	:SceneObject(c, shape) {}
*/

CubeObject::CubeObject(physx::PxScene* s, config& c)
	:SceneObject(s, CreateShape(PxBoxGeometry(c.half_extents)), c.so_config) {}
