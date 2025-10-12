#include "Ship.hpp"
#include "ShipCannon.hpp"

Ship::Ship()
	:CompositeGameObject()
{
	addChild(new ShipCannon());
}
/*
void Ship::process_input(unsigned char c)
{
	switch (c) {
	case ' ':
		//SHOOT
		break;
	case'1':
		//x wing shot
		break;
	case '2':
		//misil q genera una estela de particulas
		break;
	case '3':
		//Rafagas
		break;
	}
}
*/

void Ship::step(double dt)
{
	CompositeGameObject::step(dt);
	GetCamera()->setTransform(global_transform);
}

void Ship::handle_keyboard_button_down(unsigned char c)
{
	if(c==' ')
		static_cast<ShipCannon*>(child_objects.front().get())->start_fire();

}

void Ship::handle_keyboard_button_up(unsigned char c)
{
	if(c==' ')
		static_cast<ShipCannon*>(child_objects.front().get())->stop_fire();
}
