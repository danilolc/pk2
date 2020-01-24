//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "game/gifts.hpp"

#include "game/sprites.hpp"

#include <cstring>

int gifts_count = 0;
int gifts_list[MAX_GIFTS];

int Gifts_Count() {
    return gifts_count;
}

int Gifts_Get(int i) {
	return gifts_list[i];
}

PrototypeClass* Gifts_GetProtot(int i) {
	return &Prototypes_List[ gifts_list[i] ];
}

void Gifts_Draw(int i, int x, int y) {
	PrototypeClass* prot = &Prototypes_List[ gifts_list[i] ];
	prot->Piirra(x - prot->leveys / 2, y - prot->korkeus / 2, 0);
}

void Gifts_Clean() {
	for (int i=0;i<MAX_GIFTS;i++)
		gifts_list[i] = -1;
	gifts_count = 0;
}

bool Gifts_Add(int prototype_id) {
	int i = 0;
	bool success = false;

	while (i < MAX_GIFTS && !success) {
		if (gifts_list[i] == -1) {
			success = true;
			gifts_list[i] = prototype_id;
			gifts_count++;
		}
		i++;
	}
	return success;
}

void Gifts_Remove(int i) {

	if(gifts_list[i] == -1)
		return;

	for (int i = 0; i < MAX_GIFTS - 1; i++)
		gifts_list[i] = gifts_list[i+1];
	
	gifts_list[MAX_GIFTS - 1] = -1;
	
	gifts_count--;

}

int Gifts_Use() {
	if (gifts_count > 0) {
		Sprites_add(
			gifts_list[0], 0,
			Player_Sprite->x - Prototypes_List[gifts_list[0]].leveys,
			Player_Sprite->y,
			MAX_SPRITEJA, false);

		Gifts_Remove(0);
	}

	return 0;
}

int Gifts_ChangeOrder() {
	if (gifts_list[0] == -1)
		return 0;

	int temp = gifts_list[0];

	for (int i = 0; i < MAX_GIFTS - 1; i++)
		gifts_list[i] = gifts_list[i+1];

	int count = 0;

	while(count < MAX_GIFTS-1 && gifts_list[count] != -1)
		count++;

	gifts_list[count] = temp;

	return 0;
}
