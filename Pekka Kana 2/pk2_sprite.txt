SHORT PEKKA KANA 2 SPRITE 1.3 DOCUMENTATION
-------------------------------------------------------------------------------------------------
This is a dummy version of the actual PK2Sprite_Prototype class. It's
used to save sprites in .spr files and to load them.

class PK2Sprite_Prototype13 {
public:

	/* Sprite's Type */
	
	int	type;				// Sprite's type. Check Sprite Type List below.

	/* Bitmap File */

	char		image_file[100];	// .BMP or PCX file containing sprite's graphics

	/* Sound Effects */

	char		sound_files[7][100];	// sound effects
						// Index:	Explanation:
						// 0 = 		Damage Sound
						// 1 = 		Knock Out Sound
						// 2 = 		Attack 1 Sound
						// 3 = 		Attack 2 Sound
						// 4 = 		Random Sound
						// 5 = 		Special Sound 1 (not implemented)
						// 6 = 		Special Sound 2 (not implemented)

	int		sounds[7];		// not used here 

	/* Animation */

	unsigned char	frames;			// Number of frames

	PK2SPRITE_ANIMATION animations[20];	// Animation sequensses
						// Index:	Explanation:
						// 0 =		Still
						// 1 =		Walking
						// 2 =		Jump up
						// 3 =		Jump Down
						// 4 =		Duck
						// 5 =		Damage
						// 6 =		Knock Out
						// 7 =		Attack 1
						// 8 =		Attack 2

	unsigned char	animations;		// Number of animations

	unsigned char	frame_rate;		// Durations of one animation frame (in game frames)

	int		position_x;		// X cordinate of first frame in bitmap (left corner)
	int		position_y;		// Y cordinate of first frame in bitmap (top corner)

	int		frame_width;		// One frame's width
	int		frame_height;		// One frame's height
	int		frame_distance;		// Distance of two frame's in bitmap (in pixels).
						// Not actually used. 

	/* Sprite information */

	char		name[30];		// Sprite's name (visible in level editor)

	int		width;			// Sprite's width 
	int		height;			// Sprite's height
						// NOTE: Height and width do not have to mach
						// frame width and height. 

	double		weight;			// Sprite's weight 
						// NOTE: Sprite's weight affects jumping and switches.
						// Sprites that weight more than 1 can press switches.
						// Weight also increases damage when sprite falls on
						// another (hostile) sprite.
						// A sprite with negative widt wiil rise up (fast)

	bool		enemy;			// True if sprite is an enemy. Enemys do damage to
						// non enemy sprites and vice versa.

	int		energy;			// Sprite's energy (both maximum and default).
						// If the sprite is a bonus item, energy is the
						// amount of energy this bonus returns (e.g. feather).

	int		damage;			// How much damage sprite causes (positive decreases 
						// and negative decreases).

	unsigned char  	damage_type;		// Type of sprite's damage (check Damage Type list).		
	unsigned char	immunity;		// Type of damage that does not harm the sprite 
						// (check Damage Type list).

	int		score;			// If the sprite is a game character, then score is the 
						// number of points you get when you knock it out.
						// If sprite is a bonus item sprite, then score is
						// the amount of points you get when you collect the
						// bonus item.

	int		AI[10];			// Table of sprite's AI modules. Check the AI List.

	unsigned char	max_jump;		// Duration of sprite's jump. Normal jump around 20-25.

	double		max_speed;		// Sprite's maximum speed: 0=still 4=normal speed

	int		loading_time;		// Wait after shooting (in frames).

	unsigned char	color;			// Sprite's color (see Color List).

	bool		obstacle;		// True if sprite is a wall.

	int		destruction;		// Knock out effect. 
						// See Destruction Style list.

	bool		key;			// true if sprite is a key or can open locks

	bool		shakes;			// true if sprite shakes randomly

	unsigned char   bonuses;		// number of bonuses the sprite drops when ko'ed	

