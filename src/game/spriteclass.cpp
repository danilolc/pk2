//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "game/spriteclass.hpp"

#include "system.hpp"
#include "gfx/effect.hpp"

#include "engine/PDraw.hpp"
#include "engine/PSound.hpp"
#include "engine/PLog.hpp"
#include "engine/platform.hpp"

#include <cstring>

/* -------- SpriteClass Prototyyppi ------------------------------------------------------------------ */

PrototypeClass::PrototypeClass(){}

PrototypeClass::~PrototypeClass(){
	for (int i=0;i<SPRITE_MAX_FRAMEJA;i++) {
		if (framet[i] > 0)
			PDraw::image_delete(this->framet[i]);
		if (this->framet_peilikuva[i] != 0)
			PDraw::image_delete(this->framet_peilikuva[i]);
	}

	for (int j = 0; j < SPRITE_MAX_SOUNDS; j++)
		PSound::free_sfx(aanet[j]);
}

void PrototypeClass::SetProto10(PrototypeClass10 &proto){
	strncpy(kuvatiedosto,  proto.kuvatiedosto,   13);
	strncpy(nimi,          proto.nimi,           30);
	strncpy(muutos_sprite, proto.muutos_sprite,  13);
	strncpy(bonus_sprite,  proto.bonus_sprite,   13);
	strncpy(ammus1_sprite, proto.ammus1_sprite,  13);
	strncpy(ammus2_sprite, proto.ammus2_sprite,  13);

	for (int aani=0;aani<SPRITE_MAX_SOUNDS;aani++) {
		strncpy(aanitiedostot[aani], proto.aanitiedostot[aani], 13);
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
	charge_time			= proto.charge_time;
	leveys				= proto.leveys;
	max_hyppy			= proto.max_hyppy;
	max_nopeus			= proto.max_nopeus;
	weight				= proto.weight;
	pisteet				= proto.pisteet;
	suojaus				= (u8) proto.suojaus;
	tuhoutuminen		= proto.tuhoutuminen;
	tyyppi				= proto.tyyppi;
	vahinko				= proto.vahinko;
	vari				= proto.vari;
	vihollinen			= proto.vihollinen;

	for (int i=0;i<5;i++)
		AI[i] = proto.AI[i];

	for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++) {
		for (int j=0;j<ANIMATION_SEQUENCE_SIZE;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];

		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;
	}
}
void PrototypeClass::SetProto11(PrototypeClass11 &proto){
	strncpy(kuvatiedosto,  proto.kuvatiedosto,   13);
	strncpy(nimi,          proto.nimi,           30);
	strncpy(muutos_sprite, proto.muutos_sprite,  13);
	strncpy(bonus_sprite,  proto.bonus_sprite,   13);
	strncpy(ammus1_sprite, proto.ammus1_sprite,  13);
	strncpy(ammus2_sprite, proto.ammus2_sprite,  13);

	for (int aani=0;aani<SPRITE_MAX_SOUNDS;aani++) {
		strncpy(aanitiedostot[aani], proto.aanitiedostot[aani], 13);
		aanet[aani] = proto.aanet[aani];
	}

	animaatioita		= proto.animaatioita;
	avain				= proto.avain;
	bonusten_lkm        = proto.bonusten_lkm;
	energia				= proto.energia;
	este				= proto.este;
	frameja				= proto.frameja;
	frame_rate			= proto.frame_rate;
	attack1_time		= proto.attack1_time;
	attack2_time		= proto.attack2_time;
	kuva_x				= proto.kuva_x;
	kuva_y				= proto.kuva_y;
	kuva_frame_leveys	= proto.kuva_frame_leveys;
	kuva_frame_korkeus	= proto.kuva_frame_korkeus;
	korkeus				= proto.korkeus;
	charge_time			= proto.charge_time;
	leveys				= proto.leveys;
	max_hyppy			= proto.max_hyppy;
	max_nopeus			= proto.max_nopeus;
	weight				= proto.weight;
	pallarx_kerroin		= proto.pallarx_kerroin;
	pisteet				= proto.pisteet;
	suojaus				= proto.suojaus;
	vibrate				= proto.vibrate;
	tuhoutuminen		= proto.tuhoutuminen;
	tyyppi				= proto.tyyppi;
	vahinko				= proto.vahinko;
	vahinko_tyyppi		= proto.vahinko_tyyppi;
	vari				= proto.vari;
	vihollinen			= proto.vihollinen;

	for (int i=0;i<5;i++)
		AI[i] = proto.AI[i];

	for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++) {
		for (int j=0;j<ANIMATION_SEQUENCE_SIZE;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];

		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;
	}
}
void PrototypeClass::SetProto12(PrototypeClass12 &proto){
	strncpy(kuvatiedosto,  proto.kuvatiedosto,  100);
	strncpy(nimi,          proto.nimi,           30);
	strncpy(muutos_sprite, proto.muutos_sprite, 100);
	strncpy(bonus_sprite,  proto.bonus_sprite,  100);
	strncpy(ammus1_sprite, proto.ammus1_sprite, 100);
	strncpy(ammus2_sprite, proto.ammus2_sprite, 100);

	for (int aani=0;aani<SPRITE_MAX_SOUNDS;aani++) {
		strncpy(aanitiedostot[aani], proto.aanitiedostot[aani], 13);
		aanet[aani] = proto.aanet[aani];
	}

	aani_frq			= proto.aani_frq;
	animaatioita		= proto.animaatioita;
	avain				= proto.avain;
	bonusten_lkm        = proto.bonusten_lkm;
	energia				= proto.energia;
	este				= proto.este;
	este_ylos			= proto.este_ylos;
	este_alas			= proto.este_alas;
	este_oikealle		= proto.este_oikealle;
	este_vasemmalle		= proto.este_vasemmalle;
	frameja				= proto.frameja;
	frame_rate			= proto.frame_rate;
	attack1_time		= proto.attack1_time;
	attack2_time		= proto.attack2_time;
	kuva_x				= proto.kuva_x;
	kuva_y				= proto.kuva_y;
	kuva_frame_leveys	= proto.kuva_frame_leveys;
	kuva_frame_korkeus	= proto.kuva_frame_korkeus;
	korkeus				= proto.korkeus;
	charge_time			= proto.charge_time;
	leveys				= proto.leveys;
	max_hyppy			= proto.max_hyppy;
	max_nopeus			= proto.max_nopeus;
	weight				= proto.weight;
	pallarx_kerroin		= proto.pallarx_kerroin;
	pisteet				= proto.pisteet;
	random_frq			= proto.random_frq;
	suojaus				= proto.suojaus;
	vibrate				= proto.vibrate;
	tiletarkistus		= proto.tiletarkistus;
	tuhoutuminen		= proto.tuhoutuminen;
	tyyppi				= proto.tyyppi;
	vahinko				= proto.vahinko;
	vahinko_tyyppi		= proto.vahinko_tyyppi;
	vari				= proto.vari;
	vihollinen			= proto.vihollinen;

	for (int i=0;i<5;i++)
		AI[i] = proto.AI[i];

	for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++) {
		for (int j=0;j<ANIMATION_SEQUENCE_SIZE;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];

		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;
	}
}
void PrototypeClass::SetProto13(PrototypeClass13 &proto){
	strncpy(kuvatiedosto,  proto.kuvatiedosto,  100);
	strncpy(nimi,          proto.nimi,           30);
	strncpy(muutos_sprite, proto.muutos_sprite, 100);
	strncpy(bonus_sprite,  proto.bonus_sprite,  100);
	strncpy(ammus1_sprite, proto.ammus1_sprite, 100);
	strncpy(ammus2_sprite, proto.ammus2_sprite, 100);

	for (int aani=0;aani<SPRITE_MAX_SOUNDS;aani++)
	{
		strncpy(aanitiedostot[aani], proto.aanitiedostot[aani], 100);
		aanet[aani] = proto.aanet[aani];
	}

	aani_frq			= proto.aani_frq;
	animaatioita		= proto.animaatioita;
	avain				= proto.avain;
	bonusten_lkm        = proto.bonusten_lkm;
	energia				= proto.energia;
	este				= proto.este;
	este_ylos			= proto.este_ylos;
	este_alas			= proto.este_alas;
	este_oikealle		= proto.este_oikealle;
	este_vasemmalle		= proto.este_vasemmalle;
	frameja				= proto.frameja;
	frame_rate			= proto.frame_rate;
	attack1_time		= proto.attack1_time;
	attack2_time		= proto.attack2_time;
	kuva_x				= proto.kuva_x;
	kuva_y				= proto.kuva_y;
	kuva_frame_leveys	= proto.kuva_frame_leveys;
	kuva_frame_korkeus	= proto.kuva_frame_korkeus;
	korkeus				= proto.korkeus;
	charge_time			= proto.charge_time;
	leveys				= proto.leveys;
	max_hyppy			= proto.max_hyppy;
	max_nopeus			= proto.max_nopeus;
	weight				= proto.weight;
	pallarx_kerroin		= proto.pallarx_kerroin;
	pisteet				= proto.pisteet;
	random_frq			= proto.random_frq;
	suojaus				= proto.suojaus;
	vibrate				= proto.vibrate;
	tiletarkistus		= proto.tiletarkistus;
	tuhoutuminen		= proto.tuhoutuminen;
	tyyppi				= proto.tyyppi;
	vahinko				= proto.vahinko;
	vahinko_tyyppi		= proto.vahinko_tyyppi;
	vari				= proto.vari;
	vihollinen			= proto.vihollinen;

	effect		= proto.effect;
	hehkuu				= proto.hehkuu;
	tulitauko			= proto.tulitauko;
	can_glide			= proto.can_glide;
	boss				= proto.boss;
	bonus_always			= proto.bonus_always;
	can_swim			= proto.can_swim;

	for (int i=0;i<10;i++)
		AI[i] = proto.AI[i];

	for (int i=0;i<SPRITE_MAX_ANIMAATIOITA;i++) {
		for (int j=0;j<ANIMATION_SEQUENCE_SIZE;j++)
			animaatiot[i].sekvenssi[j] = proto.animaatiot[i].sekvenssi[j];

		animaatiot[i].looppi  = proto.animaatiot[i].looppi;
		animaatiot[i].frameja = proto.animaatiot[i].frameja;
	}
}

