//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

class SpriteClass;


namespace AI_Functions{

void Rooster(SpriteClass*s);
void BlueFrog(SpriteClass*s);
void RedFrog(SpriteClass*s);
void Egg(SpriteClass*s);
void Egg2(SpriteClass*s);
void Projectile(SpriteClass*s);
void Jumper(SpriteClass*s);
void Basic(SpriteClass*s);

void NonStop(SpriteClass*s);
void NonStopVertical(SpriteClass*s);

void NonStop2(SpriteClass*s);

void Turning_Horizontally(SpriteClass*s);
void Turning_Vertically(SpriteClass*s);
void Look_For_Cliffs(SpriteClass*s);
void Random_Change_Dir_H(SpriteClass*s);
void Random_Turning(SpriteClass*s);
void Random_Jump(SpriteClass*s);

void Follow_Player(SpriteClass*s);
void Follow_Player_If_Seen(SpriteClass*s);
void Follow_Player_Vert_Hori(SpriteClass*s);
void Follow_Player_If_Seen_Vert_Hori(SpriteClass*s);
void Follow_Player_Diagonally(SpriteClass*s);

void Look_at_Player(SpriteClass* s);


void Run_Away_From_Player(SpriteClass*s);
void Run_Away_From_Supermode_Player(SpriteClass*s);

void SelfDestruction(SpriteClass*s);
void Attack_1_Nonstop(SpriteClass*s);
void Attack_2_Nonstop(SpriteClass*s);
void Attack_1_if_Player_in_Front(SpriteClass*s);
void Attack_2_if_Player_in_Front(SpriteClass*s);
void Attack_1_if_Player_Below(SpriteClass*s);
void Attack_1_If_Player_Above(SpriteClass*s);
void Attack_2_If_Player_Above(SpriteClass*s);

void Transform_If_Player_Above(SpriteClass*s);
void Transform_If_Player_Below(SpriteClass*s);
void Transform_If_Player_in_Front(SpriteClass* s);


void Attack_1_if_Player_Nearby(SpriteClass*s);
void Attack_2_if_Player_Nearby(SpriteClass*s);

void Transform_if_Player_Nearby(SpriteClass*s);
void Die_if_Player_Nearby(SpriteClass*s);
void Transform_if_Player_Away(SpriteClass*s);
void Die_if_Player_Away(SpriteClass*s);


void Jump_If_Player_Above(SpriteClass*s);
void Jump_If_Player_in_Front(SpriteClass*s);

void Damaged_by_Water(SpriteClass*s);
void Friction_Effect(SpriteClass*s);
void Hiding(SpriteClass*s);
void Return_To_Orig_X(SpriteClass*s);
void Return_To_Orig_Y(SpriteClass*s);
void Transform_When_Energy_Under_2(SpriteClass* s);
void Transform_When_Energy_Over_1(SpriteClass* s);

void Self_Transformation(SpriteClass* s);
void Self_Transformation_Random_Prototype(SpriteClass * s);

void Random_Move_Vert_Hori(SpriteClass* s);


void Destructed_Next_To_Player(SpriteClass* s);
void Follow_Commands(SpriteClass* s);
void Climber(SpriteClass*s);
void Climber2(SpriteClass*s);
void Fall_When_Shaken(SpriteClass*s);

void Move_If_Switch_Pressed(SpriteClass*s, int game_button, int ak, int bk);
void Teleporter(SpriteClass*s);
void Turn_Back_If_Damaged(SpriteClass* s);

void Return_To_Orig_X_Constant(SpriteClass*s);
void Return_To_Orig_Y_Constant(SpriteClass*s);


void SwimInWater(SpriteClass*s);
void SwimInWaterMaxSpeed(SpriteClass*s);

void UncontrollablePlayer(SpriteClass* s);
void UncontrollablePlayer2(SpriteClass* s);


void MaxSpeedPlayer(SpriteClass*s);
void MaxSpeedOnSuper(SpriteClass*s);
void NpcCollectBonuses(SpriteClass*s);


void DieIfTouchesWall(SpriteClass*s);
void CannotBePushed(SpriteClass*s);


void TransformIfTouchesFloor(SpriteClass*s);
void TransformIfTouchesWall(SpriteClass*s);


void Follow_Enemy_X(SpriteClass*s);
void Follow_Enemy_Y(SpriteClass*s);
void Follow_Enemy_Diagonally(SpriteClass*s);

/**
 * @brief 
 * AIs triggered on death
 */

void EvilOne(SpriteClass*s);
void Chick(SpriteClass*s);
void Reborn(SpriteClass*s);

/**
 * @brief 
 * AIs triggered on damage
 */
void Attack_1_If_Damaged(SpriteClass*s);
void Attack_2_If_Damaged(SpriteClass*s);
//void Transform_If_Damaged(SpriteClass* s);

/**
 * @brief 
 * AIs triggered on game start
 */

void RandomStartDirection(SpriteClass*s);
void RandomStartDirectionVert(SpriteClass*s);
void StartFacingThePlayer(SpriteClass*s);
void StartFacingThePlayerVert(SpriteClass*s);

void StartDown(SpriteClass*s);
void StartUp(SpriteClass*s);
void StartLeft(SpriteClass*s);
void StartRight(SpriteClass*s);

/**
 * @brief 
 * AIs triggered when skull changed
 * 
 */

void DieIfSkullBlocksChanged(SpriteClass*s);
void InfiniteEnergy(SpriteClass*s);
//void TransformIfSkullBlocksChanged(SpriteClass*s);

/**
 * @brief 
 * AIs for projectiles
 */

void ThrowableWeapon(SpriteClass*sprite, SpriteClass*shooter);
void ThrowableWeapon2(SpriteClass*sprite, SpriteClass*shooter);
void ProjectileEgg(SpriteClass*sprite, SpriteClass*shooter);
void StaticProjectile(SpriteClass*sprite, SpriteClass*shooter);

void ProjectileAimRandomly(SpriteClass*sprite, SpriteClass*shooter);
void ProjectileAimAtPlayer(SpriteClass*sprite, SpriteClass*shooter);

/**
 * @brief 
 * Info AIs
 */

void DisplayInfo(SpriteClass* sprite);
void DisplayInfoIfTouchesPlayer(SpriteClass*sprite);

void DisplayMissingSpriteWarning(SpriteClass* sprite);
void DisplayMissingTextureWarning(SpriteClass* sprite);

}