	int         	attack1_duration;	// how long the attack 1 animation is displayed (frames)
	int         	attack2_duration;	// how long the attack 1 animation is displayed (frames)	

	int		parallax_factor;	// Relation to background image (Background sprite 							// types).
	/* Linked Sprites */

	char		transformation_sprite[100];	// Another sprite to which this sprite can
							// transform into (e.g. 'r_naked.spr')
							// If the sprite is a bonus, the player will
							// turn to this sprite when the bonus is
							// collected.
	char		bonus_sprite[100];		// A sprite this sprite drops when ko'ed
							// (e.g. 'apple.spr')
							// If the sprite itself is a bonus, 
							// 'bonus_sprite' will appear in the item box.
							 
	char		ammo1_sprite[100];		// The sprite this sprite launches in attack 1
	char		ammo2_sprite[100];		// The sprite this sprite launches in attack 2
	
	bool		tile_check;			// True if sprite can't walk/fly through walls. 

	unsigned long	sound_frq;			// Default frequency of sound effects 
							// (default 22050)
	bool		random_frq;			// true = Play sound effects in random frequency
							// related to default frequency
	
	// If sprite is an obstacle
	bool		wall_up;			// Sprite works as a wall upwards
	bool		wall_down;			// Sprite works as a wall downwards
	bool		wall_right;			// Sprite works as a wall ...
	bool		wall_left;			// Sprite works as a wall ...

	unsigned char	transparency;		// not implemented
	bool		glow;			// not implemented					

	int		attack_pause;		// If the sprite is an ammunition, this is the loading
						// time between two attacks.

	bool		glide_ability;		// true if sprite can glide down			
	bool		boss;			// true if sprite is a boss. If sprite is a boss,
						// skull switches will rotate after the sprite is
						// ko'ed.

	bool		bonus_always;		// always drops bonus(es) when ko'ed

	bool		can_swim;		// if true, gravity doesn't affect in water
};

	Sprite Type List
	================
	1	= Character Sprite
	2	= Bonus Item Sprite
	3	= Ammo Sprite
	4	= Teleport Sprite
	5	= Background Sprite



	AI Module List
	==============
	(B) = AI affects Bonus Sprites
	(P) = AI affects also when sprite is controlled by player

	No AI					= 0	Marks the last AI.
	Chicken					= 1	Sprite jumps randomly and tries to get over
							obstacles.
	An Egg					= 2	Sprite dies, if it hits the floor.
	Baby Chicken				= 3	Same as Chicken, but it tries to follow the 
							player.
	Bonus	(B)				= 4	
	Jumper					= 5
	Basic AI (B)				= 6	Sprite won't get off the map and looks at the 	
							same direction it's walking.
	Turn horizontally			= 7	Sprite turns horizontally if it hits an 
							obstacle.
	Looks Out for Cliffs			= 8	Sprite turns horizontally before it falls down
							from a cliff.
	Random Horizontal Change of Direction	= 9	Sprite turns horizontally at random points.
	Random Jumping				= 10	Sprite jumps randomly.
	Follow the Player			= 11	Sprite follows the player even if it doesn't 
							see it.
	Random Horizontal Change of Direction	= 12	Sprite turns horizontally at random points.
	Follow the Player if Player is in Front	= 13	Sprite follows the player only if it  
							sees it.
	Transformation If Energy Below 2 (P)	= 14	Sprite changes to Transformation Sprite when
							it's energy drops below 2.
	Transformation If Energy Below 2 (P)	= 15	Sprite changes to Transformation Sprite when
							it's energy rises over 1.
	Start Directions Towards Player		= 16
	Ammonition				= 17
	Non Stop				= 18	Sprite is not affected by friction.
	Attack 1 If Damaged			= 19
	Self Destruction			= 20
	Attack 1 If Player is in Front		= 21
	Attack 1 If Pleyer is Below		= 22
	Damaged by Water (P)			= 23		
	Attack 2 If Player is in Front		= 24
	Kill 'em all				= 25	Sprite damages both enemies and friends.
	Affected by Friction			= 26	Friction slows the sprite down and stops it.
	Hide					= 27	Sprite ducks if it's behind tile 144.
	Return to Start Position X		= 28	After nothing better to do the sprite always
							tries to return to it's start position.
	Return to Start Position Y		= 29	After nothing better to do the sprite always
							tries to return to it's start position.
	Teleport				= 30
	Throwable Weapon			= 35
	Falls When Shaken (B)			= 36
	Change Trap Stones If KO'ed		= 37
	Change Trap Stones If Damaged		= 38
	Self Destructs With Mother Sprite	= 39
	Moves X COS			    	= 41
	Moves Y COS				= 42
	Moves X SIN			    	= 43
	Moves Y SIN			    	= 44
	Moves X COS Fast			= 45
	Moves Y SIN Fast			= 46
	Moves X COS Slow			= 47
	Moves Y SIN Slow			= 48
	Moves Y SIN Free			= 49
	Random Turning				= 50
	Jump If Player Is Above			= 51
	Transformation Timer (B)		= 52
	Falls If Switch 1 Is Pressed (B)	= 53
	Falls If Switch 2 Is Pressed (B)	= 54
	Falls If Switch 3 Is Pressed (B)	= 55
	Moves Down  If Switch 1 Is Pressed	= 56
	Moves Up    If Switch 1 Is Pressed	= 57
	Moves Right If Switch 1 Is Pressed	= 58
	Moves Left  If Switch 1 Is Pressed	= 59
	Moves Down  If Switch 2 Is Pressed	= 60
	Moves Up    If Switch 2 Is Pressed	= 61
	Moves Right If Switch 2 Is Pressed	= 62
	Moves Left  If Switch 2 Is Pressed	= 63
	Moves Down  If Switch 3 Is Pressed	= 64
	Moves Up    If Switch 3 Is Pressed	= 65
	Moves Right If Switch 3 Is Pressed	= 66
	Moves Left  If Switch 3 Is Pressed	= 67