int PrototypeClass::Load(PFile::Path path){

	PFile::RW* file = path.GetRW("r");
	if (file == nullptr) {
		PLog::Write(PLog::ERR, "PK2", "Failed to open %s", path.c_str());
		return -1;
	}

	char versio[4];
	file->read(versio, 4);

	if (strcmp(versio,"1.0") == 0){
		PrototypeClass10 proto;
		file->read(&proto, sizeof(proto));
		this->SetProto10(proto);
	}
	else if (strcmp(versio,"1.1") == 0){
		PrototypeClass11 proto;
		file->read(&proto, sizeof(proto));
		this->SetProto11(proto);
	}
	else if (strcmp(versio,"1.2") == 0){
		PrototypeClass12 proto;
		file->read(&proto, sizeof(proto));
		this->SetProto12(proto);
	}
	else if (strcmp(versio,"1.3") == 0){
		PrototypeClass13 proto;
		file->read(&proto, sizeof(proto));
		this->SetProto13(proto);
	}
	else {
		PLog::Write(PLog::ERR, "PK2", "Can't support sprite version %s", versio);
		return -1;
	}
	strcpy(this->versio, versio);
	strcpy(this->tiedosto, path.GetFileName().c_str());

	file->close();

	// Get sprite bmp
	PFile::Path image = path;
	image.SetFile(this->kuvatiedosto);
	if (!FindAsset(&image, "sprites" PE_SEP)) {

		PLog::Write(PLog::ERR, "PK2", "Couldn't find sprite image %s", this->kuvatiedosto);
		return -1;

	}

	int bufferi = PDraw::image_load(image, false);
	if (bufferi == -1) {

		PLog::Write(PLog::ERR, "PK2", "Couldn't load sprite image %s", this->kuvatiedosto);
		return -1;

	}

	//Change sprite colors
	if (this->vari != COLOR_NORMAL){ 

		int w, h;
		u8 color;
		PDraw::image_getsize(bufferi,w,h);

		u8 *buffer = NULL;
		u32 leveys;
		PDraw::drawimage_start(bufferi, buffer, leveys);

		for (int x = 0; x < w; x++)
			for (int y = 0; y < h; y++)
				if ((color = buffer[x+y*leveys]) != 255) {
					color %= 32;
					color += this->vari;
					buffer[x+y*leveys] = color;
				}

		PDraw::drawimage_end(bufferi);
	}

	int frame_x = kuva_x;
	int frame_y = kuva_y;

	//Get each frame
	for (int frame_i = 0; frame_i < frameja; frame_i++) {

		if (frame_x + kuva_frame_leveys > 640) {
			frame_y += this->kuva_frame_korkeus + 3;
			frame_x = kuva_x;
		}

		framet[frame_i] = PDraw::image_cut(bufferi,frame_x,frame_y,kuva_frame_leveys,kuva_frame_korkeus); //frames
		framet_peilikuva[frame_i] = PDraw::image_cut(bufferi,frame_x,frame_y,kuva_frame_leveys,kuva_frame_korkeus); //flipped frames
		PDraw::image_fliphori(framet_peilikuva[frame_i]);

		frame_x += this->kuva_frame_leveys + 3;
	
	}

	PDraw::image_delete(bufferi);

	//Load sounds
	for (int i = 0; i < SPRITE_MAX_SOUNDS; i++) {

		if (strcmp(aanitiedostot[i], "") != 0) {

			PFile::Path sound = path;
			sound.SetFile(aanitiedostot[i]);

			if (FindAsset(&sound, "sprites" PE_SEP)) {

				aanet[i] = PSound::load_sfx(sound);

			} else {

				PLog::Write(PLog::ERR, "PK2", "Can't find sound %s", aanitiedostot[i]);
				//return -1;

			}
		}
	}

	return 0;
}

