//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "game/gifts.hpp"

#include "game/sprites.hpp"

#include <cstring>

static int gifts_count = 0;
static PrototypeClass* gifts_list[MAX_GIFTS] = {nullptr};

int Gifts_Count() {
    return gifts_count;
}

PrototypeClass* Gifts_Get(int i) {
	return gifts_list[i];
}

void Gifts_Draw(int i, int x, int y) {
	PrototypeClass* prot = gifts_list[i];
	prot->Piirra(x - prot->leveys / 2, y - prot->korkeus / 2, 0);
}

void Gifts_Clean() {
	for (int i = 0; i < MAX_GIFTS; i++)
		gifts_list[i] = nullptr;
	gifts_count = 0;
}

bool Gifts_Add(PrototypeClass* protot) {
	int i = 0;
	bool success = false;

	while (i < MAX_GIFTS && !success) {
		if (gifts_list[i] == nullptr) {
			success = true;
			gifts_list[i] = protot;
			gifts_count++;
		}
		i++;
	}
	return success;
}

void Gifts_Remove(int i) {

	if(gifts_list[i] == nullptr)
		return;

	for (int i = 0; i < MAX_GIFTS - 1; i++)
		gifts_list[i] = gifts_list[i+1];
	
	gifts_list[MAX_GIFTS - 1] = nullptr;
	
	gifts_count--;

}

int Gifts_Use() {
	if (gifts_count > 0) {
		Sprites_add(
			gifts_list[0], 0,
			Player_Sprite->x - gifts_list[0]->leveys,
			Player_Sprite->y,
			nullptr, false);

		Gifts_Remove(0);
	}

	return 0;
}

int Gifts_ChangeOrder() {
	if (gifts_list[0] == nullptr)
		return 0;

	PrototypeClass* temp = gifts_list[0];

	for (int i = 0; i < MAX_GIFTS - 1; i++)
		gifts_list[i] = gifts_list[i+1];

	int count = 0;

	while(count < MAX_GIFTS-1 && gifts_list[count] != nullptr)
		count++;

	gifts_list[count] = temp;

	return 0;
}
