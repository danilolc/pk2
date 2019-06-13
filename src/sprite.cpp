//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

#include "sprite.hpp"

#include "PisteDraw.hpp"
#include "platform.hpp"
#include "PisteLog.hpp"

using namespace std;


/* -------- PK2Sprite Prototype ------------------------------------------------------------------ */

PK2Sprite_Prototype::PK2Sprite_Prototype(){

	strcpy(versio,PK2SPRITE_VIIMEISIN_VERSIO);
	file = "";
	image_file = "";
	nimi = "";
	transformation_sprite = "";

	for (int i = 0; i < 5; i++) {
		bonus_sprite[i] = "";
	}
	
	ammus1_sprite = "";
	ammus2_sprite = "";

	for (int aani=0;aani<MAX_AANIA;aani++){
		strcpy(aanitiedostot[aani], "");
		aanet[aani] = -1;
	}

	lua_script = "";
	transformation_value = 0;

	aani_frq		= 22050;
	ammus1			= -1;
	ammus2			= -1;
	animaatioita	= 0;
	avain			= false;
	
	for (int i = 0; i < 5; i++) {
		bonus[i] = -1;

		bonus_amount[i] = 1;
	}

	energia			= 0;
	este			= false;
	este_ylos		= true;
	este_alas		= true;
	este_oikealle	= true;
	este_vasemmalle	= true;
	frameja			= 0;
	frame_rate		= 0;
	hyokkays1_aika  = 60;
	hyokkays2_aika  = 60;
	indeksi			= 0;
	kuva_x			= 0;
	kuva_y			= 0;
	kuva_frame_leveys  = 0;
	kuva_frame_korkeus = 0;
	korkeus			= 0;
	latausaika		= 0;
	leveys			= 0;
	max_hyppy		= 0;
	max_nopeus		= 3;
	muutos			= -1;
	paino			= 0;
	pallarx_kerroin = 0;
	pisteet			= 0;
	random_frq		= true;
	suojaus			= VAHINKO_EI;
	tarisee         = false;
	tiletarkistus	= true;
	tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	type			= type_EI_MIKAAN;
	vahinko			= 0;
	vahinko_type  = VAHINKO_ISKU;
	vari			= VARI_NORMAALI;
	vihollinen		= false;

	lapinakyvyys	= false;
	hehkuu			= false;
	tulitauko		= 0;
	liitokyky		= false;
	boss			= false;
	bonus_aina		= false;
	osaa_uida		= false;

	for (int i=0;i<SPRITE_MAX_AI;i++){
		AI[i] = AI_EI;
	}

	for (int i=0;i<SPRITE_MAX_FRAMEJA;i++)
		framet[i] = 0;

	for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++){
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			animaatiot[i].sekvenssi[j] = 0;

		animaatiot[i].looppi  = false;
		animaatiot[i].frameja = 0;
	}
}
PK2Sprite_Prototype::~PK2Sprite_Prototype(){
	for (int i=0;i<SPRITE_MAX_FRAMEJA;i++)
		if (framet[i] > 0)
			PisteDraw2_Image_Delete(this->framet[i]);
}

void PK2Sprite_Prototype::New(){
	strcpy(versio,PK2SPRITE_VIIMEISIN_VERSIO);
	file = "";
	image_file = "";
	nimi = "";
	transformation_sprite = "";

	for (int i = 0; i < 5; i++) {
		bonus_sprite[i] = "";
	}

	ammus1_sprite = "";
	ammus2_sprite = "";

	lua_script = "";
	transformation_value = 0;

	for (int aani=0;aani<MAX_AANIA;aani++){
		strcpy(aanitiedostot[aani], "");
		aanet[aani] = -1;
	}

	aani_frq		= 22050;
	ammus1			= -1;
	ammus2			= -1;
	animaatioita	= 0;
	avain			= false;

	for (int i = 0; i < 5; i++) {
		bonus[i] = -1;

		bonus_amount[i] = 1;
	}

	energia			= 0;
	este			= false;
	este_ylos		= true;
	este_alas		= true;
	este_oikealle	= true;
	este_vasemmalle	= true;
	frameja			= 0;
	frame_rate		= 0;
	hyokkays1_aika  = 60;
	hyokkays2_aika  = 60;
	indeksi			= 0;
	kuva_x			= 0;
	kuva_y			= 0;
	kuva_frame_leveys  = 0;
	kuva_frame_korkeus = 0;
	korkeus			= 0;
	latausaika		= 0;
	leveys			= 0;
	max_hyppy		= 0;
	max_nopeus		= 3;
	muutos			= -1;
	paino			= 0;
	pallarx_kerroin = 0;
	pisteet			= 0;
	random_frq		= true;
	suojaus			= VAHINKO_EI;
	tarisee         = false;
	tiletarkistus	= true;
	tuhoutuminen	= TUHOUTUMINEN_ANIMAATIO;
	type			= type_EI_MIKAAN;
	vahinko			= 0;
	vahinko_type	= VAHINKO_ISKU;
	vari			= VARI_NORMAALI;
	vihollinen		= false;

	lapinakyvyys	= false;
	hehkuu			= false;
	tulitauko		= 0;
	liitokyky		= false;
	boss			= false;
	bonus_aina		= false;
	osaa_uida		= false;

	for (int i = 0; i < SPRITE_MAX_FRAMEJA; i++) {
		this->framet_peilikuva[i] = 0;
	}

	int i=0;

	for (i=0;i<SPRITE_MAX_AI;i++){
		AI[i] = AI_EI;
	}

	for (i=0;i<SPRITE_MAX_FRAMEJA;i++){
		if (framet[i] != 0)
			PisteDraw2_Image_Delete(this->framet[i]);

		if (this->framet_peilikuva[i] != 0)
			PisteDraw2_Image_Delete(this->framet_peilikuva[i]);
	}

	for (i=0;i<SPRITE_MAX_FRAMEJA;i++){
		framet[i] = 0;
		framet_peilikuva[i] = 0;
	}

	for (i=0;i<SPRITE_MAX_ANIMAATIOITA;i++){
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			animaatiot[i].sekvenssi[j] = 0;

		animaatiot[i].looppi  = false;
		animaatiot[i].frameja = 0;
	}
}
int PK2Sprite_Prototype::Animaatio_Uusi(int anim_i, BYTE *sekvenssi, bool looppi){
	BYTE frame_i = 0;

	if (anim_i < SPRITE_MAX_ANIMAATIOITA)
	{
		animaatiot[anim_i].frameja = 0;

		while (frame_i < ANIMAATIO_MAX_SEKVENSSEJA && sekvenssi[frame_i] != 0)
		{
			animaatiot[anim_i].sekvenssi[frame_i] = sekvenssi[frame_i];
			animaatiot[anim_i].frameja++;
			frame_i++;
		}

		animaatiot[anim_i].looppi = looppi;
	}
	return 0;
}

