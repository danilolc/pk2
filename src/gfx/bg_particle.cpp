//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "bg_particle.hpp"
#include "system.hpp"
#include "engine/PDraw.hpp"
#include "game/game.hpp"


static void DrawWaterdrop(const BgParticle& p, int cam_x, int cam_y) {
	
    int kx = int(p.x)-cam_x;
	int ky = int(p.y)-cam_y;
	PDraw::screen_fill(kx,ky,kx+1,ky+4,p.color+(u8)p.b);
}

static void DrawLeaf1(const BgParticle& p, int cam_x, int cam_y) {

	int kx = int(p.x)-cam_x,
		ky = int(p.y)-cam_y;

	PDraw::screen_fill(kx,ky,kx+2,ky+2,96+6+(int)p.b+int(p.x+p.y)%10);
}

static void DrawLeaf2(const BgParticle& p, int cam_x, int cam_y){

	int kx = int(p.x)-cam_x,
		ky = int(p.y)-cam_y,
		frame = (int(p.y/10)%4)*23;

	PDraw::image_cutclip(Game->gfxTexture,kx,ky,1+frame,141,21+frame,152);

}

static void DrawLeaf3(const BgParticle& p, int cam_x, int cam_y) {

	int kx = int(p.x)-cam_x,
		ky = int(p.y)-cam_y,
		frame = (int(p.y/5)%4)*20;

	PDraw::image_cutclip(Game->gfxTexture,kx,ky,93+frame,141,109+frame,150);

}

static void DrawLeaf4(const BgParticle& p, int cam_x, int cam_y) {

	int kx = int(p.x)-cam_x,
		ky = int(p.y)-cam_y,
		frame = (int(p.y/5)%2)*14;

	PDraw::image_cutclip(Game->gfxTexture,kx,ky,173+frame,141,183+frame,150);
}

static void DrawFlake1(const BgParticle& p, int cam_x, int cam_y) {

	int kx = int(p.x)-cam_x,
		ky = int(p.y)-cam_y;

	PDraw::image_cutclip(Game->gfxTexture,kx,ky,1,155,8,162);
}

static void DrawFlake2(const BgParticle& p, int cam_x, int cam_y) {

	int kx = int(p.x)-cam_x,
		ky = int(p.y)-cam_y;

	PDraw::image_cutclip(Game->gfxTexture,kx,ky,11,155,16,160);

}

static void DrawFlake3(const BgParticle& p, int cam_x, int cam_y) {

	int kx = int(p.x)-cam_x,
		ky = int(p.y)-cam_y;

	PDraw::image_cutclip(Game->gfxTexture,kx,ky,19,155,22,158);

}

static void DrawFlake4(const BgParticle& p, int cam_x, int cam_y) {

	int kx = int(p.x)-cam_x,
		ky = int(p.y)-cam_y;

	PDraw::screen_fill(kx,ky,kx+2,ky+2,20+(int)p.b);
}

/**
 * @brief 
 * New GE particles
 */

static void DrawDandelion1(const BgParticle& p, int cam_x, int cam_y){
    int kx = int(p.x)-cam_x,
		ky = int(p.y)-cam_y,
		frame = (int(p.y/10)%4)*12;

	PDraw::image_cutclip(Game->gfxTexture,kx,ky,
        1+frame,165, 10+frame, 176);
}

static void DrawDandelion2(const BgParticle& p, int cam_x, int cam_y){
    
    int kx = int(p.x)-cam_x,
		ky = int(p.y)-cam_y,
		frame = (int(p.y/5)%4)*12;

	PDraw::image_cutclip(Game->gfxTexture,kx,ky,
        49+frame,165,58+frame,175);    
}

static void DrawDandelion3(const BgParticle& p, int cam_x, int cam_y){
    
    int kx = int(p.x)-cam_x,
		ky = int(p.y)-cam_y,
		frame = (int(p.y/5)%2)*9;

	PDraw::image_cutclip(Game->gfxTexture,kx,ky,
        97+frame,165,103+frame,171);    
}

