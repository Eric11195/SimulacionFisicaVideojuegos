#include "hud_elem.hpp"

#include "Render.h"

#define STB_IMAGE_IMPLEMENTATION  
#include "../../skeleton/stb_image.hpp"
#include <stdexcept>

#include "../../skeleton/ScreenSizeConstants.hpp"

#include "../mouse_pos.hpp"

hud_elem::hud_elem(const std::string& file_name, physx::PxVec2 start_pos, physx::PxVec2 width_height)
	: GameObject(), texture(load_texture(file_name))
{
	generate_quad(start_pos,width_height, texCoords);
}

void hud_elem::render2D()
{
	if (!texture) return;

	//glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0, 1.0, 1.0, 1.0f);

	//GENERATE RECT
	glBegin(GL_QUADS);
		//
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTexCoord2d(0.0, 0.0);  glVertex2f(WINDOW_WIDTH * texCoords[0].x, WINDOW_HEIGHT* texCoords[0].y);


		glTexCoord2d(1.0, 0.0); glVertex2f(WINDOW_WIDTH * texCoords[1].x, WINDOW_HEIGHT * texCoords[1].y);
		//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		

		glTexCoord2d(1.0, 1.0); glVertex2f(WINDOW_WIDTH * texCoords[2].x, WINDOW_HEIGHT * texCoords[2].y);
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		

		glTexCoord2d(0.0, 1.0); glVertex2f(WINDOW_WIDTH * texCoords[3].x, WINDOW_HEIGHT * texCoords[3].y);
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnd();
	//-------------

	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	//glPopMatrix();

	GameObject::render2D();
}

unsigned int hud_elem::load_texture(const std::string& filename)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width,height, nrChannels;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		else
			throw std::logic_error("Unsupported channel count");


		//glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			format,
			GL_UNSIGNED_BYTE,
			data);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			printf("glTexImage2D error: %d\n", err);
		}
		//glGenerateMipMap()

//		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		throw std::logic_error("load_texture(string&): ERROR: cannot load image: " + filename + ": " + stbi_failure_reason());
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	
	stbi_image_free(data);
	return texture;
}

void hud_elem::generate_quad(physx::PxVec2 start_point, physx::PxVec2 width_height, std::vector<physx::PxVec2>& v)
{
	v = {
		start_point,
		start_point + physx::PxVec2(width_height.x, 0),
		start_point + width_height,
		start_point + physx::PxVec2(0,width_height.y)
	};
}

text_hud_elem::text_hud_elem(std::string text, physx::PxVec2 start_pos)
	: my_text(text), my_start_pos(start_pos)
{
}

void text_hud_elem::render2D()
{
	draw_text();
	GameObject::render2D();
}

void text_hud_elem::change_text(const std::string&& s)
{
	my_text = s;
}

void text_hud_elem::draw_text()
{
	glMatrixMode(GL_PROJECTION);
	double* matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 512, 0, 512, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	//glLoadIdentity();
	glRasterPos2i(my_start_pos.x*WINDOW_WIDTH, my_start_pos.y*WINDOW_HEIGHT);

	int length = my_text.length();

	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)my_text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}