void PK2Sprite_Prototype::SetProto10(PK2Sprite_Prototype10 &proto){
	image_file = proto.image_file;
	nimi = proto.nimi;
	transformation_sprite = proto.transformation_sprite;
	bonus_sprite[0] = proto.bonus_sprite;
	ammus1_sprite = proto.ammus1_sprite;
	ammus2_sprite = proto.ammus2_sprite;

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(aanitiedostot[aani], proto.aanitiedostot[aani]);
		aanet[aani] = proto.aanet[aani];
	}

	animaatioita		= proto.animaatioita;
	avain				= proto.avain;
	energia				= proto.energia;
	este				= proto.este;
	frameja				= proto.frameja;
	frame_rate			= proto.frame_rate;
	kuva_x				= proto.kuva_x;
	kuva_y				= proto.kuva_y;
	kuva_frame_leveys	= proto.kuva_frame_leveys;
	kuva_frame_korkeus	= proto.kuva_frame_korkeus;
	korkeus				= proto.korkeus;
	latausaika			= proto.latausaika;
	leveys				= proto.leveys;
	max_hyppy			= proto.max_hyppy;
	max_nopeus			= proto.max_nopeus;
	paino				= proto.paino;
	pisteet				= proto.pisteet;
	suojaus				= (BYTE) proto.suojaus;
	tuhoutuminen		= proto.tuhoutuminen;
	type				= proto.type;
	vahinko				= proto.vahinko;
	vari				= proto.vari;
	vihollinen			= proto.vihollinen;

	for (int i=0;i<5;i++)
	{
		AI[i] = proto.AI[i];
	}

	for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j = 0; j < ANIMAATIO_MAX_SEKVENSSEJA; j++) {
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];
		}

		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;
	}
}
void PK2Sprite_Prototype::SetProto11(PK2Sprite_Prototype11 &proto){
	image_file = proto.image_file;
	nimi = proto.nimi;
	transformation_sprite = proto.transformation_sprite;
	bonus_sprite[0] = proto.bonus_sprite;
	ammus1_sprite = proto.ammus1_sprite;
	ammus2_sprite = proto.ammus2_sprite;

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(aanitiedostot[aani], proto.aanitiedostot[aani]);
		aanet[aani] = proto.aanet[aani];
	}

	animaatioita		= proto.animaatioita;
	avain				= proto.avain;
	bonus_amount[0]     = proto.bonusten_lkm;
	energia				= proto.energia;
	este				= proto.este;
	frameja				= proto.frameja;
	frame_rate			= proto.frame_rate;
	hyokkays1_aika		= proto.hyokkays1_aika;
	hyokkays2_aika		= proto.hyokkays2_aika;
	kuva_x				= proto.kuva_x;
	kuva_y				= proto.kuva_y;
	kuva_frame_leveys	= proto.kuva_frame_leveys;
	kuva_frame_korkeus	= proto.kuva_frame_korkeus;
	korkeus				= proto.korkeus;
	latausaika			= proto.latausaika;
	leveys				= proto.leveys;
	max_hyppy			= proto.max_hyppy;
	max_nopeus			= proto.max_nopeus;
	paino				= proto.paino;
	pallarx_kerroin		= proto.pallarx_kerroin;
	pisteet				= proto.pisteet;
	suojaus				= proto.suojaus;
	tarisee				= proto.tarisee;
	tuhoutuminen		= proto.tuhoutuminen;
	type				= proto.type;
	vahinko				= proto.vahinko;
	vahinko_type		= proto.vahinko_type;
	vari				= proto.vari;
	vihollinen			= proto.vihollinen;

	for (int i=0;i<5;i++)
	{
		AI[i] = proto.AI[i];
	}

	for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];

		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;
	}
}
void PK2Sprite_Prototype::SetProto12(PK2Sprite_Prototype12 &proto){
	image_file = proto.image_file;
	nimi = proto.nimi;
	transformation_sprite = proto.transformation_sprite;
	bonus_sprite[0] = proto.bonus_sprite;
	ammus1_sprite = proto.ammus1_sprite;
	ammus2_sprite = proto.ammus2_sprite;

	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(aanitiedostot[aani], proto.aanitiedostot[aani]);
		aanet[aani] = proto.aanet[aani];
	}

	aani_frq			= proto.aani_frq;
	animaatioita		= proto.animaatioita;
	avain				= proto.avain;
	bonus_amount[0]		= proto.bonusten_lkm;
	energia				= proto.energia;
	este				= proto.este;
	este_ylos			= proto.este_ylos;
	este_alas			= proto.este_alas;
	este_oikealle		= proto.este_oikealle;
	este_vasemmalle		= proto.este_vasemmalle;
	frameja				= proto.frameja;
	frame_rate			= proto.frame_rate;
	hyokkays1_aika		= proto.hyokkays1_aika;
	hyokkays2_aika		= proto.hyokkays2_aika;
	kuva_x				= proto.kuva_x;
	kuva_y				= proto.kuva_y;
	kuva_frame_leveys	= proto.kuva_frame_leveys;
	kuva_frame_korkeus	= proto.kuva_frame_korkeus;
	korkeus				= proto.korkeus;
	latausaika			= proto.latausaika;
	leveys				= proto.leveys;
	max_hyppy			= proto.max_hyppy;
	max_nopeus			= proto.max_nopeus;
	paino				= proto.paino;
	pallarx_kerroin		= proto.pallarx_kerroin;
	pisteet				= proto.pisteet;
	random_frq			= proto.random_frq;
	suojaus				= proto.suojaus;
	tarisee				= proto.tarisee;
	tiletarkistus		= proto.tiletarkistus;
	tuhoutuminen		= proto.tuhoutuminen;
	type				= proto.type;
	vahinko				= proto.vahinko;
	vahinko_type		= proto.vahinko_type;
	vari				= proto.vari;
	vihollinen			= proto.vihollinen;

	for (int i=0;i<5;i++)
	{
		AI[i] = proto.AI[i];
	}

	for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];

		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;
	}
}

void PK2Sprite_Prototype::SetProto13(PK2Sprite_Prototype13 &proto){
	image_file = proto.image_file;
	nimi = proto.nimi;
	transformation_sprite = proto.transformation_sprite;
	
	bonus_sprite[0] = proto.bonus_sprite;

	ammus1_sprite = proto.ammus1_sprite;
	ammus2_sprite = proto.ammus2_sprite;
	
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(aanitiedostot[aani], proto.aanitiedostot[aani]);
		aanet[aani] = proto.aanet[aani];
	}

	aani_frq			= proto.aani_frq;
	animaatioita		= proto.animaatioita;
	avain				= proto.avain;
	bonus_amount[0]		= proto.bonusten_lkm;
	energia				= proto.energia;
	este				= proto.este;
	este_ylos			= proto.este_ylos;
	este_alas			= proto.este_alas;
	este_oikealle		= proto.este_oikealle;
	este_vasemmalle		= proto.este_vasemmalle;
	frameja				= proto.frameja;
	frame_rate			= proto.frame_rate;
	hyokkays1_aika		= proto.hyokkays1_aika;
	hyokkays2_aika		= proto.hyokkays2_aika;
	kuva_x				= proto.kuva_x;
	kuva_y				= proto.kuva_y;
	kuva_frame_leveys	= proto.kuva_frame_leveys;
	kuva_frame_korkeus	= proto.kuva_frame_korkeus;
	korkeus				= proto.korkeus;
	latausaika			= proto.latausaika;
	leveys				= proto.leveys;
	max_hyppy			= proto.max_hyppy;
	max_nopeus			= proto.max_nopeus;
	paino				= proto.paino;
	pallarx_kerroin		= proto.pallarx_kerroin;
	pisteet				= proto.pisteet;
	random_frq			= proto.random_frq;
	suojaus				= proto.suojaus;
	tarisee				= proto.tarisee;
	tiletarkistus		= proto.tiletarkistus;
	tuhoutuminen		= proto.tuhoutuminen;
	type				= proto.type;
	vahinko				= proto.vahinko;
	vahinko_type		= proto.vahinko_type;
	vari				= proto.vari;
	vihollinen			= proto.vihollinen;

	lapinakyvyys		= proto.lapinakyvyys;
	hehkuu				= proto.hehkuu;
	tulitauko			= proto.tulitauko;
	liitokyky			= proto.liitokyky;
	boss				= proto.boss;
	bonus_aina			= proto.bonus_aina;
	osaa_uida			= proto.osaa_uida;

	for (int i=0;i<10;i++)
	{
		AI[i] = proto.AI[i];
	}

	for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j = 0; j < ANIMAATIO_MAX_SEKVENSSEJA; j++) {
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];
			animations[i].frames[j] = proto.animaatiot[i].sekvenssi[j];

			if (j + 1 < ANIMAATIO_MAX_SEKVENSSEJA) {
				if (proto.animaatiot[i].sekvenssi[j + 1] == 0 && proto.animaatiot[i].sekvenssi[j] != 0) {
					animations[i].last_frame = proto.animaatiot[i].sekvenssi[j] - 1;
				}
			}
		}

		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;

		animations[i].loop = proto.animaatiot[i].looppi;
		animations[i].frame_amount = proto.animaatiot[i].frameja;
	}
}

PK2Sprite_Prototype13 PK2Sprite_Prototype::GetProto13(){
	PK2Sprite_Prototype13 proto;

	strcpy(proto.image_file, image_file.c_str());
	strcpy(proto.nimi, nimi.c_str());
	strcpy(proto.transformation_sprite, transformation_sprite.c_str());
	
	strcpy(proto.bonus_sprite, bonus_sprite[0].c_str());

	strcpy(proto.ammus1_sprite, ammus1_sprite.c_str());
	strcpy(proto.ammus2_sprite, ammus2_sprite.c_str());
	
	for (int aani=0;aani<MAX_AANIA;aani++)
	{
		strcpy(proto.aanitiedostot[aani], aanitiedostot[aani]);
		proto.aanet[aani] = aanet[aani];
	}

	proto.aani_frq			= aani_frq;
	proto.animaatioita		= animaatioita;
	proto.avain				= avain;
	proto.bonusten_lkm      = bonus_amount[0];
	proto.energia			= energia;
	proto.este				= este;
	proto.este_ylos			= este_ylos;
	proto.este_alas			= este_alas;
	proto.este_oikealle		= este_oikealle;
	proto.este_vasemmalle	= este_vasemmalle;
	proto.frameja			= frameja;
	proto.frame_rate		= frame_rate;
	proto.hyokkays1_aika	= hyokkays1_aika;
	proto.hyokkays2_aika	= hyokkays2_aika;
	proto.kuva_x			= kuva_x;
	proto.kuva_y			= kuva_y;
	proto.kuva_frame_leveys = kuva_frame_leveys;
	proto.kuva_frame_korkeus= kuva_frame_korkeus;
	proto.korkeus			= korkeus;
	proto.latausaika		= latausaika;
	proto.leveys			= leveys;
	proto.max_hyppy			= max_hyppy;
	proto.max_nopeus		= max_nopeus;
	proto.paino				= paino;
	proto.pallarx_kerroin   = pallarx_kerroin;
	proto.pisteet			= pisteet;
	proto.random_frq		= random_frq;
	proto.suojaus			= suojaus;
	proto.tarisee			= tarisee;
	proto.tiletarkistus		= tiletarkistus;
	proto.tuhoutuminen		= tuhoutuminen;
	proto.type			= type;
	proto.vahinko			= vahinko;
	proto.vahinko_type	= vahinko_type;
	proto.vari				= vari;
	proto.vihollinen		= vihollinen;

	proto.lapinakyvyys		= lapinakyvyys;
	proto.hehkuu			= hehkuu;
	proto.tulitauko			= tulitauko;
	proto.liitokyky			= liitokyky;
	proto.boss				= boss;
	proto.bonus_aina		= bonus_aina;
	proto.osaa_uida			= osaa_uida;

	for (int i=0;i<10;i++)
	{
		proto.AI[i] = AI[i];
	}

	for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++)
	{
		for (int j=0;j<ANIMAATIO_MAX_SEKVENSSEJA;j++)
			proto.animaatiot[i].sekvenssi[j] = animaatiot[i].sekvenssi[j];

		proto.animaatiot[i].looppi  = animaatiot[i].looppi;
		proto.animaatiot[i].frameja = animaatiot[i].frameja;
	}

	return proto;
}