int PrototypeClass::Piirra(int x, int y, int frame){
	PDraw::image_clip(framet[frame], x, y);
	return 0;
}
bool PrototypeClass::Onko_AI(int ai){
	for (int i = 0; i < SPRITE_MAX_AI; i++)
		if (AI[i] == ai)
			return true;
	return false;
}

/* -------- SpriteClass  ------------------------------------------------------------------ */

SpriteClass::SpriteClass(){}
SpriteClass::SpriteClass(PrototypeClass *tyyppi, int pelaaja, double x, double y){
	if (tyyppi) {

		this->tyyppi         = tyyppi;
		this->pelaaja        = pelaaja;

		this->aktiivinen     = true;
		this->piilota        = false;

		this->x              = x;
		this->y              = y;
		this->alku_x         = x;
		this->alku_y         = y;
		
		this->energia        = tyyppi->energia;
		this->initial_weight = tyyppi->weight;
		this->weight         = tyyppi->weight;
		
		this->vihollinen     = tyyppi->vihollinen;
		this->ammus1         = tyyppi->ammus1;
		this->ammus2         = tyyppi->ammus2;
	
	}
}

SpriteClass::~SpriteClass() {}

bool SpriteClass::Onko_AI(int ai){
	
	return tyyppi->Onko_AI(ai);

}
int SpriteClass::Animaatio(int anim_i, bool reset){
	if (anim_i != animation_index){
		if (reset)
			current_sequence = 0;

		animation_index = anim_i;
	}

	return 0;
}
int SpriteClass::Animoi(){
	
	switch (tyyppi->AI[0]) {
		case AI_KANA:           Animation_Kana();  break;
		case AI_LITTLE_CHICKEN: Animation_Kana();  break;
		case AI_BONUS:          Animation_Bonus(); break;
		case AI_EGG:            Animation_Egg();   break;
		case AI_EGG2:           Animation_Egg();   break;
		case AI_AMMUS:          Animation_Ammus(); break;
		case AI_JUMPER:         Animation_Kana();  break;
		case AI_BASIC:          Animation_Perus(); break;
		case AI_TELEPORT:       Animation_Perus(); break;
		default:                break;
	}

	PK2SPRITE_ANIMAATIO &animaatio = tyyppi->animaatiot[animation_index];

	if (current_sequence >= animaatio.frameja)
		current_sequence = 0;

	int frame = animaatio.sekvenssi[current_sequence] - 1;

	// Calculate how much of the currently valid frame is still displayed
	if (frame_timer < tyyppi->frame_rate)
		frame_timer++;
	// If the time has elapsed, the next frame of the current animation is changed
	else {
		frame_timer = 0;

		// Are there more frames in the animation?
		if (current_sequence < animaatio.frameja - 1)
			current_sequence++;
		// If not and the animation is set to loop, the animation is started from the beginning.
		else {
			if (animaatio.looppi)
				current_sequence = 0;
		}
	}

	if (frame < 0)
		frame = 0;
	
	if (frame >= tyyppi->frameja)
		frame = tyyppi->frameja - 1;

	return frame;
}
int SpriteClass::Piirra(int kamera_x, int kamera_y){
	// Tehdaan apumuuttujia
	int	l = (int)tyyppi->kuva_frame_leveys/2,//leveys
		h = (int)tyyppi->kuva_frame_korkeus/2,
		x = (int)this->x-(kamera_x),
		y = (int)this->y-(kamera_y);

	int frame = this->Animoi();

	if (tyyppi->vibrate){
		x += rand()%2 - rand()%2;
		y += rand()%2 - rand()%2;
	}
	
	if (flip_x) {
		
		if(this->invisible_timer)
			PDraw::image_cliptransparent(tyyppi->framet_peilikuva[frame], x-l-1, y-h, 40, COLOR_GRAY);
		else
			PDraw::image_clip(tyyppi->framet_peilikuva[frame], x-l-1, y-h);

	} else {

		if(this->invisible_timer)
			PDraw::image_cliptransparent(tyyppi->framet[frame], x-l-1, y-h, 40, COLOR_GRAY);
		else
			PDraw::image_clip(tyyppi->framet[frame], x-l-1, y-h);
	
	}

	return 0;
}