static void UpdateWaterdrop(BgParticle& p){
    p.y += p.b / 3.0 + 2.0;
}

static void UpdateLeaf1(BgParticle& p){
    p.x += p.a / 9.0;
	p.y += p.b / 9.0;
}

static void UpdateLeaf2(BgParticle& p) {
	p.x += p.a / 14.0;
	p.y += p.b / 9.0;
}

static void UpdateLeaf3(BgParticle& p) {

	p.x += p.a / 12.0;
	p.y += p.b / 9.0;

}

static void UpdateLeaf4(BgParticle& p) {
	p.x += p.a / 11.0;
	p.y += p.b / 9.0;
}


static void UpdateFlake1(BgParticle& p) {

	p.x += sin_table(p.y)/50.0;
	p.y += p.b / 7.0;

}

static void UpdateFlake2(BgParticle& p) {

	p.x += sin_table(p.y)/100.0;
	p.y += p.b / 8.0;

}

static void UpdateFlake3(BgParticle& p) {

	p.y += p.b / 8.0;

}

static void UpdateFlake4(BgParticle& p) {
	p.y += p.b / 9.0;
}

static void UpdateDandelion(BgParticle& p){
    p.x += p.a + sin_table(p.y)/100.0;
	p.y += p.b + sin_table(p.x)/100.0;
}


void BgParticle::checkScreenBorders(int cam_x, int cam_y){
    if ( x  >  cam_x + screen_width )
		x  =  cam_x + int(x - cam_x + screen_width) % screen_width;
	if ( x  <  cam_x )
		x  =  cam_x + screen_width - int(cam_x - x) % screen_width;
	if ( y  >  cam_y + screen_height )
		y  =  cam_y + int(y - cam_y + screen_height) % screen_height;
	if ( y  <  cam_y )
		y  =  cam_y + screen_height - int(cam_y - y) % screen_height;
}

void BgParticle::setType(int type){
    this->mType = type;

    switch (type)
    {
    case BGPARTICLE_NOTHING:
        this->mDraw = nullptr;
        this->mUpdate = nullptr;
        break;
    
    case BGPARTICLE_WATERDROP:
        this->mDraw = DrawWaterdrop;
        this->mUpdate = UpdateWaterdrop;
        break;

    case BGPARTICLE_LEAF1:
        this->mDraw = DrawLeaf1;
        this->mUpdate = UpdateLeaf1;
        break;
    
    case BGPARTICLE_LEAF2:
        this->mDraw = DrawLeaf2;
        this->mUpdate = UpdateLeaf2;
        break;

    case BGPARTICLE_LEAF3:
        this->mDraw = DrawLeaf3;
        this->mUpdate = UpdateLeaf3;
        break;

    case BGPARTICLE_LEAF4:
        this->mDraw = DrawLeaf4;
        this->mUpdate = UpdateLeaf4;
        break;

    case BGPARTICLE_FLAKE1:
        this->mDraw = DrawFlake1;
        this->mUpdate = UpdateFlake1;
        break;

    case BGPARTICLE_FLAKE2:
        this->mDraw = DrawFlake2;
        this->mUpdate = UpdateFlake2;
        break;

    case BGPARTICLE_FLAKE3:
        this->mDraw = DrawFlake3;
        this->mUpdate = UpdateFlake3;
        break;

    case BGPARTICLE_FLAKE4:
        this->mDraw = DrawFlake4;
        this->mUpdate = UpdateFlake4;
        break;


    /**
     * @brief 
     * New GE particles
     */

    case BGPARTICLE_DANDELION1:
        this->mDraw = DrawDandelion1;
        this->mUpdate = UpdateDandelion;
        break;

    case BGPARTICLE_DANDELION2:
        this->mDraw = DrawDandelion2;
        this->mUpdate = UpdateDandelion;
        break;

    case BGPARTICLE_DANDELION3:
        this->mDraw = DrawDandelion3;
        this->mUpdate = UpdateDandelion;
        break;


    default:
        break;
    }
}