void PK2Sprite_Prototype::SetProto14(PK2Sprite_Prototype14 &proto) {
	image_file = proto.image_file;
	nimi = proto.name;
	transformation_sprite = proto.transformation_sprite;

	for (int i = 0; i < 5; i++) {
		bonus_sprite[i] = proto.bonus_sprite[i];
	}

	ammus1_sprite = proto.ammo1_sprite;
	ammus2_sprite = proto.ammo2_sprite;

	for (int aani = 0; aani<MAX_AANIA; aani++) {
		strcpy(aanitiedostot[aani], proto.sound_files[aani]);
		//aanet[aani] = proto.aanet[aani];
	}

	aani_frq = proto.sound_frq;
	animaatioita = proto.animations;
	avain = proto.is_key;

	for (int i = 0; i < 5; i++) {
		bonus_amount[i] = proto.bonus_amount[i];
	}

	energia = proto.energy;
	este = proto.obstacle;
	este_ylos = proto.wall_up;
	este_alas = proto.wall_down;
	este_oikealle = proto.wall_right;
	este_vasemmalle = proto.wall_left;
	frameja = proto.frames;
	frame_rate = proto.frame_rate;
	hyokkays1_aika = proto.attack1_frames;
	hyokkays2_aika = proto.attack2_frames;
	kuva_x = proto.clip_x;
	kuva_y = proto.clip_y;
	kuva_frame_leveys = proto.clip_width;
	kuva_frame_korkeus = proto.clip_height;
	korkeus = proto.hitbox_height;
	latausaika = proto.loading_time;
	leveys = proto.hitbox_width;
	max_hyppy = proto.max_jump_time;
	max_nopeus = proto.max_speed;
	paino = proto.weight;
	pallarx_kerroin = proto.parallax_factor;
	pisteet = proto.score;
	random_frq = proto.random_frq;
	suojaus = proto.immunity;
	tarisee = proto.shakes;
	tiletarkistus = proto.tile_check;
	tuhoutuminen = proto.destruction;
	type = proto.type;
	vahinko = proto.damage;
	vahinko_type = proto.damage_type;
	vari = proto.color;
	vihollinen = proto.is_enemy;

	tulitauko = proto.attack_pause;
	liitokyky = proto.can_glide;
	boss = proto.is_boss;
	bonus_aina = proto.bonus_always;
	osaa_uida = proto.can_swim;

	transformation_value = proto.transformation_value;
	hitbox_x = proto.hitbox_x;
	hitbox_y = proto.hitbox_y;

	for (int i = 0; i<10; i++) {
		AI[i] = proto.AI[i];
	}

	for (int i = 0; i<SPRITE_MAX_ANIMAATIOITA; i++) {
		for (int j = 0; j<ANIMAATIO_MAX_SEKVENSSEJA; j++)
			animaatiot[i].sekvenssi[j] = proto.animation_sequence[i].sekvenssi[j];

		animaatiot[i].looppi = proto.animation_sequence[i].looppi;
		animaatiot[i].frameja = proto.animation_sequence[i].frameja;
	}

	message = proto.message;
}

int PK2Sprite_Prototype::Lataa(char *path, char *file_name, char* episode){
	this->New();

	if (strcmp(path,"") != 0)
		strcat(path, file_name);
	else
		strcpy(path, file_name);

	ifstream *tiedosto = new ifstream(path, ios::binary);
	char versio[4];

	if (tiedosto->fail()){
		//printf("PK2SPR - failed to open %s.\n", path);
		delete (tiedosto);

		return 1;
	}

	tiedosto->read ((char *)versio, 4);

	if (strcmp(versio,"1.0") == 0){
		this->New();
		PK2Sprite_Prototype10 proto;
		tiedosto->read ((char *)&proto, sizeof (proto));
		this->SetProto10(proto);
		strcpy(this->versio,versio);
		this->file = file_name;
	}
	if (strcmp(versio,"1.1") == 0){
		this->New();
		PK2Sprite_Prototype11 proto;
		tiedosto->read ((char *)&proto, sizeof (proto));
		this->SetProto11(proto);
		strcpy(this->versio,versio);
		this->file = file_name;
	}
	if (strcmp(versio,"1.2") == 0){
		this->New();
		PK2Sprite_Prototype12 proto;
		tiedosto->read ((char *)&proto, sizeof (proto));
		this->SetProto12(proto);
		strcpy(this->versio,versio);
		this->file = file_name;
	}
	if (strcmp(versio,"1.3") == 0){
		this->New();

		PK2Sprite_Prototype13 proto;

		tiedosto->read ((char *) &proto, sizeof (proto));
		this->SetProto13(proto);

		strcpy(this->versio,versio);
		this->file = file_name;
	}

	if (strcmp(versio, "1.4") == 0) {
		this->New();

		PK2Sprite_Prototype14 proto;

		tiedosto->read((char *) &proto, sizeof(proto));
		SetProto14(proto);

		strcpy(this->versio, versio);
		this->file = file_name;
	}

	if (tiedosto->fail()){
		delete (tiedosto);
		return 1;
	}

	delete (tiedosto);

	// Get sprite bmp
	
	// Check the episode/-custom episode-/sprites directory first...
	std::stringstream ss;
	ss << "episodes/" << episode << "/sprites/" << image_file;
	
	int bufferi = PisteDraw2_Image_Load(ss.str().c_str(), false);

	if (bufferi == -1) {
		//... if that fails check the games sprites/ folder
		ss.str("");
		ss << "sprites/" << image_file;

		bufferi = PisteDraw2_Image_Load(ss.str().c_str(), false);

		if (bufferi == -1) {
			return 1;
		}
	}

	//Set diferent colors
	BYTE *buffer = NULL;
	DWORD leveys;
	BYTE vari;
	int x,y,w,h;

	if (this->vari != VARI_NORMAALI){ //Change sprite colors
		PisteDraw2_Image_GetSize(bufferi,w,h);

		PisteDraw2_DrawImage_Start(bufferi, *&buffer, leveys);

		for (x= 0; x < w; x++)
			for (y = 0; y < h; y++)
				if ((vari = buffer[x + y * leveys]) != 255){
					vari %= 32;
					vari += this->vari;
					buffer[x + y * leveys] = vari;
				}

		PisteDraw2_DrawImage_End(bufferi);
	}

	int frame_i = 0,
		frame_x = kuva_x,
		frame_y = kuva_y;

	//Get each frame
	for (frame_i = 0; frame_i < frameja; frame_i++){
		if (frame_x + kuva_frame_leveys > 640){
			frame_y += this->kuva_frame_korkeus + 3;
			frame_x = kuva_x;
		}

		framet[frame_i] = PisteDraw2_Image_Cut(bufferi,frame_x,frame_y,kuva_frame_leveys,kuva_frame_korkeus); //frames
		framet_peilikuva[frame_i] = PisteDraw2_Image_Cut(bufferi,frame_x,frame_y,kuva_frame_leveys,kuva_frame_korkeus); //flipped frames
		PisteDraw2_Image_FlipHori(framet_peilikuva[frame_i]);

		frame_x += this->kuva_frame_leveys + 3;
	}

	PisteDraw2_Image_Delete(bufferi);
	return 0;
}

