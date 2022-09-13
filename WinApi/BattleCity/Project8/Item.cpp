#include "Item.h"

Item::Item()
{

}

void Item::Init_Item()
{
	item_info[0].item_type = SHIELD_ITEM;
	item_info[0].on_off = FALSE;

	item_info[1].item_type = SPEED_ITEM;
	item_info[1].on_off = FALSE;
}

Item::~Item()
{

}