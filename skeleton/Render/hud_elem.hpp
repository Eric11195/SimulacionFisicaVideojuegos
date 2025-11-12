#pragma once

#include <string>
#include <vector>

class hud_elem {
public:
	hud_elem(const std::string& file_name);
	void render();
protected:
	unsigned int texture;
	unsigned int load_texture(const std::string& file_name);

	//Have shape coord(3), color(3), texture_coords(2)
	std::vector<float> texCoords;
	void generate_quad(std::vector<float>& v);


};