int PK2Sprite_Prototype::Load(std::string filename, std::string episode) {
	this->New();

	std::stringstream ss;
	ss << "episodes/" << episode << "/sprites/" << filename;

	ifstream filestream(ss.str(), ios::binary);

	if (filestream.fail()) {
		ss.str("");
		ss << "sprites/" << filename;

		filestream.open(ss.str(), ios::binary);

		if (filestream.fail()) {
			PisteLog_Write("PK2Sprite", "Can't find file: " + filename, TYPE::T_ERROR);

			return 1;
		}
	}

	char versio[4];
	filestream.read((char *)versio, 4);

	if (strcmp(versio, "1.0") == 0) {
		this->New();
		PK2Sprite_Prototype10 proto;
		filestream.read((char *)&proto, sizeof(proto));
		this->SetProto10(proto);
		strcpy(this->versio, versio);

		this->file = filename;
	}

	if (strcmp(versio, "1.1") == 0) {
		this->New();
		PK2Sprite_Prototype11 proto;
		filestream.read((char *)&proto, sizeof(proto));
		this->SetProto11(proto);
		strcpy(this->versio, versio);

		this->file = filename;
	}

	if (strcmp(versio, "1.2") == 0) {
		this->New();
		PK2Sprite_Prototype12 proto;
		filestream.read((char *)&proto, sizeof(proto));
		this->SetProto12(proto);
		strcpy(this->versio, versio);

		this->file = filename;
	}

	if (strcmp(versio, "1.3") == 0) {
		this->New();

		PK2Sprite_Prototype13 proto;

		filestream.read((char *)&proto, sizeof(proto));
		this->SetProto13(proto);

		strcpy(this->versio, versio);

		this->file = filename;
	}

	if (strcmp(versio, "1.4") == 0) {
		this->New();

		PK2Sprite_Prototype14 proto;
		
		this->SetProto14(proto);

		/*
		filestream.read(reinterpret_cast<char *>(&proto.type), sizeof(BYTE));
		
		proto.image_file = read_string(filestream);
		proto.lua_script = read_string(filestream);
		proto.message = read_string(filestream);

		for (int i = 0; i < 7; i++) {
			filestream.read(proto.sound_files[i], 100);
		}

		filestream.read(reinterpret_cast<char *>(&proto.frames), sizeof(BYTE));

		filestream.read(reinterpret_cast<char *>(&proto.animation_sequence), sizeof(proto.animation_sequence));

		filestream.read(reinterpret_cast<char *>(&proto.animations), sizeof(BYTE));
		filestream.read(reinterpret_cast<char *>(&proto.frame_rate), sizeof(BYTE));

		filestream.read(reinterpret_cast<char *>(&proto.clip_x), sizeof(int));
		filestream.read(reinterpret_cast<char *>(&proto.clip_y), sizeof(int));
		filestream.read(reinterpret_cast<char *>(&proto.clip_width), sizeof(int));
		filestream.read(reinterpret_cast<char *>(&proto.clip_height), sizeof(int));

		proto.name = read_string(filestream);

		filestream.read(reinterpret_cast<char *>(&proto.hitbox_x), sizeof(int));
		filestream.read(reinterpret_cast<char *>(&proto.hitbox_y), sizeof(int));
		filestream.read(reinterpret_cast<char *>(&proto.hitbox_width), sizeof(int));
		filestream.read(reinterpret_cast<char *>(&proto.hitbox_height), sizeof(int));

		filestream.read(reinterpret_cast<char *>(&proto.weight), sizeof(double));
		
		filestream.read(reinterpret_cast<char *>(&proto.is_enemy), sizeof(bool));
		
		filestream.read(reinterpret_cast<char *>(&proto.energy), sizeof(int));
		filestream.read(reinterpret_cast<char *>(&proto.damage), sizeof(int));

		filestream.read(reinterpret_cast<char *>(&proto.damage_type), sizeof(BYTE));
		filestream.read(reinterpret_cast<char *>(&proto.immunity), sizeof(BYTE));

		filestream.read(reinterpret_cast<char *>(&proto.score), sizeof(int));
		
		filestream.read(reinterpret_cast<char *>(&proto.AI), sizeof(proto.AI));

		filestream.read(reinterpret_cast<char *>(&proto.max_jump_time), sizeof(BYTE));

		filestream.read(reinterpret_cast<char *>(&proto.max_speed), sizeof(double));

		filestream.read(reinterpret_cast<char *>(&proto.loading_time), sizeof(int));
		filestream.read(reinterpret_cast<char *>(&proto.color), sizeof(BYTE));
		filestream.read(reinterpret_cast<char *>(&proto.obstacle), sizeof(bool));
		filestream.read(reinterpret_cast<char *>(&proto.destruction), sizeof(int));
		filestream.read(reinterpret_cast<char *>(&proto.is_key), sizeof(bool));
		filestream.read(reinterpret_cast<char *>(&proto.shakes), sizeof(bool));

		int bonus_val = 0;
		filestream.read(reinterpret_cast<char *>(&bonus_val), sizeof(int));

		for (int i = 0; i < bonus_val; i++) {
			filestream.read(reinterpret_cast<char *>(&proto.bonus_amount), sizeof(proto.bonus_amount));
		}

		filestream.read(reinterpret_cast<char *>(&proto.attack1_frames), sizeof(int));
		filestream.read(reinterpret_cast<char *>(&proto.attack2_frames), sizeof(int));
		filestream.read(reinterpret_cast<char *>(&proto.parallax_factor), sizeof(int));

		proto.transformation_sprite = read_string(filestream);

		int bonus_spr_amount = 0;

		filestream.read(reinterpret_cast<char *>(&bonus_spr_amount), sizeof(int));

		for (int i = 0; i < bonus_spr_amount; i++) {
			bonus_sprite[i] = read_string(filestream);
		}

		proto.ammo1_sprite = read_string(filestream);
		proto.ammo2_sprite = read_string(filestream);

		proto.message = read_string(filestream);
		filestream.read(reinterpret_cast<char *>(&proto.message_when_active), sizeof(bool));
		
		filestream.read(reinterpret_cast<char *>(&proto.show_healthbar), sizeof(bool));
		filestream.read(reinterpret_cast<char *>(&proto.healthbar_when_active), sizeof(bool));
		
		filestream.read(reinterpret_cast<char *>(&proto.tile_check), sizeof(bool));

		filestream.read(reinterpret_cast<char *>(&proto.sound_frq), sizeof(int));

		filestream.read(reinterpret_cast<char *>(&proto.random_frq), sizeof(bool));

		filestream.read(reinterpret_cast<char *>(&proto.wall_up), sizeof(bool));
		filestream.read(reinterpret_cast<char *>(&proto.wall_down), sizeof(bool));
		filestream.read(reinterpret_cast<char *>(&proto.wall_right), sizeof(bool));
		filestream.read(reinterpret_cast<char *>(&proto.wall_left), sizeof(bool));
		
		filestream.read(reinterpret_cast<char *>(&proto.attack_pause), sizeof(int));

		filestream.read(reinterpret_cast<char *>(&proto.can_glide), sizeof(bool));
		filestream.read(reinterpret_cast<char *>(&proto.is_boss), sizeof(bool));
		filestream.read(reinterpret_cast<char *>(&proto.bonus_always), sizeof(bool));
		filestream.read(reinterpret_cast<char *>(&proto.can_swim), sizeof(bool));

		filestream.read(reinterpret_cast<char *>(&proto.transformation_value), sizeof(int));

		filestream.read(reinterpret_cast<char *>(&proto.trigger_healthbar_when_active, sizeof(bool)));
		filestream.read(reinterpret_cast<char *>(&proto.trigger_message_on_collision, sizeof(bool)));*/

		this->file = filename;
	}

	filestream.close();

	// Get sprite bmp
	// Check the episode/-custom episode-/sprites directory first...
	ss.str("");
	ss << "episodes/" << episode << "/sprites/" << image_file;

	int bufferi = PisteDraw2_Image_Load(ss.str().c_str(), false);

	if (bufferi == -1) {
		//... if that fails check the games sprites/ folder
		ss.str("");
		ss << "sprites/" << image_file;

		bufferi = PisteDraw2_Image_Load(ss.str().c_str(), false);

		if (bufferi == -1) {
			return 1;
		}
	}

	//Set diferent colors
	BYTE *buffer = nullptr;
	DWORD leveys;
	BYTE vari;
	int x, y, w, h;

	if (this->vari != VARI_NORMAALI) { //Change sprite colors
		PisteDraw2_Image_GetSize(bufferi, w, h);

		PisteDraw2_DrawImage_Start(bufferi, *&buffer, leveys);

		for (x = 0; x < w; x++)
			for (y = 0; y < h; y++)
				if ((vari = buffer[x + y * leveys]) != 255) {
					vari %= 32;
					vari += this->vari;
					buffer[x + y * leveys] = vari;
				}

		PisteDraw2_DrawImage_End(bufferi);
	}

	int frame_i = 0,
		frame_x = kuva_x,
		frame_y = kuva_y;

	//Get each frame
	for (frame_i = 0; frame_i < frameja; frame_i++) {
		if (frame_x + kuva_frame_leveys > 640) {
			frame_y += this->kuva_frame_korkeus + 3;
			frame_x = kuva_x;
		}

		framet[frame_i] = PisteDraw2_Image_Cut(bufferi, frame_x, frame_y, kuva_frame_leveys, kuva_frame_korkeus); //frames
		framet_peilikuva[frame_i] = PisteDraw2_Image_Cut(bufferi, frame_x, frame_y, kuva_frame_leveys, kuva_frame_korkeus); //flipped frames
		PisteDraw2_Image_FlipHori(framet_peilikuva[frame_i]);

		frame_x += this->kuva_frame_leveys + 3;
	}

	PisteDraw2_Image_Delete(bufferi);
	return 0;
}