THESE WORK ONLY IN PK2 VERSION 1.2 ->

	Turns Vertically From Obstacle		= 70	
	Random Vertical Starting Direction	= 71	
	Starting Direction Towards Player	= 72
	Climber					= 73	Sprite bounces from walls, floor and ceiling
	Climber Type 2				= 74	Sprite moves along walls (kind of)
	Runs Away From Player If Sees Player	= 75
	Reborn (B)				= 76	A new item appears when sprite is collected 

	Arrow Left				= 77	These set the direction of the sprite, if 
	Arrow Right				= 78	the sprite has AI 81.
	Arrow Up				= 79
	Arrow Down				= 80
	Move to Arrows Direction		= 81

<- THESE WORK ONLY IN PK2 VERSION 1.2


	Background Sprite Moon			= 101
	Background Sprite Moves to Left		= 102
	Background Sprite Moves to Right 	= 103

	Add Time to Clock			= 120	Add time from loading_time 
	Make Player Invisible			= 121   Set invicibility time from loading_time 
							Works only in PK2 version 1.2

	Follow the Player Vertic. and Horiz.	= 130	Sprite follows the player even if doesn't 
							see it.
	Follow the Player Vertic. and Horiz.
	if Player is in Front			= 131	Sprite follows the player only if it  
							sees it.
	Random Move Vertic. and Horiz.		= 132
	Frog Jump 1				= 133
	Frog Jump 2				= 134
	Frog Jump 3				= 135
	Attack 2 if Damaged			= 136
	Attack 1 Non Stop			= 137
	Attack 2 Non Stop			= 138
	Turn if Damaged				= 139
	Evil One				= 140

	Info 1		= 201 // use doodle attack to knock out enemys.
	Info 2		= 202 // use egg attack to knock out enemys.
	Info 3		= 203 // collect all keys to open all locks.
	Info 4		= 204 // you can activate switches by jumping on them.
	Info 5		= 205 // green switches activate green blocks.
	Info 6		= 206 // blue switches activate blue blocks.
	Info 7		= 207 // orange switches turn off fire.
	Info 8		= 208 // collect gifts to obtain useful items.
	Info 9		= 209 // if you are hit, feathers will give you energy.
	Info 10		= 210 // a drink from a magic bottle will change you...
	Info 11		= 211 // drink doodle drinks to and get special doodle attacks.
	Info 12		= 212 // drink hatching-booster and get special egg attacks.
	Info 13		= 213 // look for hidden bonuses!
	Info 14		= 214 // you can stomp on enemys, but you will lose energy.
	Info 15		= 215 // look out for traps!
	Info 16		= 216 // hurry up! your time is limited!
	Info 17		= 217 // be aware that some enemys resist certain attacks.
	Info 18		= 218 // a hen is mightier than the sword.
	Info 19		= 219 // why did the chicken cross the road?

	

	Color List
	==========
	0	= Grey
	32	= Blue
	64	= Red
	96	= Green
	128	= Orange
	160	= Violet
	192	= Turquoise
	255	= Original


	Damage Type List
	================
	0	= No damage
	1	= Punch
	2	= Fall (stomp)
	3	= Noise
	4	= Fire
	5	= Water
	6	= Snow
	7	= Bonus
	8	= Electricity (stuns a sprite for a few seconds)
	9	= Self destruction
	10	= Squeeze
	11	= Smell
	12	= All
	13	= Sting


	Destruction Style List
	======================
	Indestructible		= 0
	Animation		= 100
	Feathers		= 1	(feathers will fly also when damaged)
	Grey stars		= 2
	Blue stars		= 3
	Red stars		= 4
	Green stars		= 5
	Orange stars		= 6
	Violet stars		= 7
	Turquoise stars		= 8
	Grey explotion		= 9
	Blue explotion		= 10
	Red explotion		= 11
	Green explotion		= 12
	Orange explotion	= 13
	Violet explotion	= 14
	Turquoise explotion	= 15
	Grey smoke		= 16
	Blue smoke		= 17
	Red smoke		= 18
	Green smoke		= 19
	Orange smoke		= 20
	Violet smoke		= 21
	Turquoise smoke		= 22
	Smoke clouds		= 23

	Special effects can also be combined with animation.
	Example: Animation + Feathers



