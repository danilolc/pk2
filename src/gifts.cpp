#include "gifts.hpp"

#include "sprites.hpp"
#include "game.hpp"
#include "language.hpp"
#include <cstring>

int gifts_count = 0;
int gifts_list[MAX_GIFTS];

int Gifts_Count() {
    return gifts_count;
}

int Gifts_Get(int i) {
	return gifts_list[i];
}

PK2Sprite_Prototyyppi* Gifts_GetProtot(int i) {
	return &Prototypes_List[ gifts_list[i] ];
}

void Gifts_Draw(int i, int x, int y) {
	PK2Sprite_Prototyyppi* prot = &Prototypes_List[ gifts_list[i] ];
	prot->Piirra(x - prot->leveys / 2, y - prot->korkeus / 2, 0);
}

void Gifts_Clean() {
	for (int i=0;i<MAX_GIFTS;i++)
		gifts_list[i] = -1;
	gifts_count = 0;
}

bool Gifts_Add(int prototype_id) {
	int i=0;
	bool lisatty = false;

	char ilmo[80];
	strcpy(ilmo,tekstit->Hae_Teksti(PK_txt.game_newitem));  //"a new item: ";

	while (i < MAX_GIFTS && !lisatty) {
		if (gifts_list[i] == -1) {
			lisatty = true;
			gifts_list[i] = prototype_id;
			
			strcat(ilmo, Prototypes_List[prototype_id].nimi); //TODO
			PK_Start_Info(ilmo);
			gifts_count++;
		}
		i++;
	}
	return lisatty;
}

int Gifts_Use() {
	if (gifts_count > 0) {
		Sprites_add(
			gifts_list[0], 0,
			Player_Sprite->x - Prototypes_List[gifts_list[0]].leveys,
			Player_Sprite->y,
			MAX_SPRITEJA, false);

		for (int i = 0; i < MAX_GIFTS - 1; i++)
			gifts_list[i] = gifts_list[i+1];

		gifts_list[MAX_GIFTS-1] = -1;

		gifts_count--;
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