void PK2Sprite_Prototype::Tallenna(char *tiedoston_nimi){
   	/*strcpy(this->tiedosto,tiedoston_nimi);

	PK2Sprite_Prototype13 proto = GetProto13();

	ofstream *tiedosto = new ofstream(tiedoston_nimi, ios::binary);
	tiedosto->write (PK2SPRITE_VIIMEISIN_VERSIO, 4);
	tiedosto->write ((char *)&proto, sizeof (proto));

	delete (tiedosto);*/
}

int PK2Sprite_Prototype::Piirra(int x, int y, int frame){
	PisteDraw2_Image_Clip(framet[frame], x, y);
	return 0;
}
bool PK2Sprite_Prototype::Onko_AI(int ai){
	for (int i=0;i<SPRITE_MAX_AI;i++)
		if (AI[i] == ai)
			return true;
	return false;
}

/* -------- PK2Sprite  ------------------------------------------------------------------ */

PK2Sprite::PK2Sprite(){
	this->type		= NULL;
	this->pelaaja		= 0;
	this->piilota		= true;
	this->x				= 0;
	this->y				= 0;
	this->alku_x		= 0;
	this->alku_y		= 0;
	this->a				= 0;
	this->b				= 0;
	this->hyppy_ajastin	= 0;
	this->kyykky		= false;
	this->energia		= 0;
	this->alkupaino		= 0;
	this->paino			= 0;
	this->kytkinpaino   = 0;
	this->flip_x		= false;
	this->flip_y		= false;
	this->current_animation = Animation::STILL;
	this->alas			= true;
	this->ylos			= true;
	this->oikealle		= true;
	this->vasemmalle	= true;
	this->reuna_oikealla	= false;
	this->reuna_vasemmalla	= false;
	this->frame_aika	= 0;
	this->current_frame = 0;
	this->isku			= 0;
	this->lataus		= 0;
	this->hyokkays1		= 0;
	this->hyokkays2		= 0;
	this->vedessa		= false;
	this->piilossa      = false;
	this->saatu_vahinko = 0;
	this->vihollinen	= false;
	this->ammus1		= -1;
	this->ammus2		= -1;
	this->pelaaja_x		= -1;
	this->pelaaja_y		= -1;
	this->ajastin		= 0;
	this->transformation_ajastin = 0;
}
PK2Sprite::PK2Sprite(PK2Sprite_Prototype *type, int pelaaja, bool piilota, double x, double y){
	if (type){
		this->type		= type;
		this->pelaaja		= pelaaja;
		this->piilota		= piilota;
		this->x				= x;
		this->y				= y;
		this->alku_x		= x;
		this->alku_y		= y;
		this->a				= 0;
		this->b				= 0;
		this->hyppy_ajastin	= 0;
		this->energia		= type->energia;
		this->alkupaino     = type->paino;
		this->paino			= this->alkupaino;
		this->kytkinpaino   = 0;
		this->kyykky		= false;
		this->flip_x		= false;
		this->flip_y		= false;
		this->current_animation = Animation::STILL;
		this->alas			= true;
		this->ylos			= true;
		this->oikealle		= true;
		this->vasemmalle	= true;
		this->reuna_oikealla	= false;
		this->reuna_vasemmalla	= false;
		this->frame_aika	= 0;
		this->current_frame = 0;
		this->isku			= 0;
		this->lataus		= 0;
		this->hyokkays1		= 0;
		this->hyokkays2		= 0;
		this->vedessa		= false;
		this->piilossa      = false;
		this->saatu_vahinko = 0;
		this->vihollinen    = type->vihollinen;
		this->ammus1		= type->ammus1;
		this->ammus2		= type->ammus2;
		this->pelaaja_x		= -1;
		this->pelaaja_y		= -1;
		this->ajastin		= 0;
		this->transformation_ajastin = 0;
	}
}
PK2Sprite::~PK2Sprite() {}

bool PK2Sprite::Onko_AI(int ai){
	for (int i=0;i<SPRITE_MAX_AI;i++)
		if (type->AI[i] == ai)
			return true;
	return false;
}

void PK2Sprite::Animate(int anim_i, bool loop){
	if (anim_i != current_animation) {
		if (loop) {
			current_frame = 0;
		}

		current_animation = anim_i;
	}
}
int PK2Sprite::Animoi(){
	int frame = 0;

	if (type->type == TYPE_CHECKPOINT) {
		Animate_Checkpoint();
	} else {
		switch (type->AI[0]) {
			case AI_KANA:		Animaatio_Kana(); break;
			case AI_PIKKUKANA:	Animaatio_Kana(); break;
			case AI_BONUS:		Animaatio_Bonus(); break;
			case AI_MUNA:		Animaatio_Muna(); break;
			case AI_AMMUS:		Animaatio_Ammus(); break;
			case AI_HYPPIJA:	Animaatio_Kana(); break;
			case AI_PERUS:		Animaatio_Perus(); break;
			case AI_TELEPORTTI:	Animaatio_Perus(); break;
			default:			break;
		}
	}

	PK2SPRITE_ANIMAATIO &animaatio = type->animaatiot[current_animation];

	if (current_frame >= animaatio.frameja)
		current_frame = 0;

	frame = animaatio.sekvenssi[current_frame]-1;

	// Lasketaan kuinka paljon talla hetkella voimassa olevaa framea viela naytetaan
	if (frame_aika < type->frame_rate)
		frame_aika++;
	// Jos aika on kulunut loppuun, vaihdetaan seuraava frame tamanhetkisesta animaatiosta
	else{
		frame_aika = 0;

		// Are there any more frames in the animation?
		if (current_frame < animaatio.frameja - 1) {
			current_frame++;
		} else {
			if (animaatio.looppi) {
				current_frame = 0;
			}
		}
	}

	if (frame > type->frameja)
		frame = type->frameja;

	return frame;
}
int PK2Sprite::Piirra(int kamera_x, int kamera_y){
	// Tehdaan apumuuttujia
	int	l = (int)type->kuva_frame_leveys/2,//leveys
		h = (int)type->kuva_frame_korkeus/2,
		x = (int)this->x-(kamera_x),
		y = (int)this->y-(kamera_y),
		frame = 0;

	frame = Animoi();

	if (type->tarisee){
		x+= rand()%2 - rand()%2;
		y+= rand()%2 - rand()%2;
	}

	if (flip_x){
		if (!flip_y)
			PisteDraw2_Image_Clip(type->framet_peilikuva[frame], x-l-1, y-h);
		else
			PisteDraw2_Image_Clip(type->framet_peilikuva[frame], x-l-1, y-h);//, false, true);//TODO-Fix
	} else{
		if (!flip_y)
			PisteDraw2_Image_Clip(type->framet[frame], x-l-1, y-h);
		else
			PisteDraw2_Image_Clip(type->framet[frame], x-l-1, y-h);//, false, true);
	}

	return 0;
}

