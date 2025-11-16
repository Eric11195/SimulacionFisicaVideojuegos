#pragma once

#include <string>
#include <vector>
#include "../../skeleton/GameObject.hpp"

class hud_elem : GameObject{
public:
	//Pos in percentage
	hud_elem(const std::string& file_name, physx::PxVec2 start_pos = { 0,0 }, physx::PxVec2 witdh_height = {1, 1});
	void render2D() override;
protected:
	unsigned int texture;
	unsigned int load_texture(const std::string& file_name);

	//Have shape coord(3), color(3), texture_coords(2)
	std::vector<physx::PxVec2> texCoords;
	void generate_quad(physx::PxVec2 start_point, physx::PxVec2 width_height, std::vector<physx::PxVec2>& v);
};