void SpriteClass::HandleEffects() {

	if (super_mode_timer || tyyppi->effect == EFFECT_STARS)
		Effect_Super(x, y, tyyppi->leveys, tyyppi->korkeus);
		
	if (tyyppi->effect == EFFECT_SMOKE)
		Effect_Points(x - 5, y - 5, tyyppi->leveys, tyyppi->korkeus, tyyppi->vari);

	if (tyyppi->effect == EFFECT_THUNDER && tyyppi->charge_time > 0)
		if (rand() % tyyppi->charge_time == 0) Do_Thunder();

}

int SpriteClass::AI_Basic(){
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

	action_timer++;

	if (action_timer > 31320) // divisible by 360
		action_timer = 0;

	return 0;
}
int SpriteClass::AI_Kaantyy_Esteesta_Hori(){
	if (energia > 0)
	{
		if (!oikealle)
		{
			a = this->tyyppi->max_nopeus / -3.5;
			return 1;
		}

		if (!vasemmalle)
		{
			a = this->tyyppi->max_nopeus / 3.5;
			return 1;
		}
	}
	return 0;
}
int SpriteClass::AI_Kaantyy_Esteesta_Vert(){
	if (energia > 0)
	{
		if (!alas)
		{
			b = this->tyyppi->max_nopeus / -3.5;
			return 1;
		}

		if (!ylos)
		{
			b = this->tyyppi->max_nopeus / 3.5;
			return 1;
		}
	}
	return 0;
}
int SpriteClass::AI_Kiipeilija(){
	if (energia > 0)
	{
		if (!alas && vasemmalle)
		{
			b = 0;
			a = this->tyyppi->max_nopeus / -3.5;
			//return 1;
		}

		if (!ylos && oikealle)
		{
			b = 0;
			a = this->tyyppi->max_nopeus / 3.5;
			//b = this->tyyppi->max_nopeus / 3.5;
			//return 1;
		}

		if (!oikealle && alas)
		{
			a = 0;
			b = this->tyyppi->max_nopeus / 3.5;
			//return 1;
		}

		if (!vasemmalle && ylos)
		{
			a = 0;
			b = this->tyyppi->max_nopeus / -3.5;
			//return 1;
		}
	}
	return 0;
}
int SpriteClass::AI_Kiipeilija2(){
	if (energia > 0){
		if (vasemmalle && oikealle && ylos && alas) {

			if (a < 0) {
				b = this->tyyppi->max_nopeus / 3.5;
				//a = 0;
			}
			else if (a > 0) {
				b = this->tyyppi->max_nopeus / -3.5;
				//a = 0;
			}
			else if (b < 0) {
				a = this->tyyppi->max_nopeus / -3.5;
				//b = 0;
			}
			else if (b > 0) {
				a = this->tyyppi->max_nopeus / 3.5;
				//b = 0;
			}
			if (b != 0)
				a = 0;
		}
	}
	return 0;
}
int SpriteClass::AI_Varoo_Kuoppaa(){
	double max = tyyppi->max_nopeus / 3.5;

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
int SpriteClass::AI_Random_Hyppy(){
	if (energia > 0)
	{
		if (rand()%150 == 10 && b == 0 && jump_timer == 0 && ylos)
		{
			jump_timer = 1;
		}
	}
	return 0;
}
int SpriteClass::AI_Sammakko1(){
	if (energia > 0)
	{
		if (action_timer%100 == 0 && jump_timer == 0 && ylos)
		{
			jump_timer = 1;
		}
	}
	return 0;
}
int SpriteClass::AI_Sammakko2(){
	if (energia > 0)
	{
		if (action_timer%100 == 0 && ylos)
		{
			jump_timer = 1;

		}

		if (jump_timer > 0)
		{
			if (!flip_x)
				a = this->tyyppi->max_nopeus / 3.5;
			else
				a = this->tyyppi->max_nopeus / -3.5;
		}
	}
	return 0;
}
int SpriteClass::AI_Random_Suunnanvaihto_Hori(){
	if (energia > 0)
	{
		if (rand()%150 == 1)
		{
			int max = (int)tyyppi->max_nopeus / 4;

			while (a == 0 && max > 0)
				a = rand()%max+1 - rand()%max+1;
		}
	}
	return 0;
}
int SpriteClass::AI_Random_Kaantyminen(){
	if (energia > 0)
	{
		if (action_timer%400 == 1 && a == 0)
		{
			flip_x = !flip_x;
		}
	}
	return 0;
}
int SpriteClass::AI_Kaantyy_Jos_Osuttu() {

	if (damage_timer == 1 && energia > 0) {
		
		if (a != 0) a = -a;

		flip_x = !flip_x;
	
	}

	return 0;

}
int SpriteClass::AI_Random_Liikahdus_Vert_Hori(){
	if (energia > 0)
	{
		if (rand()%150 == 1 || action_timer == 1)
		if ((int)a == 0 || (int)b == 0)
		{
			int max = (int)tyyppi->max_nopeus;

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
int SpriteClass::AI_Seuraa_Pelaajaa(SpriteClass &pelaaja){
	if (energia > 0 && pelaaja.energia > 0)
	{
		double max = tyyppi->max_nopeus / 3.5;

		if (a > -max && x > pelaaja.x)
		{
			a -= 0.1;
		}

		if (a < max && x < pelaaja.x)
		{
			a += 0.1;
		}

		seen_player_x = (int)(pelaaja.x+pelaaja.a);
		seen_player_y = (int)(pelaaja.y+pelaaja.b);

		if (tyyppi->max_nopeus == 0)
		{
			if (pelaaja.x < x)
				flip_x = true;
			else
				flip_x = false;
		}
	}
	return 0;
}
int SpriteClass::AI_Pakenee_Pelaajaa_Jos_Nakee(SpriteClass &pelaaja){
	if (energia > 0 && pelaaja.energia > 0)
	{
		if ((pelaaja.x < x && flip_x && !pelaaja.flip_x) || (pelaaja.x > x && !flip_x && pelaaja.flip_x))
			if ((pelaaja.x - x < 300 && pelaaja.x - x > -300) &&
				(pelaaja.y - y < tyyppi->korkeus && pelaaja.y - y > -tyyppi->korkeus))
			{
				double max = tyyppi->max_nopeus / 2.5;

				if (x > pelaaja.x) {
					a = max;
					flip_x = false;
				}

				if (x < pelaaja.x) {
					a = max * -1;
					flip_x = true;
				}
			}
	}


	return 0;
}
int SpriteClass::AI_Seuraa_Pelaajaa_Jos_Nakee(SpriteClass &pelaaja){
	if (energia > 0  && pelaaja.energia > 0){
		double max = tyyppi->max_nopeus / 3.5;

		if (seen_player_x != -1){
			if (a > -max && x > seen_player_x)
				a -= 0.1;

			if (a < max && x < seen_player_x)
				a += 0.1;
		}

		if ((pelaaja.x < x && flip_x) || (pelaaja.x > x && ! flip_x)){
			if ((pelaaja.x - x < 300 && pelaaja.x - x > -300) &&
				(pelaaja.y - y < tyyppi->korkeus && pelaaja.y - y > -tyyppi->korkeus)){
				seen_player_x = (int)(pelaaja.x+pelaaja.a);
				seen_player_y = (int)(pelaaja.y+pelaaja.b);
			}
			else{
				seen_player_x = -1;
				seen_player_y = -1;
			}
		}
	}


	return 0;
}
int SpriteClass::AI_Jahtaa_Pelaajaa(SpriteClass &pelaaja){
	if (energia > 0 && pelaaja.energia > 0)
	{
		double max = tyyppi->max_nopeus / 3.5;

		if (seen_player_x != -1)
		{
			if (a > -max && x > seen_player_x)
				a -= 0.1;

			if (a < max && x < seen_player_x)
				a += 0.1;

			if (x - seen_player_x > -8 && x - seen_player_x < 8) // onko sprite saavuttanut pelaajan viime sijainnin
				seen_player_x = -1;
		}

		if (seen_player_y != -1 && weight == 0)
		{
			if (b > -max && y > seen_player_y)
				b -= 0.1;

			if (b < max && y < seen_player_y)
				b += 0.1;

			if (y - seen_player_y > -8 && y - seen_player_y < 8) // onko sprite saavuttanut pelaajan viime sijainnin
				seen_player_y = -1;
		}

		if (((pelaaja.x < x && !flip_x) || (pelaaja.x > x && flip_x)) &&
			pelaaja.attack2_timer != pelaaja.tyyppi->charge_time)
			return 0;

		if ((pelaaja.x - x < 300 && pelaaja.x - x > -300) &&
			(pelaaja.y - y < tyyppi->korkeus && pelaaja.y - y > -tyyppi->korkeus))
		{
			seen_player_x = (int)(pelaaja.x+pelaaja.a);
			seen_player_y = (int)(pelaaja.y+pelaaja.b);
		}
	}


	return 0;
}
int SpriteClass::AI_Seuraa_Pelaajaa_Vert_Hori(SpriteClass &pelaaja){
	if (energia > 0 && pelaaja.energia > 0)
	{
		double max = tyyppi->max_nopeus / 3.5;

		if (a > -max && x > pelaaja.x)
		{
			a -= 0.1;
		}

		if (a < max && x < pelaaja.x)
		{
			a += 0.1;
		}

		if (b > -max && y > pelaaja.y)
		{
			b -= 0.4;
		}

		if (b < max && y < pelaaja.y)
		{
			b += 0.4;
		}

		seen_player_x = (int)(pelaaja.x+pelaaja.a);
		seen_player_y = (int)(pelaaja.y+pelaaja.b);

		if (tyyppi->max_nopeus == 0)
		{
			if (pelaaja.x < x)
				flip_x = true;
			else
				flip_x = false;
		}
	}
	return 0;
}
int SpriteClass::AI_Seuraa_Pelaajaa_Jos_Nakee_Vert_Hori(SpriteClass &pelaaja){
	if (energia > 0  && pelaaja.energia > 0){
		double max = tyyppi->max_nopeus / 3.5;

		if (seen_player_x != -1){
			if (a > -max && x > seen_player_x)
				a -= 0.1;

			if (a < max && x < seen_player_x)
				a += 0.1;

			if (b > -max && y > seen_player_y)
				b -= 0.4;

			if (b < max && y < seen_player_y)
				b += 0.4;
		}

		if ((pelaaja.x < x && flip_x) || (pelaaja.x > x && ! flip_x)){
			if ((pelaaja.x - x < 300 && pelaaja.x - x > -300) &&
				(pelaaja.y - y < 80 && pelaaja.y - y > -80)){
				seen_player_x = (int)(pelaaja.x+pelaaja.a);
				seen_player_y = (int)(pelaaja.y+pelaaja.b);
			}
			else{
				seen_player_x = -1;
				seen_player_y = -1;
			}
		}
	}


	return 0;
}
int SpriteClass::AI_Change_When_Energy_Under_2(PrototypeClass *muutos){
	
	if (energia < 2 && muutos != tyyppi) {
		tyyppi = muutos;
		initial_weight = tyyppi->weight;
		//ammus1 = tyyppi->ammus1;
		//ammus2 = tyyppi->ammus2;
		return 1;
	}

	return 0;
}
int SpriteClass::AI_Change_When_Energy_Over_1(PrototypeClass *muutos){

	if (energia > 1 && muutos != tyyppi) {
		tyyppi = muutos;
		initial_weight = tyyppi->weight;
		//ammus1 = tyyppi->ammus1;
		//ammus2 = tyyppi->ammus2;
		return 1;
	}

	return 0;
}
int SpriteClass::AI_Muutos_Ajastin(PrototypeClass *muutos){
	if (energia > 0 && muutos != tyyppi)
	{
		if (mutation_timer/*charging_timer*/ == 0)
			mutation_timer/*charging_timer*/ = tyyppi->charge_time;

		if (mutation_timer/*charging_timer*/ == 1)
		{
			tyyppi = muutos;
			initial_weight = tyyppi->weight;

			ammus1 = tyyppi->ammus1;
			ammus2 = tyyppi->ammus2;

			animation_index = -1;

			Animaatio(ANIMATION_IDLE,true);
		}
		return 1;
	}

	return 0;
}
int SpriteClass::AI_Muutos_Jos_Osuttu(PrototypeClass *muutos){
	if (energia > 0 && muutos != tyyppi)
	{
		if (saatu_vahinko > 0)
		{
			tyyppi = muutos;
			initial_weight = tyyppi->weight;

			ammus1 = tyyppi->ammus1;
			ammus2 = tyyppi->ammus2;

			animation_index = -1;

			Animaatio(ANIMATION_IDLE,true);

			return 1;
		}
	}

	return 0;
}
int SpriteClass::AI_Tuhoutuu_Jos_Emo_Tuhoutuu(){
	if (emosprite != nullptr)
	{
		if (emosprite->energia < 1 && energia > 0)
		{
			saatu_vahinko = energia;
			saatu_vahinko_tyyppi = DAMAGE_ALL;

			return 1;
		}
	}

	return 0;
}
int SpriteClass::AI_Attack_1_Jos_Osuttu(){
	if (saatu_vahinko > 0 && energia > 0)
	{
		this->attack1_timer = this->tyyppi->attack1_time;
		this->charging_timer = 0;
		return 1;
	}

	return 0;
}
int SpriteClass::AI_Attack_2_Jos_Osuttu(){
	if (saatu_vahinko > 0 && energia > 0)
	{
		this->attack2_timer = this->tyyppi->attack2_time;
		this->charging_timer = 0;
		return 1;
	}

	return 0;
}
int SpriteClass::AI_Attack_1_Nonstop(){
	if (this->charging_timer == 0 && energia > 0)
	{
		this->attack1_timer = this->tyyppi->attack1_time;
		return 1;
	}

	return 0;
}
int SpriteClass::AI_Attack_2_Nonstop(){
	if (this->charging_timer == 0 && energia > 0)
	{
		this->attack2_timer = this->tyyppi->attack2_time;
		return 1;
	}

	return 0;
}
int SpriteClass::AI_Attack_1_if_Player_in_Front(SpriteClass &pelaaja){
	if (energia > 0 && damage_timer == 0 && pelaaja.energia > 0)
	{
		if ((pelaaja.x - x < 200 && pelaaja.x - x > -200) &&
			(pelaaja.y - y < tyyppi->korkeus && pelaaja.y - y > -tyyppi->korkeus))
		{
			if ((pelaaja.x < x && flip_x) || (pelaaja.x > x && !flip_x))
			{
				this->attack1_timer = this->tyyppi->attack1_time;
				return 1;
			}
		}
	}
	return 0;
}
int SpriteClass::AI_Attack_2_if_Player_in_Front(SpriteClass &pelaaja){
	if (energia > 0 && damage_timer == 0 && pelaaja.energia > 0)
	{
		if ((pelaaja.x - x < 200 && pelaaja.x - x > -200) &&
			(pelaaja.y - y < tyyppi->korkeus && pelaaja.y - y > -tyyppi->korkeus))
		{
			if ((pelaaja.x < x && flip_x) || (pelaaja.x > x && !flip_x))
			{
				this->attack2_timer = this->tyyppi->attack2_time;
				return 1;
			}
		}
	}
	return 0;
}
int SpriteClass::AI_Attack_1_if_Player_Bellow(SpriteClass &pelaaja){
	if (energia > 0 && damage_timer == 0 && pelaaja.energia > 0)
	{
		if ((pelaaja.x - x < tyyppi->leveys && pelaaja.x - x > -tyyppi->leveys) &&
			(pelaaja.y > y && pelaaja.y - y < 350))
		{
			this->attack1_timer = this->tyyppi->attack2_time;
			return 1;
		}
	}
	return 0;
}
int SpriteClass::AI_Hyppy_Jos_Pelaaja_Ylapuolella(SpriteClass &pelaaja){
	if (energia > 0 && jump_timer == 0 && pelaaja.energia > 0)
	{
		if ((pelaaja.x - x < tyyppi->leveys && pelaaja.x - x > -tyyppi->leveys) &&
			(pelaaja.y < y && y - pelaaja.y < 350))
		{
			jump_timer = 1;
			return 1;
		}
	}
	return 0;
}
int SpriteClass::AI_NonStop(){
	if (energia > 0)
	{

		double max = tyyppi->max_nopeus / 3.5;

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
int SpriteClass::AI_Kitka_Vaikuttaa(){

	if (energia > 0)
	{
		if (!alas)
			a /= 1.07;
		else
			a /= 1.02;
	}

	return 0;
}
int SpriteClass::AI_Piiloutuu(){

	if (energia > 0 && piilossa)
	{
		a /= 1.02;
		crouched = true;
	}

	return 0;
}
int SpriteClass::AI_Palaa_Alkuun_X(){

	if (energia < 1 || seen_player_x !=  -1)
		return 0;

	double max = tyyppi->max_nopeus / 3.5;

	if (x < alku_x-16 && a < max)
		a += 0.05;

	if (x > alku_x+16 && a > -max)
		a -= 0.05;

	return 0;
}
int SpriteClass::AI_Palaa_Alkuun_Y(){

	if (energia > 0 && seen_player_x == -1)
	{
		double max = tyyppi->max_nopeus / 3.5;

		if (y < alku_y-16 && b < max)
			b += 0.04;

		if (y > alku_y+16 && b > -max)
			b -= 0.04;
	}

	return 0;
}
int SpriteClass::AI_Tippuu_Tarinasta(int tarina){

	if (energia > 0 && tarina > 0)
	{
		initial_weight = 0.5;
	}

	return 0;
}
int SpriteClass::AI_Damaged_by_Water(){
	if (energia > 0)
		if (this->vedessa)
			saatu_vahinko++;

	return 0;
}
int SpriteClass::AI_Kill_Everyone(){
	if (energia > 0)
		this->vihollinen = !this->vihollinen;

	return 0;
}
int SpriteClass::AI_Jumper(){
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
		if (!alas && b==0 && jump_timer == 0)
		{
			jump_timer = 1;
		}
	}

	if (a < 0)
		flip_x = true;

	if (a > 0)
		flip_x = false;

	return 0;
}
int SpriteClass::AI_Liikkuu_X(double liike){
	if (energia > 0)
		this->x = this->alku_x + liike;

	return 0;
}
int SpriteClass::AI_Liikkuu_Y(double liike){
	if (energia > 0)
		this->y = this->alku_y + liike;

	return 0;
}
int SpriteClass::AI_Tippuu_Jos_Kytkin_Painettu(int kytkin){
	if (kytkin > 0)
	{
		initial_weight = 1.5;
	}

	return 0;
}
int SpriteClass::AI_Liikkuu_Jos_Kytkin_Painettu(int kytkin, int ak, int bk){
	if (kytkin > 0)
	{
		if (a == 0 && ak != 0)
		{
			a = tyyppi->max_nopeus / 3.5 * ak; // ak = -1 / 1
		}

		if (b == 0 && bk != 0)
			b = tyyppi->max_nopeus / 3.5 * bk; // bk = -1 / 1
	}

	flip_x = false;

	return 0;
}
bool SpriteClass::AI_Info(SpriteClass &pelaaja){
	if ((pelaaja.x - x < 10 && pelaaja.x - x > -10) &&
		(pelaaja.y - y < tyyppi->korkeus && pelaaja.y - y > -tyyppi->korkeus))
	{
		return true;
	}

	return false;
}
int SpriteClass::AI_Kana(){
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

		if (rand()%150 == 10 && b == 0 && jump_timer == 0 && ylos)
		{
			jump_timer = 1;
			while (a == 0)
				a = rand()%2 - rand()%2;
		}

		if (rand()%20 == 1 && b == 0 && jump_timer == 0 && !oikealle && !flip_x)
		{
			jump_timer = 1;
			while (a == 0)
				a = rand()%2;
		}

		if (rand()%20 == 1 && b == 0 && jump_timer == 0 && !vasemmalle && flip_x)
		{
			jump_timer = 1;
			while (a == 0)
				a = rand()%2 * -1;
		}

		if (rand()%200 == 10)
			a = rand()%2 - rand()%2;

		if (jump_timer == tyyppi->max_hyppy && a == 0)
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
int SpriteClass::AI_Bonus(){
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
int SpriteClass::AI_Egg(){
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

	if (energia == 0 && charging_timer == 0)
		charging_timer = tyyppi->charge_time;

	if (this->charging_timer == 1)
		this->piilota = true;

	return 0;
}
int SpriteClass::AI_Egg2(){
	if (x < 10)
	{
		x = 10;
	}

	if (y > 9920)
	{
		y = 9920;
	}

	if (!alas)
		saatu_vahinko = tyyppi->energia;

	//a /= 1.01;

	if (energia == 0 && charging_timer == 0)
		charging_timer = tyyppi->charge_time;

	if (charging_timer == 1)
		piilota = true;

	return 0;
}
int SpriteClass::AI_Ammus(){
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

	if (this->charging_timer == 0)
		charging_timer = this->tyyppi->charge_time;

	if (this->charging_timer == 1)
	{
		this->saatu_vahinko = this->tyyppi->energia;
		this->saatu_vahinko_tyyppi = DAMAGE_ALL;
	}

	if (energia < 1)
		piilota = true;

	return 0;
}
int SpriteClass::AI_Pommi(){
	if (this->charging_timer == 0)
		charging_timer = this->tyyppi->charge_time;

	if (this->charging_timer == 1)
	{
		this->saatu_vahinko = this->energia;
		this->saatu_vahinko_tyyppi = DAMAGE_ALL;
	}

	return 0;
}
int SpriteClass::AI_Teleportti(std::list<SpriteClass*> spritet, SpriteClass &player){
	int siirto = 0;

	if (energia > 0 && charging_timer == 0 && attack1_timer == 0)
	{
		if (player.x <= x + tyyppi->leveys /2 && player.x >= x - tyyppi->leveys /2 &&
			player.y <= y + tyyppi->korkeus/2 && player.y >= y - tyyppi->korkeus/2 )
		{

			std::vector<SpriteClass*> portit;

			// search for teleports of the same type
			for (SpriteClass* sprite : spritet)
				if (tyyppi == sprite->tyyppi && sprite != this)
						portit.push_back(sprite);

			// if it didn't find any, search for all teleports
			if (portit.size() == 0) {
				for (SpriteClass* sprite : spritet)
					if (sprite->tyyppi->tyyppi == TYPE_TELEPORT && sprite != this)
						portit.push_back(sprite);
			}

			// if you don't have any teleports (excluding the teleport itself), return
			if (portit.size() == 0)
				return 0;

			// arvotaan kohdeportti
			SpriteClass* dst = portit[rand()%portit.size()];
			
			player.x = dst->x;
			player.y = dst->y;
			//charging_timer    = tyyppi->charge_time;
			//attack1_timer = tyyppi->attack1_time;
			//spritet[i].charging_timer    = spritet[i].tyyppi->charge_time;
			dst->attack1_timer = dst->tyyppi->attack1_time;
			dst->charging_timer = 0;
			this->charging_timer = 0;

			siirto = 1;
		}
	}

	return siirto;
}

int SpriteClass::Animation_Perus(){

	int uusi_animaatio = -1;
	bool alusta = false;

	if (energia < 1 && !alas)
	{
		uusi_animaatio = ANIMATION_DEATH;
		alusta = true;
	}
	else
	{

		if (a > -0.2 && a < 0.2 && b == 0 && jump_timer <= 0)
		{
			uusi_animaatio = ANIMATION_IDLE;
			alusta = true;
		}

		if ((a < -0.2 || a > 0.2) && jump_timer <= 0)
		{
			uusi_animaatio = ANIMATION_WALKING;
			alusta = false;
		}

		if (b < 0)//-0.3
		{
			uusi_animaatio = ANIMATION_JUMP_UP;
			alusta = false;
		}

		if ((jump_timer > tyyppi->max_hyppy || b > 1.5) && alas)
		{
			uusi_animaatio = ANIMATION_HYPPY_DOWN;
			alusta = false;
		}

		if (crouched)
		{
			uusi_animaatio = ANIMATION_SQUAT;
			alusta = true;
		}

		if (attack1_timer > 0)
		{
			uusi_animaatio = ANIMATION_ATTACK1;
			alusta = true;
		}

		if (attack2_timer > 0)
		{
			uusi_animaatio = ANIMATION_ATTACK2;
			alusta = true;
		}

		if (damage_timer > 0)
		{
			uusi_animaatio = ANIMATION_DAMAGE;
			alusta = false;
		}

	}

	if (uusi_animaatio != -1)
		Animaatio(uusi_animaatio,alusta);

	return 0;
}
int SpriteClass::Animation_Kana(){

	int uusi_animaatio = -1;
	bool alusta = false;

	if (energia < 1 && !alas) {
	
		uusi_animaatio = ANIMATION_DEATH;
		alusta = true;
	
	} else {

		if (a > -0.2 && a < 0.2 && b == 0 && jump_timer <= 0) {

			uusi_animaatio = ANIMATION_IDLE;
			alusta = true;
		
		}

		if ((a < -0.2 || a > 0.2) && jump_timer <= 0) {

			uusi_animaatio = ANIMATION_WALKING;
			alusta = false;
		
		}

		if (b < 0) {

			uusi_animaatio = ANIMATION_JUMP_UP;
			alusta = false;

		}

		if ((jump_timer > 90+10/*tyyppi->max_hyppy || b > 1.5*/) && alas) {
		
			uusi_animaatio = ANIMATION_HYPPY_DOWN;
			alusta = false;
		
		}

		if (attack1_timer > 0) {

			uusi_animaatio = ANIMATION_ATTACK1;
			alusta = true;

		}

		if (attack2_timer > 0) {

			uusi_animaatio = ANIMATION_ATTACK2;
			alusta = true;

		}

		if (crouched) {

			uusi_animaatio = ANIMATION_SQUAT;
			alusta = true;

		}

		if (damage_timer > 0) {

			uusi_animaatio = ANIMATION_DAMAGE;
			alusta = false;

		}

	}

	if (uusi_animaatio != -1)
		Animaatio(uusi_animaatio,alusta);

	return 0;
}
int SpriteClass::Animation_Bonus() {

	Animaatio(ANIMATION_IDLE, true);
	return 0;

}
int SpriteClass::Animation_Ammus() {

	Animaatio(ANIMATION_IDLE, true);
	return 0;

}

int SpriteClass::Animation_Egg() {

	int uusi_animaatio = ANIMATION_IDLE;
	bool alusta = true;

	if (energia < tyyppi->energia)
		uusi_animaatio = ANIMATION_DEATH;
	
	Animaatio(uusi_animaatio, alusta);

	return 0;

}