int PK2Sprite::AI_Perus(){
	if (x < 10)
	{
		x = 10;
		vasemmalle = false;
	}

	if (x > 8192)
	{
		x = 8192;
		oikealle = false;
	}

	if (y > 9920)
	{
		y = 9920;
	}

	if (y < -32)
	{
		y = -32;
	}

	if (a < 0)
		flip_x = true;

	if (a > 0)
		flip_x = false;

	ajastin++;

	if (ajastin > 31320) // jaollinen 360:lla
		ajastin = 0;

	return 0;
}
int PK2Sprite::AI_Kaantyy_Esteesta_Hori(){
	if (energia > 0)
	{
		if (!oikealle)
		{
			a = this->type->max_nopeus / -3.5;
			return 1;
		}

		if (!vasemmalle)
		{
			a = this->type->max_nopeus / 3.5;
			return 1;
		}
	}
	return 0;
}
int PK2Sprite::AI_Kaantyy_Esteesta_Vert(){
	if (energia > 0)
	{
		if (!alas)
		{
			b = this->type->max_nopeus / -3.5;
			return 1;
		}

		if (!ylos)
		{
			b = this->type->max_nopeus / 3.5;
			return 1;
		}
	}
	return 0;
}
int PK2Sprite::AI_Kiipeilija(){
	if (energia > 0)
	{
		if (!alas && vasemmalle)
		{
			b = 0;
			a = this->type->max_nopeus / -3.5;
			//return 1;
		}

		if (!ylos && oikealle)
		{
			b = 0;
			a = this->type->max_nopeus / 3.5;
			//b = this->type->max_nopeus / 3.5;
			//return 1;
		}

		if (!oikealle && alas)
		{
			a = 0;
			b = this->type->max_nopeus / 3.5;
			//return 1;
		}

		if (!vasemmalle && ylos)
		{
			a = 0;
			b = this->type->max_nopeus / -3.5;
			//return 1;
		}
	}
	return 0;
}
int PK2Sprite::AI_Kiipeilija2(){
	if (energia > 0){
		if (vasemmalle && oikealle && ylos && alas) {

			if (a < 0) {
				b = this->type->max_nopeus / 3.5;
				//a = 0;
			}
			else if (a > 0) {
				b = this->type->max_nopeus / -3.5;
				//a = 0;
			}
			else if (b < 0) {
				a = this->type->max_nopeus / -3.5;
				//b = 0;
			}
			else if (b > 0) {
				a = this->type->max_nopeus / 3.5;
				//b = 0;
			}
			if (b != 0)
				a = 0;
		}
	}
	return 0;
}
int PK2Sprite::AI_Varoo_Kuoppaa(){
	double max = type->max_nopeus / 3.5;

	if (energia > 0)
	{

		if (this->reuna_oikealla && this->a > -max)
		{
			this->a -= 0.13;
		}

		if (this->reuna_vasemmalla && this->a < max)
		{
			this->a += 0.13;
		}

		/*
		if (this->reuna_oikealla && this->a > 0)
		{
			this->a = this->a * -1;
			flip_x = true;
		}

		if (this->reuna_vasemmalla && this->a < 0)
		{
			this->a = this->a * -1;
			flip_x = false;
		}
	*/
	}
	return 0;
}
int PK2Sprite::AI_Random_Hyppy(){
	if (energia > 0)
	{
		if (rand()%150 == 10 && b == 0 && hyppy_ajastin == 0 && ylos)
		{
			hyppy_ajastin = 1;
		}
	}
	return 0;
}
int PK2Sprite::AI_Sammakko1(){
	if (energia > 0)
	{
		if (ajastin%100 == 0 && hyppy_ajastin == 0 && ylos)
		{
			hyppy_ajastin = 1;
		}
	}
	return 0;
}
int PK2Sprite::AI_Sammakko2(){
	if (energia > 0)
	{
		if (ajastin%100 == 0 && ylos)
		{
			hyppy_ajastin = 1;

		}

		if (hyppy_ajastin > 0)
		{
			if (!flip_x)
				a = this->type->max_nopeus / 3.5;
			else
				a = this->type->max_nopeus / -3.5;
		}
	}
	return 0;
}
int PK2Sprite::AI_Random_Suunnanvaihto_Hori(){
	if (energia > 0)
	{
		if (rand()%150 == 1)
		{
			int max = (int)type->max_nopeus / 4;

			while (a == 0 && max > 0)
				a = rand()%max+1 - rand()%max+1;
		}
	}
	return 0;
}
int PK2Sprite::AI_Random_Kaantyminen(){
	if (energia > 0)
	{
		if (ajastin%400 == 1 && a == 0)
		{
			flip_x = !flip_x;
		}
	}
	return 0;
}
int PK2Sprite::AI_Kaantyy_Jos_Osuttu(){
	int dam = (VAHINKO_AIKA > 0 && energia > 0)? 1 : 0; //Damage
	if (isku == dam) {
		if (a != 0)
			a = -a;

		flip_x = !flip_x;
	}
	return 0;
}
int PK2Sprite::AI_Random_Liikahdus_Vert_Hori(){
	if (energia > 0)
	{
		if (rand()%150 == 1 || ajastin == 1)
		if ((int)a == 0 || (int)b == 0)
		{
			int max = (int)type->max_nopeus;

			if (max != 0)
			{
				while (a == 0)
					a = rand()%(max+1) - rand()%(max+1);

				while (b == 0)
					b = rand()%(max+1) - rand()%(max+1);

				//a /= 3.0;
				//b /= 3.0;
			}

		}
	}
	return 0;
}
int PK2Sprite::AI_Seuraa_Pelaajaa(PK2Sprite* pelaaja){
	if (energia > 0 && pelaaja->energia > 0)
	{
		double max = type->max_nopeus / 3.5;

		if (a > -max && x > pelaaja->x)
		{
			a -= 0.1;
		}

		if (a < max && x < pelaaja->x)
		{
			a += 0.1;
		}

		pelaaja_x = (int)(pelaaja->x + pelaaja->a);
		pelaaja_y = (int)(pelaaja->y + pelaaja->b);

		if (type->max_nopeus == 0)
		{
			if (pelaaja->x < x)
				flip_x = true;
			else
				flip_x = false;
		}
	}
	return 0;
}
int PK2Sprite::AI_Pakenee_Pelaajaa_Jos_Nakee(PK2Sprite *pelaaja){
	if (energia > 0 && pelaaja->energia > 0)
	{
		if ((pelaaja->x < x && flip_x && !pelaaja->flip_x) || (pelaaja->x > x && !flip_x && pelaaja->flip_x))
			if ((pelaaja->x - x < 300 && pelaaja->x - x > -300) &&
				(pelaaja->y - y < type->korkeus && pelaaja->y - y > -type->korkeus))
			{
				double max = type->max_nopeus / 2.5;

				if (x > pelaaja->x) {
					a = max;
					flip_x = false;
				}

				if (x < pelaaja->x) {
					a = max * -1;
					flip_x = true;
				}
			}
	}


	return 0;
}
int PK2Sprite::AI_Seuraa_Pelaajaa_Jos_Nakee(PK2Sprite *pelaaja){
	if (energia > 0  && pelaaja->energia > 0){
		double max = type->max_nopeus / 3.5;

		if (pelaaja_x != -1){
			if (a > -max && x > pelaaja_x)
				a -= 0.1;

			if (a < max && x < pelaaja_x)
				a += 0.1;
		}

		if ((pelaaja->x < x && flip_x) || (pelaaja->x > x && ! flip_x)){
			if ((pelaaja->x - x < 300 && pelaaja->x - x > -300) &&
				(pelaaja->y - y < type->korkeus && pelaaja->y - y > -type->korkeus)){
				pelaaja_x = (int)(pelaaja->x+pelaaja->a);
				pelaaja_y = (int)(pelaaja->y+pelaaja->b);
			}
			else{
				pelaaja_x = -1;
				pelaaja_y = -1;
			}
		}
	}


	return 0;
}
int PK2Sprite::AI_Jahtaa_Pelaajaa(PK2Sprite *pelaaja){
	if (energia > 0 && pelaaja->energia > 0)
	{
		double max = type->max_nopeus / 3.5;

		if (pelaaja_x != -1)
		{
			if (a > -max && x > pelaaja_x)
				a -= 0.1;

			if (a < max && x < pelaaja_x)
				a += 0.1;

			if (x - pelaaja_x > -8 && x - pelaaja_x < 8) // onko sprite saavuttanut pelaajan viime sijainnin
				pelaaja_x = -1;
		}

		if (pelaaja_y != -1 && paino == 0)
		{
			if (b > -max && y > pelaaja_y)
				b -= 0.1;

			if (b < max && y < pelaaja_y)
				b += 0.1;

			if (y - pelaaja_y > -8 && y - pelaaja_y < 8) // onko sprite saavuttanut pelaajan viime sijainnin
				pelaaja_y = -1;
		}

		if (((pelaaja->x < x && !flip_x) || (pelaaja->x > x && flip_x)) &&
			pelaaja->hyokkays2 != pelaaja->type->latausaika)
			return 0;

		if ((pelaaja->x - x < 300 && pelaaja->x - x > -300) &&
			(pelaaja->y - y < type->korkeus && pelaaja->y - y > -type->korkeus))
		{
			pelaaja_x = (int)(pelaaja->x+pelaaja->a);
			pelaaja_y = (int)(pelaaja->y+pelaaja->b);
		}
	}


	return 0;
}
int PK2Sprite::AI_Seuraa_Pelaajaa_Vert_Hori(PK2Sprite *pelaaja){
	if (energia > 0 && pelaaja->energia > 0)
	{
		double max = type->max_nopeus / 3.5;

		if (a > -max && x > pelaaja->x)
		{
			a -= 0.1;
		}

		if (a < max && x < pelaaja->x)
		{
			a += 0.1;
		}

		if (b > -max && y > pelaaja->y)
		{
			b -= 0.4;
		}

		if (b < max && y < pelaaja->y)
		{
			b += 0.4;
		}

		pelaaja_x = (int)(pelaaja->x+pelaaja->a);
		pelaaja_y = (int)(pelaaja->y+pelaaja->b);

		if (type->max_nopeus == 0)
		{
			if (pelaaja->x < x)
				flip_x = true;
			else
				flip_x = false;
		}
	}
	return 0;
}
int PK2Sprite::AI_Seuraa_Pelaajaa_Jos_Nakee_Vert_Hori(PK2Sprite *pelaaja){
	if (energia > 0  && pelaaja->energia > 0){
		double max = type->max_nopeus / 3.5;

		if (pelaaja_x != -1){
			if (a > -max && x > pelaaja_x)
				a -= 0.1;

			if (a < max && x < pelaaja_x)
				a += 0.1;

			if (b > -max && y > pelaaja_y)
				b -= 0.4;

			if (b < max && y < pelaaja_y)
				b += 0.4;
		}

		if ((pelaaja->x < x && flip_x) || (pelaaja->x > x && ! flip_x)){
			if ((pelaaja->x - x < 300 && pelaaja->x - x > -300) &&
				(pelaaja->y - y < 80 && pelaaja->y - y > -80)){
				pelaaja_x = (int)(pelaaja->x+pelaaja->a);
				pelaaja_y = (int)(pelaaja->y+pelaaja->b);
			}
			else{
				pelaaja_x = -1;
				pelaaja_y = -1;
			}
		}
	}


	return 0;
}
int PK2Sprite::AI_Transformation_Jos_Energiaa_Alle_2(PK2Sprite_Prototype &muutos){
	if (energia < 2 && muutos.indeksi != type->indeksi)
	{
		type = &muutos;
		alkupaino = type->paino;
		//ammus1 = type->ammus1;
		//ammus2 = type->ammus2;
		return 1;
	}

	return 0;
}
int PK2Sprite::AI_Transformation_Jos_Energiaa_Yli_1(PK2Sprite_Prototype &muutos){
	if (energia > 1 && muutos.indeksi != type->indeksi)
	{
		type = &muutos;
		alkupaino = type->paino;
		//ammus1 = type->ammus1;
		//ammus2 = type->ammus2;
		return 1;
	}

	return 0;
}
int PK2Sprite::AI_Transformation_Ajastin(PK2Sprite_Prototype &muutos){
	if (energia > 0 && muutos.indeksi != type->indeksi)
	{
		if (transformation_ajastin/*lataus*/ == 0)
			transformation_ajastin/*lataus*/ = type->latausaika;

		if (transformation_ajastin/*lataus*/ == 1)
		{
			type = &muutos;
			alkupaino = type->paino;

			ammus1 = type->ammus1;
			ammus2 = type->ammus2;

			current_animation = -1;

			Animate(Animation::STILL,true);
		}
		return 1;
	}

	return 0;
}
int PK2Sprite::AI_Transformation_Jos_Osuttu(PK2Sprite_Prototype &muutos){
	if (energia > 0 && muutos.indeksi != type->indeksi)
	{
		if (saatu_vahinko > 0)
		{
			type = &muutos;
			alkupaino = type->paino;

			ammus1 = type->ammus1;
			ammus2 = type->ammus2;

			current_animation = -1;

			Animate(Animation::STILL,true);

			return 1;
		}
	}

	return 0;
}
int PK2Sprite::AI_Tuhoutuu_Jos_Emo_Tuhoutuu(std::vector<PK2Sprite> &spritet){
	if (emosprite > -1)
	{
		if (spritet[emosprite].energia < 1 && energia > 0)
		{
			saatu_vahinko = energia;
			this->saatu_vahinko_type = VAHINKO_KAIKKI;

			return 1;
		}
	}

	return 0;
}
int PK2Sprite::AI_Hyokkays_1_Jos_Osuttu(){
	if (saatu_vahinko > 0 && energia > 0)
	{
		this->hyokkays1 = this->type->hyokkays1_aika;
		this->lataus = 0;
		return 1;
	}

	return 0;
}
int PK2Sprite::AI_Hyokkays_2_Jos_Osuttu(){
	if (saatu_vahinko > 0 && energia > 0)
	{
		this->hyokkays2 = this->type->hyokkays2_aika;
		this->lataus = 0;
		return 1;
	}

	return 0;
}
int PK2Sprite::AI_Hyokkays_1_Nonstop(){
	if (this->lataus == 0 && energia > 0)
	{
		this->hyokkays1 = this->type->hyokkays1_aika;
		return 1;
	}

	return 0;
}
int PK2Sprite::AI_Hyokkays_2_Nonstop(){
	if (this->lataus == 0 && energia > 0)
	{
		this->hyokkays2 = this->type->hyokkays2_aika;
		return 1;
	}

	return 0;
}
int PK2Sprite::AI_Hyokkays_1_Jos_Pelaaja_Edessa(PK2Sprite *pelaaja){
	if (energia > 0 && isku == 0 && pelaaja->energia > 0)
	{
		if ((pelaaja->x - x < 200 && pelaaja->x - x > -200) &&
			(pelaaja->y - y < type->korkeus && pelaaja->y - y > -type->korkeus))
		{
			if ((pelaaja->x < x && flip_x) || (pelaaja->x > x && !flip_x))
			{
				this->hyokkays1 = this->type->hyokkays1_aika;
				return 1;
			}
		}
	}
	return 0;
}
int PK2Sprite::AI_Hyokkays_2_Jos_Pelaaja_Edessa(PK2Sprite *pelaaja){
	if (energia > 0 && isku == 0 && pelaaja->energia > 0)
	{
		if ((pelaaja->x - x < 200 && pelaaja->x - x > -200) &&
			(pelaaja->y - y < type->korkeus && pelaaja->y - y > -type->korkeus))
		{
			if ((pelaaja->x < x && flip_x) || (pelaaja->x > x && !flip_x))
			{
				this->hyokkays2 = this->type->hyokkays2_aika;
				return 1;
			}
		}
	}
	return 0;
}
int PK2Sprite::AI_Hyokkays_1_Jos_Pelaaja_Alapuolella(PK2Sprite *pelaaja){
	if (energia > 0 && isku == 0 && pelaaja->energia > 0)
	{
		if ((pelaaja->x - x < type->leveys && pelaaja->x - x > -type->leveys) &&
			(pelaaja->y > y && pelaaja->y - y < 350))
		{
			this->hyokkays1 = this->type->hyokkays2_aika;
			return 1;
		}
	}
	return 0;
}
int PK2Sprite::AI_Hyppy_Jos_Pelaaja_Ylapuolella(PK2Sprite *pelaaja){
	if (energia > 0 && hyppy_ajastin == 0 && pelaaja->energia > 0)
	{
		if ((pelaaja->x - x < type->leveys && pelaaja->x - x > -type->leveys) &&
			(pelaaja->y < y && y - pelaaja->y < 350))
		{
			hyppy_ajastin = 1;
			return 1;
		}
	}
	return 0;
}
int PK2Sprite::AI_NonStop(){
	if (energia > 0)
	{

		double max = type->max_nopeus / 3.5;

		if (flip_x)
		{
			if (a > -max)
				a -= 0.1;
		}
		else
		{
			if (a < max)
				a += 0.1;
		}
	}

	return 0;
}
int PK2Sprite::AI_Kitka_Vaikuttaa(){

	if (energia > 0)
	{
		if (!alas)
			a /= 1.07;
		else
			a /= 1.02;
	}

	return 0;
}
int PK2Sprite::AI_Piiloutuu(){

	if (energia > 0 && piilossa)
	{
		a /= 1.02;
		kyykky = true;
	}

	return 0;
}
int PK2Sprite::AI_Palaa_Alkuun_X(){

	if (energia < 1 || pelaaja_x !=  -1)
		return 0;

	double max = type->max_nopeus / 3.5;

	if (x < alku_x-16 && a < max)
		a += 0.05;

	if (x > alku_x+16 && a > -max)
		a -= 0.05;

	return 0;
}
int PK2Sprite::AI_Palaa_Alkuun_Y(){

	if (energia > 0 && pelaaja_x == -1)
	{
		double max = type->max_nopeus / 3.5;

		if (y < alku_y-16 && b < max)
			b += 0.04;

		if (y > alku_y+16 && b > -max)
			b -= 0.04;
	}

	return 0;
}
int PK2Sprite::AI_Tippuu_Tarinasta(int tarina){

	if (energia > 0 && tarina > 0)
	{
		alkupaino = 0.5;
	}

	return 0;
}
int PK2Sprite::AI_Vahingoittuu_Vedesta(){
	if (energia > 0)
		if (this->vedessa)
			saatu_vahinko++;

	return 0;
}
int PK2Sprite::AI_Tapa_Kaikki(){
	if (energia > 0)
		this->vihollinen = !this->vihollinen;

	return 0;
}
int PK2Sprite::AI_Hyppija(){
	if (x < 10)
	{
		x = 10;
	}

	if (y > 9920)
	{
		y = 9920;
	}

	if (energia > 0)
	{
		if (!alas && b==0 && hyppy_ajastin == 0)
		{
			hyppy_ajastin = 1;
		}
	}

	if (a < 0)
		flip_x = true;

	if (a > 0)
		flip_x = false;

	return 0;
}
int PK2Sprite::AI_Liikkuu_X(double liike){
	if (energia > 0)
		this->x = this->alku_x + liike;

	return 0;
}
int PK2Sprite::AI_Liikkuu_Y(double liike){
	if (energia > 0)
	{
		this->y = this->alku_y + liike;
	}

	return 0;
}
int PK2Sprite::AI_Tippuu_Jos_Kytkin_Painettu(int kytkin){
	if (kytkin > 0)
	{
		alkupaino = 1.5;
	}

	return 0;
}
int PK2Sprite::AI_Liikkuu_Jos_Kytkin_Painettu(int kytkin, int ak, int bk){
	if (kytkin > 0)
	{
		if (a == 0 && ak != 0)
		{
			a = type->max_nopeus / 3.5 * ak; // ak = -1 / 1
		}

		if (b == 0 && bk != 0)
			b = type->max_nopeus / 3.5 * bk; // bk = -1 / 1
	}

	flip_x = false;

	return 0;
}
bool PK2Sprite::AI_Info(PK2Sprite &pelaaja){
	if ((pelaaja.x - x < 10 && pelaaja.x - x > -10) &&
		(pelaaja.y - y < type->korkeus && pelaaja.y - y > -type->korkeus))
	{
		return true;
	}

	return false;
}
int PK2Sprite::AI_Kana(){
	if (x < 10)
	{
		x = 10;
	}

	if (y > 9920)
	{
		y = 9920;
	}

	if (energia > 0)
	{

		if (rand()%50 == 10 && a != 0)
			a /= 1.1;

		if (rand()%150 == 10 && b == 0 && hyppy_ajastin == 0 && ylos)
		{
			hyppy_ajastin = 1;
			while (a == 0)
				a = rand()%2 - rand()%2;
		}

		if (rand()%20 == 1 && b == 0 && hyppy_ajastin == 0 && !oikealle && !flip_x)
		{
			hyppy_ajastin = 1;
			while (a == 0)
				a = rand()%2;
		}

		if (rand()%20 == 1 && b == 0 && hyppy_ajastin == 0 && !vasemmalle && flip_x)
		{
			hyppy_ajastin = 1;
			while (a == 0)
				a = rand()%2 * -1;
		}

		if (rand()%200 == 10)
			a = rand()%2 - rand()%2;

		if (hyppy_ajastin == type->max_hyppy && a == 0)
		{
			while (a == 0)
				a = rand()%2 - rand()%2;
		}
		/* // reunatesti
		if (rand()%100 == 2)
			a = rand()%2-rand()%2;

		if (reuna_vasemmalla && a < 0)
			a = 0;

		if (reuna_oikealla && a > 0)
			a = 0;*/

		if (a < 0)
			flip_x = true;

		if (a > 0)
			flip_x = false;

	}

	return 0;
}
int PK2Sprite::AI_Bonus(){
	if (x < 10)
	{
		x = 10;
	}

	if (y > 9920)
	{
		y = 9920;
	}

	return 0;
}
int PK2Sprite::AI_Muna(){
	if (x < 10)
	{
		x = 10;
	}

	if (y > 9920)
	{
		y = 9920;
	}

	if (!alas)
		energia = 0;

	//a /= 1.01;

	if (energia == 0 && lataus == 0)
		lataus = type->latausaika;

	if (this->lataus == 1)
		this->piilota = true;

	return 0;
}
int PK2Sprite::AI_Ammus(){
	if (x < 10)
	{
		x = 10;
	}

	if (y > 9920)
	{
		y = 9920;
	}

	if (a < 0)
		flip_x = true;

	if (a > 0)
		flip_x = false;

	if (this->lataus == 0)
		lataus = this->type->latausaika;

	if (this->lataus == 1)
	{
		this->saatu_vahinko = this->type->energia;
		this->saatu_vahinko_type = VAHINKO_KAIKKI;
	}

	if (energia < 1)
		piilota = true;

	return 0;
}
int PK2Sprite::AI_Pommi(){
	if (this->lataus == 0)
		lataus = this->type->latausaika;

	if (this->lataus == 1)
	{
		this->saatu_vahinko = this->energia;
		this->saatu_vahinko_type = VAHINKO_KAIKKI;
	}

	return 0;
}
int PK2Sprite::AI_Teleportti(int oma_i, std::vector<PK2Sprite> &spritet, int max, PK2Sprite &sprite){
	int siirto = 0;

	if (energia > 0 && lataus == 0 && hyokkays1 == 0)
	{
		if (sprite.x <= x + type->leveys /2 && sprite.x >= x - type->leveys /2 &&
			sprite.y <= y + type->korkeus/2 && sprite.y >= y - type->korkeus/2 )
		{

			int portit[1200];
			int portti_index = 0;
			int porttien_maara = 0;
			int i = 0;

			// alustetaan portit-taulukko, johon kerataan kaikkien teleportti spritejen indeksit
			for (i=0;i<max;i++)
				portit[i] = -1;

			// etsitaan SAMANtypeSET teleportit
			for (i=0;i<max;i++)
				if (spritet[i].type != NULL)
					if (spritet[i].type->type == type_TELEPORTTI &&
						type->indeksi == spritet[i].type->indeksi)
					{
						portit[portti_index] = i;
						portti_index++;
						porttien_maara++;
					}

			// jos yhtaan samantypesta ei laydy...
			if (porttien_maara == 0)
			{
				// ...etsitaan KAIKKI teleportit
				portti_index = 0;
				porttien_maara = 0;

				for (i=0;i<max;i++)
					if (spritet[i].type != NULL)
						if (spritet[i].type->type == type_TELEPORTTI)
						{
							portit[portti_index] = i;
							portti_index++;
							porttien_maara++;
						}
			}

			// jos vielakaan ei laydy yhtaan teleporttia (poislukien teleportti itse), poistutaan.
			if (porttien_maara <= 1)
				return 0;

			if (porttien_maara > 1119/*599*/)
				porttien_maara = 1119/*599*/;

			// arvotaan kohdeportti
			i = portit[rand()%porttien_maara];

			while(i == oma_i || i == -1)
				i = portit[rand()%porttien_maara];

			if (spritet[i].type != NULL)
			{
				sprite.x = spritet[i].x;
				sprite.y = spritet[i].y;
				//lataus    = type->latausaika;
				//hyokkays1 = type->hyokkays1_aika;
				//spritet[i].lataus    = spritet[i].type->latausaika;
				spritet[i].hyokkays1 = spritet[i].type->hyokkays1_aika;
				lataus = 0;
				spritet[i].lataus = 0;
				siirto = 1;
			}
		}
	}

	return siirto;
}

