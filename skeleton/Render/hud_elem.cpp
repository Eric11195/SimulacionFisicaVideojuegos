#include "hud_elem.hpp"

#include "Render.h"

#define STB_IMAGE_IMPLEMENTATION  
#include "../../skeleton/stb_image.hpp"
#include <stdexcept>

#include "../../skeleton/ScreenSizeConstants.hpp"

hud_elem::hud_elem(const std::string& file_name)
	: texture(load_texture(file_name))	
{
	generate_quad(texCoords);
}

void hud_elem::render()
{
	if (!texture) return;

	//glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/*
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		texture_data);
	*/

	//glUseProgram(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glColor4f(1.0, 1.0, 1.0, 1.0f);

	//GENERATE RECT
	glBegin(GL_QUADS);
		//
		
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTexCoord2d(0.0, 0.0);  glVertex2f(0.0, 0.0);


		glTexCoord2d(1.0, 0.0); glVertex2f(WINDOW_WIDTH, 0.0);
		//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		

		glTexCoord2d(1.0, 1.0); glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		

		glTexCoord2d(0.0, 1.0); glVertex2f(0.0, WINDOW_HEIGHT);
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnd();
	//-------------

	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	//glPopMatrix();
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

void hud_elem::generate_quad(std::vector<float>& v)
{
	v = {
		0.0f, 0.0f,
		(float)WINDOW_WIDTH, 0.0f,
		(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT,
		0.0f, (float)WINDOW_HEIGHT
	};
}
