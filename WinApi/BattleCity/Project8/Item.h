#include <iostream>
#include <Windows.h>
#include <string.h>
#pragma once
using namespace std;

#define SHIELD_ITEM 88
#define SPEED_ITEM 89

struct _Item
{
	int item_type;
	bool on_off;
};

class Item
{
private:
public:
	Item();

	_Item item_info[2];
	void Init_Item();

	~Item();
};