int PK2Sprite::Animaatio_Perus(){

	int uusi_animaatio = -1;
	bool alusta = false;

	if (energia < 1 && !alas)
	{
		uusi_animaatio = Animation::KNOCK_OUT;
		alusta = true;
	}
	else
	{

		if (a > -0.2 && a < 0.2 && b == 0 && hyppy_ajastin <= 0)
		{
			uusi_animaatio = Animation::STILL;
			alusta = true;
		}

		if ((a < -0.2 || a > 0.2) && hyppy_ajastin <= 0)
		{
			uusi_animaatio = Animation::WALKING;
			alusta = false;
		}

		if (b < 0)//-0.3
		{
			uusi_animaatio = Animation::JUMP_UP;
			alusta = false;
		}

		if ((hyppy_ajastin > type->max_hyppy || b > 1.5) && alas)
		{
			uusi_animaatio = Animation::JUMP_DOWN;
			alusta = false;
		}

		if (kyykky)
		{
			uusi_animaatio = Animation::DUCK;
			alusta = true;
		}

		if (hyokkays1 > 0)
		{
			uusi_animaatio = Animation::ATTACK1;
			alusta = true;
		}

		if (hyokkays2 > 0)
		{
			uusi_animaatio = Animation::ATTACK2;
			alusta = true;
		}

		if (isku > 0)
		{
			uusi_animaatio = Animation::DAMAGE;
			alusta = false;
		}

	}

	if (uusi_animaatio != -1)
		Animate(uusi_animaatio,alusta);

	return 0;
}