-------------------------------------------------------------------------------------------------
This is a help structure used to describe a sequence of frames. Maximum amount of frames is 10.

A sequence might look like this: 3,4,5,6,0,0,0,0,0,0 	(= Rooster sprite's walking animation)

First frame is 1, second is 2 etc. Zero marks the end of sequence if the sequence is shorter 
than 10 frames.

struct PK2SPRITE_ANIMATION {
	UCHAR		sequence[10];	// sequence of frames
	UCHAR		frames;		// number of frames
	bool		loops;		// true if animation loops
};

-------------------------------------------------------------------------------------------------
A method (or a sub program) for loading a version 1.3 sprite from an .SPR file.


int PK2Sprite_LoadVersion13(char *file_name) {

	ifstream *file = new ifstream(file_name, ios::binary | ios::nocreate);
	char version[4];
	
	if (file->fail()) {
		delete (file);
		return 1;
	}

	file->read ((char *)version, 4);

	if (strcmp(versio,"1.3") == 0) {
		PK2Sprite_Prototype13 sprite;
		file->read ((char *)&file, sizeof (sprite));
	}

	if (file->fail()) {
		delete (file);
		return 1;
	}
	
	delete (file);

	return 0;
}
-------------------------------------------------------------------------------------------------
A method (or a sub program) for saving a version 1.3 sprite to an .SPR file.

void PK2Sprite_SaveVersion13(char *file_name) {

	PK2Sprite_Prototype13 sprite;
	ofstream *file = new ofstream(file_name, ios::binary);
	file->write ("1.3", 4);
	file->write ((char *)&sprite, sizeof(sprite));
	delete (file);
}

-------------------------------------------------------------------------------------------------