int PK2Sprite::Animaatio_Kana(){

	int new_animation = -1;
	bool alusta = false;

	if (energia < 1 && !alas) {
		new_animation = Animation::KNOCK_OUT;
		alusta = true;
	} else {
		if (a > -0.2 && a < 0.2 && b == 0 && hyppy_ajastin <= 0) {
			new_animation = Animation::STILL;
			alusta = true;
		}

		if ((a < -0.2 || a > 0.2) && hyppy_ajastin <= 0) {
			new_animation = Animation::WALKING;
			alusta = false;
		}

		if (b < 0) {
			new_animation = Animation::JUMP_UP;
			alusta = false;
		}

		if ((hyppy_ajastin > 90+10/*type->max_hyppy || b > 1.5*/) && alas) {
			new_animation = Animation::JUMP_DOWN;
			alusta = false;
		}

		if (hyokkays1 > 0) {
			new_animation = Animation::ATTACK1;
			alusta = true;
		}

		if (hyokkays2 > 0) {
			new_animation = Animation::ATTACK2;
			alusta = true;
		}

		if (kyykky) {
			new_animation = Animation::DUCK;
			alusta = true;
		}

		if (isku > 0) {
			new_animation = Animation::DAMAGE;
			alusta = false;
		}

	}

	if (new_animation != -1)
		Animate(new_animation, alusta);

	return 0;
}
int PK2Sprite::Animaatio_Bonus(){
	Animate(Animation::STILL, true);
	return 0;
}
int PK2Sprite::Animaatio_Ammus(){
	Animate(Animation::STILL, true);
	return 0;
}
int PK2Sprite::Animaatio_Muna(){
	int new_animation = 0;
	bool alusta = false;

	new_animation = Animation::STILL;
	alusta = true;

	if (energia < type->energia){
		new_animation = Animation::KNOCK_OUT;
		alusta = true;
	}

	Animate(new_animation, alusta);

	return 0;
}

void PK2Sprite::Animate_Checkpoint() {
	if (checkpoint_activated) {
		if (current_animation == Animation::KNOCK_OUT && current_frame == type->animations[Animation::KNOCK_OUT].last_frame) {
			knockout_complete = true;
		}

		if (knockout_complete) {
			Animate(Animation::WALKING, false);
		} else {
			Animate(Animation::KNOCK_OUT, false);
		}
	} else if (!checkpoint_activated) {
		Animate(Animation::STILL, type->animations[Animation::STILL].loop);
	}
}