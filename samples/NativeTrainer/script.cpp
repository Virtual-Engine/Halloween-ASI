/*
This File Started Its Life As Part Of The GTA V SCRIPT HOOK SDK
http://dev-c.com
(C) Alexander Blade 2015

It is Now Part Of 'Halloween Mod Menu' By The Crusader
https://www.youtube.com/TheCrusaderModz

This Menu Is In Very Early Stages And Many New Options/Features Are Being Added. When Such Options Are Added, The Source Code Will Be Updated.

If You Use Any of this Code Please Credit Me, 'The Crusader' And Anyone Else Who Helps You In Anyway and Please Make It Open Source Too.

Please Dont Just Release A 'Carbon Copy' Of This Menu, But Learn From it.

Thanks. 

The Crusader

*/

/*

Controls:

Keyboard
Must Have Num Lock On
Open: F5
Select : Numpad 5
Scroll Up: Numpad 8
Scroll Down: Numpad 2
Back : Numpad 0

Gamepad

Open: R3 (RS) (Right Analog stick) + Dpad left
Up: Dpad Up (If you press it twice quickly it will bring up the phone, so navigate a bit slower)
Down: Dpad Down
Select: A
Back: B

Other Controls:

Vehicle Weapons: Numpad +

Speed Boost:

Foward: Numpad 9

Stop: Numpad 3
*/

#include "script.h"
#include "keyboard.h"

#include <string>
#include <ctime>

enum ControllerInputs
{

	INPUT_FRONTEND_X = 203, 
	INPUT_FRONTEND_Y = 204,
	INPUT_FRONTEND_LB = 205,
	INPUT_FRONTEND_RB = 206,
	INPUT_FRONTEND_LT = 207,
	INPUT_FRONTEND_RT = 208,
	INPUT_FRONTEND_LS = 209, 
	INPUT_FRONTEND_RS = 210,  
	INPUT_FRONTEND_DOWN = 187,
	INPUT_FRONTEND_UP = 188,
	INPUT_FRONTEND_LEFT = 189,
	INPUT_FRONTEND_RIGHT = 190,
	INPUT_FRONTEND_RDOWN = 191,
	INPUT_FRONTEND_RUP = 192,
	INPUT_FRONTEND_RLEFT = 193,
	INPUT_FRONTEND_RRIGHT = 194,
	INPUT_FRONTEND_ACCEPT = 201,
	INPUT_FRONTEND_CANCEL = 202,
	INPUT_PHONE = 27,
	INPUT_SCRIPT_LT = 228,
	INPUT_SCRIPT_RT = 229,
	INPUT_CELLPHONE_LEFT = 174,
	INPUT_CELLPHONE_RIGHT = 175,

};

int titleTextRed = 255;
int titleTextGreen = 255;
int titleTextBlue = 255;
int titleTextOpacity = 255;
int selectedPlayer;

#pragma warning(disable : 4244 4305) // double <-> float conversions
#define PROP_MONEY_BAG_01 0x113FD533
#define PICKUP_MONEY_CASE 0xCE6FDD6B
#define MAIN_PERSISTENT 0x5700179C
#define PICKUP_MONEY_CASE 0xCE6FDD6B
#define PICKUP_MONEY_VARIABLE 0xEA888D49


void drawText(char * text, int font, float x, float y, float scalex, float scaley, int r, int b, int g, int a, bool center)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(scalex, scaley);
	UI::SET_TEXT_COLOUR(r, g, b, a);
	UI::SET_TEXT_WRAP(0.0f, 1.0f);
	UI::SET_TEXT_CENTRE(center);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_OUTLINE();
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_TEXT(x, y);
}


void drawNotification(char* msg)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(msg);
	UI::_DRAW_NOTIFICATION(2000, 1);
}

void map_player_status(char *message, char *name, int health, int armor, float distance, char *me)
{
	char text[500];
	sprintf_s(text, message, name, health, armor, distance, me);
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_NOTIFICATION(false, false);
}

std::string statusText;
DWORD statusTextDrawTicksMax;
bool statusTextGxtEntry;

void update_status_text()
{
	if (GetTickCount() < statusTextDrawTicksMax)
	{
		UI::SET_TEXT_FONT(1);
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_WRAP(0.0, 1.0);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(1, 1, 1, 1, 1);
		UI::SET_TEXT_EDGE(1, 1, 1, 1, 200);

		if (statusTextGxtEntry)
		{
			UI::_SET_TEXT_ENTRY((char *)statusText.c_str());
		}
		else
		{
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING((char *)statusText.c_str());
		}
		UI::_DRAW_TEXT(0.5, 0.5); //55
	}
}

void set_status_text(std::string str, DWORD time = 2500, bool isGxtEntry = false)
{

	statusText = str;
	statusTextDrawTicksMax = GetTickCount() + time;
	statusTextGxtEntry = isGxtEntry;
}


void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7)
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	GRAPHICS::DRAW_RECT((A_0 + (A_2 * 0.5f)), (A_1 + (A_3 * 0.5f)), A_2, A_3, A_4, A_5, A_6, A_7);

	drawText("                                 Weclome To ~b~Halloween, ~w~enjoy!!", 1, 0.45, 0.132f, 0.50f, 0.40f, titleTextRed, titleTextGreen, titleTextBlue, titleTextOpacity, true);
	drawText("~b~Halloween", 7, 0.50, 0.09f, 0.75f, 0.75f, titleTextRed, titleTextGreen, titleTextBlue, titleTextOpacity, true);
	UI::HIDE_HUD_AND_RADAR_THIS_FRAME();


	
}

void draw_menu_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText = true)
{
	// default values
	int text_col[4] = { 10, 210, 10, 250.0f }, 
		rect_col[4] = { 0, 0, 0, 180.f };
	float text_scale = 0.35;
	int font = 0; 
	bool outline = false;
	bool dropShadow = false;

	// correcting values for active line
	if (active)
	{
		text_col[0] = 0;
		text_col[1] = 0;
		text_col[2] = 0;
		text_col[3] = 215.f;

		rect_col[0] = 255;
		rect_col[1] = 128;
		rect_col[2] = 0;
		rect_col[3] = 180;//

		//outline = true;

		if (rescaleText) text_scale = 0.40;

	}
	else if (title)
	{
		text_col[0] = 2;
		text_col[1] = 2;
		text_col[2] = 2;

		rect_col[0] = 255;
		rect_col[1] = 128;
		rect_col[2] = 0;
		rect_col[3] = 180;

		if (rescaleText) text_scale = 0.78; 
		outline = true;

		font = 1; 
	}
	else
	{
		outline = true;
	}


	int screen_w, screen_h;
	GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

	textLeft += lineLeft;

	float lineWidthScaled = lineWidth / (float)screen_w; // line width
	float lineTopScaled = lineTop / (float)screen_h; // line top offset
	float textLeftScaled = textLeft / (float)screen_w; // text left offset
	float lineHeightScaled = lineHeight / (float)screen_h; // line height

	float lineLeftScaled = lineLeft / (float)screen_w;

	// this is how it's done in original scripts

	// text upper part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	UI::_DRAW_TEXT(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

	// text lower part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_GXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	int num25 = UI::_0x9040DFB09BE75706(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

	// rect
	draw_rect(lineLeftScaled, lineTopScaled + (0.00278f),
		lineWidthScaled, ((((float)(num25)* UI::_0xDB88A37483346780(text_scale, 0)) + (lineHeightScaled * 1.5f)) + 0.006f),
		rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
}

void set_creator_text(char *text)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_SET_NOTIFICATION_MESSAGE_CLAN_TAG_2("CHAR_SOCIAL_CLUB", "CHAR_SOCIAL_CLUB", 1, 7, "Welcome To", "~b~Crusaders ~w~v1.1!", 1, "", false, true);
	UI::_DRAW_NOTIFICATION(false, false);
	std::string playerName = PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID());
	std::string welcome = "~h~Welcome ~g~";
	set_status_text(welcome + playerName);
	
}



bool trainer_switch_pressed()
{
	return IsKeyJustUp(VK_F5) || (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_LEFT) && CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_RS));

	AUDIO::PLAY_SOUND_FRONTEND(-1, "PROPERTY_PURCHASE", "HUD_AWARDS", 0);

	

	WAIT(3);
}

void get_button_state(bool *a, bool *b, bool *up, bool *down, bool *l, bool *r)
{
	if (a) *a = IsKeyDown(VK_NUMPAD5) || (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_ACCEPT));
	if (b) *b = IsKeyDown(VK_NUMPAD0) || trainer_switch_pressed() || IsKeyDown(VK_BACK) || (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_CANCEL));
	if (up) *up = IsKeyDown(VK_NUMPAD8) || (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_UP));
	if (down) *down = IsKeyDown(VK_NUMPAD2) || (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_FRONTEND_DOWN));
	if (r) *r = IsKeyDown(VK_NUMPAD6) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_CELLPHONE_RIGHT);
	if (l) *l = IsKeyDown(VK_NUMPAD4) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, INPUT_CELLPHONE_LEFT);

	CONTROLS::DISABLE_CONTROL_ACTION(30, INPUT_PHONE, true);

}


void menu_beep()
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
}


Vector3 rotDirection(Vector3* rot)
{
	float radianz = rot->z * 0.0174532924f;
	float radianx = rot->x * 0.0174532924f;
	float num = abs((float)cos((double)radianx));

	Vector3 dir;

	dir.x = (float)((double)((float)(-(float)sin((double)radianz))) * (double)num);
	dir.y = (float)((double)((float)cos((double)radianz)) * (double)num);
	dir.z = (float)sin((double)radianx);

	return dir;
}

Vector3 add(Vector3* a, Vector3* b)
{
	Vector3 result;

	result.x = a->x;
	result.y = a->y;
	result.z = a->z;
	result.x += b->x;
	result.y += b->y;
	result.z += b->z;

	return result;
}

Vector3 multiply(Vector3* coords, float coordz)
{
	Vector3 result;

	result.x = coords->x;
	result.y = coords->y;
	result.z = coords->z;
	result.x *= coordz;
	result.y *= coordz;
	result.z *= coordz;

	return result;
}

float distance(Vector3* pointa, Vector3* pointb)
{
	float ax = pointa->x;
	float ay = pointa->y;
	float az = pointa->z;
	float bx = pointb->x;
	float by = pointb->y;
	float bz = pointb->z;

	double xba = (double)(bx - ax);
	double yba = (double)(by - ay);
	double zba = (double)(bz - az);

	double y2 = yba * yba;
	double x2 = xba * xba;
	double sum = y2 + x2;

	return (float)sqrt(sum + zba);
}

float get_distance(Vector3* pointA, Vector3* pointB)
{
	float a_x = pointA->x;
	float a_y = pointA->y;
	float a_z = pointA->z;

	float b_x = pointB->x;
	float b_y = pointB->y;
	float b_z = pointB->z;

	double x_ba = (double)(b_x - a_x);
	double y_ba = (double)(b_y - a_y);
	double z_ba = (double)(b_z - a_z);

	double y_2 = y_ba * y_ba;
	double x_2 = x_ba * x_ba;
	double sum_2 = y_2 + x_2;

	return (float)sqrt(sum_2 + z_ba);
}

Vector3 rot_to_direction(Vector3* rot)
{
	float radiansZ = rot->z * 0.0174532924f;
	float radiansX = rot->x * 0.0174532924f;
	float num = abs((float)cos((double)radiansX));
	Vector3 dir;
	dir.x = (float)((double)((float)(-(float)sin((double)radiansZ))) * (double)num);
	dir.y = (float)((double)((float)cos((double)radiansZ)) * (double)num);
	dir.z = (float)sin((double)radiansX);
	return dir;
}


// features
bool featurePlayerInvincible = false;
bool featurePlayerInvincibleUpdated = false;
bool featurePlayerNeverWanted = false;
bool featurePlayerIgnored = false;
bool featurePlayerIgnoredUpdated = false;
bool featurePlayerUnlimitedAbility = false;
bool featurePlayerNoNoise = false;
bool featurePlayerNoNoiseUpdated = false;
bool featurePlayerFastSwim = false;
bool featurePlayerFastSwimUpdated = false;
bool featurePlayerFastRun = false;
bool featurePlayerFastRunUpdated = false;
bool featurePlayerSuperJump = false;
bool featurePlayermoneyDrop = false;
bool featurePlayermoneyDrop1 = false;
bool featureRainMoney = false;
bool featurePlayerInvisible = false;
bool featurePlayerInvisibleUpdated = false;
bool featurePlayerSuperRun = false;
bool featurePlayerESPTrace = false;


bool featureWepModVehGun = false;
bool featureWepModAnimalGun = false;

bool featureWeaponNoReload = false;
bool featureWeaponFireAmmo = false;
bool featureWeaponExplosiveAmmo = false;
bool featureWeaponExplosiveMelee = false;
bool featureWeaponVehRockets = false;
bool featureWeaponVehGreen = false;
bool featureWeaponVehFlare = false;
bool featureWeaponVehRed = false;
bool featureWeaponVehSnow = false;
bool featureWeaponVehTank = false;
bool featureVehAllMod = false;
bool featureWeaponOneHit = false;
bool featureWeaponVehFire = false;
bool featureWeaponWhaleGun = false;
bool featureWeaponRainbowTint = false;
bool featureVehScrollingText = false;
bool featureVehSlide = false;
bool featureVehSuperRPM = false;

bool weaponDeleteObjects = false;
bool weaponGravityGun = false;


bool featurePedestrianInvincibleBodyguard = false;
bool featurePedestrianInvincibleBodyguardUpdate = false;

bool featureMiscFlyMode = false;


bool featureVisionNight = false;
bool featureVisionThermal = false;
bool featureVisionDrunk = false;
bool featureVisionDrunkUpdated = false;

bool featurePlayerBlips = false;
bool featurePlayerHeadDisplay = false;
bool featurePlayerBlipCone = false;
bool featurePlayerNotifications = false;
bool featureShowVoiceChatSpeaker = false;


DWORD featureWeaponVehShootLastTime = 0;
DWORD featureWeaponVehShootLastTimeFire = 0;


bool featureWeaponMoneyGun = false;
bool featureWeaponFirework = false;
bool featureWeaponRPG = false;



bool featureVehInvincible  =   false;
bool featureVehInvincibleUpdated = false;
bool featureVehInvincibleWheels = false;
bool featureVehInvincibleWheelsUpdated = false;
bool featureVehSeatbelt = false;
bool featureVehSeatbeltUpdated = false;
bool featureVehSpeedBoost = false;
bool featureVehWrapInSpawned = true;
bool featureVehStickyCar = false;
bool featureVehRainbow = false;
bool featureVehRainbowTire = false;

bool featureWorldMoonGravity = false;
bool featureWorldRandomCops = true;
bool featureWorldRandomTrains = true;
bool featureWorldRandomBoats = true;
bool featureWorldGarbageTrucks = true;
bool featureWorldCrazywaves = false;

bool featureTimePaused = false;
bool featureTimePausedUpdated = false;
bool featureTimeSynced = false;

bool featureWeatherWind = false;
bool featureWeatherPers = false;

bool featureMiscLockRadio = false;
bool featureMiscHideHud = false;
bool featureMiscMobile = false;
bool featurePlayerUltraJump = false;

bool featureESP = false;
bool featureNameESP = false;

bool freezeplayer = false;

bool LoadingAnim = false;
bool LoadingAnimForPed = false;
char* Anim;
char* AnimID;
int PedToDoAnim;


// player model control, switching on normal ped model when needed	
void check_player_model()
{
	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;

	Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);
	if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, true))
		if (model != GAMEPLAY::GET_HASH_KEY("player_zero") &&
			model != GAMEPLAY::GET_HASH_KEY("player_one") &&
			model != GAMEPLAY::GET_HASH_KEY("player_two"))
		{
			set_status_text("turning to ~o~normal~w~..");
			WAIT(1000);

			model = GAMEPLAY::GET_HASH_KEY("player_zero");
			STREAMING::REQUEST_MODEL(model);
			while (!STREAMING::HAS_MODEL_LOADED(model))
				WAIT(0);
			PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
			PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

			// wait until player is ressurected
			while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, true))
				WAIT(0);

		}
}

void update_weapon_whale() {
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	Vector3 dir = rot_to_direction(&rot);
	Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	float spawnDistance = get_distance(&camPosition, &playerPosition);
	spawnDistance += 5;
	Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

	Hash model = GAMEPLAY::GET_HASH_KEY("a_c_killerwhale");

	if (featureWeaponWhaleGun)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
		{
			if (STREAMING::IS_MODEL_VALID(model))
			{
				if (!STREAMING::HAS_MODEL_LOADED(model))
				{
					STREAMING::REQUEST_MODEL(model);
					while (!STREAMING::HAS_MODEL_LOADED(model))
					{
						WAIT(0);
					}
				}

				if (STREAMING::HAS_MODEL_LOADED(model))
				{
					Ped spawnedPed = PED::CREATE_PED(26, model, spawnPosition.x, spawnPosition.y, spawnPosition.z, 1, 1, 1);

					if (ENTITY::DOES_ENTITY_EXIST(spawnedPed))
					{
						ENTITY::SET_ENTITY_RECORDS_COLLISIONS(spawnedPed, true);
						for (float f = 0.0f; f < 75.0f; f++)
						{
							if (ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(spawnedPed)) break;

							ENTITY::APPLY_FORCE_TO_ENTITY(spawnedPed, 1, dir.x * 10.0f, dir.y * 10.0f, dir.z * 10.0f, 0.0f, 0.0f, 0.0f, 0, 0, 1, 1, 0, 1);

							WAIT(0);
						}

						Vector3 coords = ENTITY::GET_ENTITY_COORDS(spawnedPed, 1);
						PED::DELETE_PED(&spawnedPed);
						FIRE::ADD_OWNED_EXPLOSION(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, 9, 25.0f, 1, 0, 0.5f);
					}
				}
			}
		}
	}
}





void update_vehicle_Snow()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehSnow) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_SNOWBALL");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();


	}
}


void update_vehicle_Green()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehGreen) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_PLAYER_LASER");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();


	}
}



void update_vehicle_Flare()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehFlare) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_FLAREGUN");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();


	}
}

bool RequestNetworkControl(uint vehID)
{
	int Tries = 0;
	bool
		hasControl = false,
		giveUp = false;
	do
	{
		hasControl = NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(vehID);
		Tries++;
		if (Tries > 300)
			giveUp = true;
	} while (!hasControl && !giveUp);

	if (giveUp)
		return false;
	else return true;
}


void update_vehicle_guns()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehRockets) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();


	}
}




void update_vehicle_Tank()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehTank) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_TANK");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();


	}
}


void update_vehicle_Red()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehRed) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_ENEMY_LASER");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();


	}
}


void update_vehicle_guns_fire()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehFire) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTimeFire + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetFire = GAMEPLAY::GET_HASH_KEY("WEAPON_FIREWORK");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetFire))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetFire, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetFire))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetFire, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetFire, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTimeFire = GetTickCount();
	}
}



void Freeze(Ped selectedPed)
{
	AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPed);
	AI::CLEAR_PED_SECONDARY_TASK(selectedPed);
}

void ApplyForceToEntity(Player ped_id, int x, int y, int z)
{
	ENTITY::APPLY_FORCE_TO_ENTITY(ped_id, 1, 0, x, y, z, 0, 0, 0, 1, 1, 1, 0, 1);
}


void flyMode()
{
	Hash hash = GAMEPLAY::GET_HASH_KEY("GADGET_PARACHUTE");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), hash, 1, 1);
	PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(), 1);
	PED::SET_PED_TO_RAGDOLL_WITH_FALL(PLAYER::PLAYER_PED_ID(), 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0);

	if (ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_RAGDOLL(PLAYER::PLAYER_PED_ID()))
	{
		if (GetAsyncKeyState(0x57)) // W key
		{
			ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 3, 0, 0);
		}

		if (GetAsyncKeyState(0x53)) // S key
		{
			ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 3, 6, 0);
		}
		if (GetAsyncKeyState(VK_SHIFT)) //VK_SHIFT
		{
			ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 6, 0, 0);
		}
	}
}






bool TeleportClientHook(int Client, float Coords, float y, float z)
{
	Ped handle = PLAYER::GET_PLAYER_PED(Client);
	if (PED::IS_PED_IN_ANY_VEHICLE(handle, 0))
	{
		handle = PED::GET_VEHICLE_PED_IS_IN(handle, 0);
		if (NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(handle))
		{
			ENTITY::SET_ENTITY_COORDS(handle, Coords, y, z, 0, 0, 0, 1);
			drawNotification("~o~Teleported!");
			return true;
		}
		return false;
	}
	else
	{
		drawNotification("~r~Player is not in a vehicle!");
		return false;
	}
}


void applyForceToEntity(Entity entity, float x, float y, float z)
{
	ENTITY::APPLY_FORCE_TO_ENTITY(entity, 1, x, y, z, 0.0f, 0.0f, 0.0f, 0, TRUE, TRUE, TRUE, FALSE, TRUE);
}




bool skinchanger_used = false;

// Updates all features that can be turned off by the game, being called each game frame
void update_features()
{
	update_status_text();

	update_vehicle_guns();

	update_vehicle_guns_fire();

	update_vehicle_Snow();

	update_vehicle_Green();

	update_vehicle_Tank();

	update_vehicle_Red();

	update_vehicle_Flare();

	update_weapon_whale();

	Vector3 ONLINE_PLAYER_COORDS = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);

	// changing player model if died/arrested while being in another skin, since it can cause inf loading loop
	if (skinchanger_used)
		check_player_model();

	// wait until player is ready, basicly to prevent using the trainer while player is dead or arrested
	while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), true))
		WAIT(0);

	// read default feature values from the game
	featureWorldRandomCops = PED::CAN_CREATE_RANDOM_COPS() == true;

	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	// player invincible
	if (featurePlayerInvincibleUpdated)
	{
		if (bPlayerExists && !featurePlayerInvincible)
			PLAYER::SET_PLAYER_INVINCIBLE(player, false);
		featurePlayerInvincibleUpdated = false;
	}
	if (featurePlayerInvincible)
	{
		if (bPlayerExists)
			PLAYER::SET_PLAYER_INVINCIBLE(player, true);
	}


	// player never wanted
	if (featurePlayerNeverWanted)
	{
		if (bPlayerExists)
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
	}

	if (featureWeaponOneHit)
	{
		if (bPlayerExists)
		{
			PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, 999999);
		}
	}

	// player super run
	if (featurePlayerSuperRun)
	{
		if (AI::IS_PED_RUNNING(playerPed) || AI::IS_PED_SPRINTING(playerPed) && !PED::IS_PED_RUNNING_RAGDOLL_TASK(playerPed))
		{
			applyForceToEntity(playerPed, 0.0f, 1.0f, 0.0f);
		}
	}

	// police ignore player
	if (featurePlayerIgnoredUpdated)
	{
		if (bPlayerExists)
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, featurePlayerIgnored);
		featurePlayerIgnoredUpdated = false;
	}


	if (featureVehSuperRPM, PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, 125);
	}
	if (!featureVehSuperRPM)
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, 0);

	}

	if (featurePlayerUltraJump)
	{
		GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(player);
		WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, GAMEPLAY::GET_HASH_KEY("GADGET_PARACHUTE"));
		if (PED::IS_PED_JUMPING(playerPed))
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, true, 0, 0, 5, 0, 0, 0, true, true, true, true, false, true);
		}
	}


	if (featureESP)
	{
		Player playerPed = PLAYER::PLAYER_PED_ID();
		for (int i = 0; i < 32; i++)
		{
			Player playerHandle = PLAYER::GET_PLAYER_PED(i);
			Vector3 handleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0, 0, 0);
			char* Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i));

			if (ENTITY::DOES_ENTITY_EXIST(playerHandle))
			{
				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 0, 255, 0, 255); // top Box
				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 0, 255, 0, 255);

				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, 0, 255, 0, 255); // bottom Box
				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, 0, 255, 0, 255);

				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 0, 255, 0, 255); // bottom Box
				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 0, 255, 0, 255);
			}
		}
	}


	Object object;
	if (weaponDeleteObjects && bPlayerExists && PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &object) && ENTITY::IS_ENTITY_AN_OBJECT(object) && ENTITY::GET_ENTITY_ALPHA(object) == 255.0)
	{
		OBJECT::DELETE_OBJECT(&object);
	}



	if (featureNameESP)
	{
		Player playerPed = PLAYER::PLAYER_PED_ID();
		for (int i = 0; i < 32; i++)
		{
			Player playerHandle = PLAYER::GET_PLAYER_PED(i);
			Vector3 handleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0, 0, 0);
			Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
			char* Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i));

			if (ENTITY::DOES_ENTITY_EXIST(playerHandle))
			{
				float x1;
				float y1;

				BOOL screenCoords = GRAPHICS::_WORLD3D_TO_SCREEN2D(handleCoords.x, handleCoords.y, handleCoords.z, &x1, &y1);

				std::string playerName = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i));

				std::string nameSetupRed = "~HUD_COLOUR_RED~" + playerName;
				std::string nameSetupGreen = "~HUD_COLOUR_GREEN~" + playerName;

				char* playerInfoRed = new char[nameSetupRed.length() + 1];
				char* playerInfoGreen = new char[nameSetupGreen.length() + 1];

				std::strcpy(playerInfoRed, nameSetupRed.c_str());
				std::strcpy(playerInfoGreen, nameSetupGreen.c_str());

				UI::SET_TEXT_FONT(7);
				UI::SET_TEXT_SCALE(0.0, 0.40);
				UI::SET_TEXT_COLOUR(0, 255, 0, 255);
				UI::SET_TEXT_CENTRE(0);
				UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
				UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
				UI::_SET_TEXT_ENTRY("STRING");
				if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(playerPed, playerHandle, 17))
				{
					UI::_ADD_TEXT_COMPONENT_STRING(playerInfoGreen);
				}
				else
				{
					UI::_ADD_TEXT_COMPONENT_STRING(playerInfoRed);
				}
				UI::_DRAW_TEXT(x1, y1);
				UI::SET_TEXT_OUTLINE();
				UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
			}
		}
	}

	if (freezeplayer)
	{
		AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::GET_PLAYER_PED(selectedPlayer));
	}

	// weapon money gun
	if (featureWeaponMoneyGun && bPlayerExists)
	{
		if (ENTITY::DOES_ENTITY_EXIST(playerPed) && PED::IS_PED_SHOOTING(playerPed))
		{
			float xD[6];
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, (Vector3*)xD);
			if (xD[0] != 0 || xD[2] != 0 || xD[4] != 0)
			{
				STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_money_bag_01"));
				while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_money_bag_01"))) WAIT(0);
				{
					OBJECT::CREATE_AMBIENT_PICKUP(GAMEPLAY::GET_HASH_KEY("pickup_money_case"), xD[0], xD[2], xD[4], 0.0, 40000.0, GAMEPLAY::GET_HASH_KEY("prop_money_bag_01"), false, true);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(GAMEPLAY::GET_HASH_KEY("prop_money_bag_01"));
				}
			}
		}
	}

	// weapon firework rounds
	if (featureWeaponFirework)
	{
		float startDistance = distance(&CAM::GET_GAMEPLAY_CAM_COORD(), &ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true));
		float endDistance = distance(&CAM::GET_GAMEPLAY_CAM_COORD(), &ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true));
		startDistance += 0.25;
		endDistance += 1000.0;
		if (PED::IS_PED_ON_FOOT(playerPed) && PED::IS_PED_SHOOTING(playerPed))
		{
			Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_FIREWORK"); //WEAPON_FIREWORK
			if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
			{
				WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, false);
				while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
					WAIT(0);
			}
			GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).x, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).y, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).z, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).x, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).y, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).z, 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		}
	}

	if (featureVehSlide)
	{
		VEHICLE::SET_VEHICLE_REDUCE_GRIP(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0), 1);
	}
	else
	{
		VEHICLE::SET_VEHICLE_REDUCE_GRIP(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0), 0);
	}

	std::string plateSymbols[35] = {
		"a", "b", "c", "d", "e", "f",
		"g", "h", "i", "j", "k", "l",
		"m", "n", "o", "p", "q",
		"r", "s", "t", "u", "v",
		"w", "x", "y", "z", "1",
		"2", "3", "4", "5", "6", "7",
		"8", "9",
	};

	if (featureVehScrollingText)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			std::string textInput = plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35];
			char* plateText = (strcpy((char*)malloc(textInput.length() + 1), textInput.c_str()));
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_IN(playerPed, 0), plateText);
		}
	}



	// weapon rps rounds
	if (featureWeaponRPG)
	{
		float startDistance = distance(&CAM::GET_GAMEPLAY_CAM_COORD(), &ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true));
		float endDistance = distance(&CAM::GET_GAMEPLAY_CAM_COORD(), &ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true));
		startDistance += 0.25;
		endDistance += 1000.0;
		if (PED::IS_PED_ON_FOOT(playerPed) && PED::IS_PED_SHOOTING(playerPed))
		{
			Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET");
			if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
			{
				WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, false);
				while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
					WAIT(0);
			}
			GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).x, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).y, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).z, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).x, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).y, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).z, 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		}
	}

	LPCSTR vehModels[283] = {
		"NINEF", "NINEF2", "BLISTA", "ASEA", "ASEA2", "T20", "BRAWLER", "COQUETTE3", "VINDICATOR"
		"COACH", "AIRBUS", "ASTEROPE", "AIRTUG", "AMBULANCE", "BARRACKS", "BARRACKS2", "BALLER", "BALLER2",
		"BJXL", "BANSHEE", "BENSON", "BFINJECTION", "BIFF", "BLAZER", "BLAZER2", "BLAZER3", "BISON", "BISON2",
		"BISON3", "BOXVILLE", "BOXVILLE2", "BOXVILLE3", "BOBCATXL", "BODHI2", "BUCCANEER", "BUFFALO", "BUFFALO2", "BULLDOZER",
		"BULLET", "BURRITO", "BURRITO2", "BURRITO3", "BURRITO4", "BURRITO5", "CAVALCADE", "CAVALCADE2", "POLICET",
		"GBURRITO", "CABLECAR", "CADDY", "CADDY2", "CAMPER", "CARBONIZZARE", "CHEETAH", "COMET2", "COGCABRIO", "COQUETTE",
		"CUTTER", "GRESLEY", "DILETTANTE", "DILETTANTE2", "DUNE", "DUNE2", "HOTKNIFE", "DLOADER", "DUBSTA", "DUBSTA2",
		"DUMP", "RUBBLE", "DOCKTUG", "DOMINATOR", "EMPEROR", "EMPEROR2", "EMPEROR3", "ENTITYXF", "EXEMPLAR", "ELEGY2",
		"F620", "FBI", "FBI2", "FELON", "FELON2", "FELTZER2", "FIRETRUK", "FLATBED", "FORKLIFT", "FQ2",
		"FUSILADE", "FUGITIVE", "FUTO", "GRANGER", "GAUNTLET", "HABANERO", "HAULER", "HANDLER", "INFERNUS", "INGOT",
		"INTRUDER", "ISSI2", "JACKAL", "JOURNEY", "JB700", "KHAMELION", "LANDSTALKER", "LGUARD", "MANANA", "MESA",
		"MESA2", "MESA3", "CRUSADER", "MINIVAN", "MIXER", "MIXER2", "MONROE", "MOWER", "MULE", "MULE2",
		"ORACLE", "ORACLE2", "PACKER", "PATRIOT", "PBUS", "PENUMBRA", "PEYOTE", "PHANTOM", "PHOENIX", "PICADOR",
		"POUNDER", "POLICE", "POLICE4", "POLICE2", "POLICE3", "POLICEOLD1", "POLICEOLD2", "PONY", "PONY2", "PRAIRIE",
		"PRANGER", "PREMIER", "PRIMO", "RANCHERXL", "RANCHERXL2", "RAPIDGT", "RAPIDGT2", "RADI", "RATLOADER",
		"REBEL", "REGINA", "REBEL2", "RENTALBUS", "RUINER", "RUMPO", "RUMPO2", "RHINO", "RIOT", "RIPLEY",
		"ROCOTO", "ROMERO", "SABREGT", "SADLER", "SADLER2", "SANDKING", "SANDKING2", "SCHAFTER2", "SCHWARZER", "SCRAP",
		"SEMINOLE", "SENTINEL", "SENTINEL2", "ZION", "ZION2", "SERRANO", "SHERIFF", "SHERIFF2", "SPEEDO", "SPEEDO2",
		"STANIER", "STINGER", "STINGERGT", "STOCKADE", "STOCKADE3", "STRATUM", "SULTAN", "SUPERD", "SURANO", "SURFER",
		"SURFER2", "SURGE", "TACO", "TAILGATER", "TAXI", "TRASH", "TRACTOR", "TRACTOR2", "TRACTOR3",
		"TIPTRUCK", "TIPTRUCK2", "TORNADO", "TORNADO2", "TORNADO3", "TORNADO4", "TOURBUS", "TOWTRUCK", "TOWTRUCK2",
		"UTILLITRUCK", "UTILLITRUCK2", "UTILLITRUCK3", "VOODOO2", "WASHINGTON", "STRETCH", "YOUGA", "ZTYPE", "SANCHEZ", "SANCHEZ2",
		"POLICEB", "AKUMA", "CARBONRS", "HEXER", "NEMESIS", "ADDER", "VOLTIC", "VACCA", "BIFTA",
		"BAGGER", "BATI", "BATI2", "RUFFIAN", "DAEMON", "DOUBLE", "PCJ", "VADER", "VIGERO", "FAGGIO2",
		"PARADISE", "KALAHARI", "JESTER", "TURISMOR", "ALPHA", "HUNTLEY", "THRUST", "MASSACRO",
		"MASSACRO2", "ZENTORNO", "BLADE", "GLENDALE", "PANTO", "PIGALLE", "WARRENER", "RHAPSODY", "DUBSTA3", "MONSTER",
		"SOVEREIGN", "INNOVATION", "HAKUCHOU", "FUROREGT", "COQUETTE2", "BTYPE", "BUFFALO3", "DOMINATOR2", "GAUNTLET2",
		"MARSHALL", "DUKES", "DUKES2", "STALION", "STALION2", "BLISTA2", "BLISTA3", "VIRGO", "WINDSOR",
		"INSURGENT", "INSURGENT2", "TECHNICAL", "KURUMA", "KURUMA2", "JESTER2", "CASCO",
		"GUARDIAN", "ENDURO", "LECTRO", "SLAMVAN", "SLAMVAN2", "RATLOADER2", "FELTZER3", "OSIRIS"
	};

	if (featureWepModVehGun)
	{
		FLOAT heading = ENTITY::GET_ENTITY_HEADING(playerPed);
		DWORD model = GAMEPLAY::GET_HASH_KEY((char *)vehModels[rand() % 283]);
		Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
		Vector3 dir = rot_to_direction(&rot);
		Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
		float spawnDistance = get_distance(&camPosition, &playerPosition);
		spawnDistance += 5;
		Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

		if (bPlayerExists)
		{
			if (PED::IS_PED_SHOOTING(playerPed))
			{
				Vector3 playerOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0, 5, 0);
				if (PED::IS_PED_ON_FOOT(playerPed))
				{
					STREAMING::REQUEST_MODEL(model);
					while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
					{
						Vehicle veh = VEHICLE::CREATE_VEHICLE(model, spawnPosition.x, spawnPosition.y, spawnPosition.z, heading, 1, 1);
						VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 120.0);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
						ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
					}

				}
			}
		}
	}


	static LPCSTR weaponNames[] = {
		"WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR",
		"WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_MICROSMG", "WEAPON_SMG",
		"WEAPON_ASSAULTSMG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_MG",
		"WEAPON_COMBATMG", "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_BULLPUPSHOTGUN",
		"WEAPON_STUNGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_GRENADELAUNCHER", "WEAPON_GRENADELAUNCHER_SMOKE",
		"WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_SMOKEGRENADE", "WEAPON_BZGAS",
		"WEAPON_MOLOTOV", "WEAPON_FIREEXTINGUISHER", "WEAPON_PETROLCAN",
		"WEAPON_SNSPISTOL", "WEAPON_SPECIALCARBINE", "WEAPON_HEAVYPISTOL", "WEAPON_BULLPUPRIFLE", "WEAPON_HOMINGLAUNCHER",
		"WEAPON_PROXMINE", "WEAPON_SNOWBALL", "WEAPON_VINTAGEPISTOL", "WEAPON_DAGGER", "WEAPON_FIREWORK", "WEAPON_MUSKET",
		"WEAPON_MARKSMANRIFLE", "WEAPON_HEAVYSHOTGUN", "WEAPON_GUSENBERG", "WEAPON_HATCHET", "WEAPON_RAILGUN", "WEAPON_FLAREGUN",
		"WEAPON_KNUCKLE", "GADGET_NIGHTVISION", "GADGET_PARACHUTE", "WEAPON_MARKSMANPISTOL", "", ""
	};
	if (bPlayerExists && featureWeaponRainbowTint)
	{
		for (int i = 0; i < sizeof(weaponNames) / sizeof(weaponNames[0]); i++)
		{
			if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), 0))
			{
				WEAPON::SET_PED_WEAPON_TINT_INDEX(playerPed, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), rand() % 8);
			}
		}
	}

	LPCSTR animalModels[26] = { "a_c_boar", "a_c_chimp", "a_c_cow", "a_c_coyote", "a_c_deer", "a_c_fish", "a_c_hen", "a_c_cat_01", "a_c_chickenhawk",
		"a_c_cormorant", "a_c_crow", "a_c_dolphin", "a_c_humpback", "a_c_killerwhale", "a_c_pigeon", "a_c_seagull", "a_c_sharkhammer",
		"a_c_pig", "a_c_rat", "a_c_rhesus", "a_c_chop", "a_c_husky", "a_c_mtlion", "a_c_retriever", "a_c_gsharktiger", "a_c_shepherd" };


	if (featureWepModAnimalGun)
	{
		if (PED::IS_PED_SHOOTING(playerPed))
		{
			Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
			Vector3 dir = rot_to_direction(&rot);
			Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
			Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
			float spawnDistance = get_distance(&camPosition, &playerPosition);
			spawnDistance += 2;
			Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));
			Hash model = GAMEPLAY::GET_HASH_KEY((char *)animalModels[rand() % 26]);
			if (PED::IS_PED_SHOOTING(playerPed))
			{
				Vector3 playerOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0, 5, 0);
				if (PED::IS_PED_ON_FOOT(playerPed))
				{
					STREAMING::REQUEST_MODEL(model);
					while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
					{
						Ped spawnedPed = PED::CREATE_PED(26, model, spawnPosition.x, spawnPosition.y, spawnPosition.z, ENTITY::GET_ENTITY_HEADING(playerPed), 1, 1);
						ENTITY::SET_ENTITY_HEALTH(spawnedPed, 0);
						ENTITY::APPLY_FORCE_TO_ENTITY(spawnedPed, 1, dir.x * 10000.0f, dir.y * 10000.0f, dir.z * 10000.0f, 0.0f, 0.0f, 0.0f, 0, 0, 1, 1, 0, 1);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
						ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&spawnedPed);
					}
				}
			}
		}
	}




	// Rain Money Bags
	if (featureRainMoney)
	{
		STREAMING::REQUEST_MODEL(0x113FD533);
		Vector3 pp = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 0.0, 10.0);
		int offset1 = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(5, 15);
		int offset2 = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(5, 15);


		STREAMING::REQUEST_MODEL(PROP_MONEY_BAG_01);
		while (!STREAMING::HAS_MODEL_LOADED(PROP_MONEY_BAG_01))
			WAIT(0);
		if (STREAMING::HAS_MODEL_LOADED(0x113FD533))
		{
			int AMOUNT = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(20000, 40000); //randomize amount for each bag
			OBJECT::CREATE_AMBIENT_PICKUP(PICKUP_MONEY_CASE, pp.x + offset1, pp.y + offset2, pp.z, 0, AMOUNT, 0x113FD533, false, true);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(0x113FD533);
		}
	}

	// player special ability
	if (featurePlayerUnlimitedAbility)
	{
		if (bPlayerExists)
			PLAYER::SPECIAL_ABILITY_FILL_METER(player, 1);
	}

	if (featureVehRainbow)
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		if (PED::IS_PED_IN_ANY_VEHICLE)
		{
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
		}
	}

	if (featureVehRainbowTire)
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		if (PED::IS_PED_IN_ANY_VEHICLE)
		{
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, rand() % 255, rand() % 255, rand() % 255);
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, rand() % 255, rand() % 255, rand() % 255);
		}
	}



	// player no noise
	if (featurePlayerNoNoiseUpdated)
	{
		if (bPlayerExists && !featurePlayerNoNoise)
			PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 1.0);
		featurePlayerNoNoiseUpdated = false;
	}
	if (featurePlayerNoNoise)
		PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 0.0);

	// player fast swim
	if (featurePlayerFastSwimUpdated)
	{
		if (bPlayerExists && !featurePlayerFastSwim)
			PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.0);
		featurePlayerFastSwimUpdated = false;
	}
	if (featurePlayerFastSwim)
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.49);

	// player fast run
	if (featurePlayerFastRunUpdated)
	{
		if (bPlayerExists && !featurePlayerFastRun)
			PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.0);
		featurePlayerFastRunUpdated = false;
	}
	if (featurePlayerFastRun)
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.49);

	// player super jump
	if (featurePlayerSuperJump)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(player);
	}

	if (featurePlayermoneyDrop)
	{
		STREAMING::REQUEST_MODEL(PROP_MONEY_BAG_01);
		while (!STREAMING::HAS_MODEL_LOADED(PROP_MONEY_BAG_01))
			WAIT(0);
		if (STREAMING::HAS_MODEL_LOADED(PROP_MONEY_BAG_01))
		{
			int NUMBER_OF_BAGS = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(6, 12);
			int MONEY = (GAMEPLAY::GET_RANDOM_INT_IN_RANGE(40000, 120001) / NUMBER_OF_BAGS);
			MONEY = (int)round(MONEY);
			for (int i = 0; i < NUMBER_OF_BAGS; i++)
			{
				OBJECT::CREATE_AMBIENT_PICKUP(PICKUP_MONEY_CASE, ONLINE_PLAYER_COORDS.x, ONLINE_PLAYER_COORDS.y, ONLINE_PLAYER_COORDS.z + 0.15f, 0, MONEY, PROP_MONEY_BAG_01, false, true);
			}
		}
	}

	if (featurePlayermoneyDrop1)
	{
		for (int i = 0; i < 3; i++)
			if (bPlayerExists)
			{
				char statNameFull[32];
				sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
				Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
				int val;
				STATS::STAT_GET_INT(hash, &val, -1);
				val += 2000;
				STATS::STAT_SET_INT(hash, val, 1);
				Vector3 ENTPOS = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);
				OBJECT::CREATE_AMBIENT_PICKUP(GAMEPLAY::GET_HASH_KEY("PICKUP_MONEY_VARIABLE"), ENTPOS.x, ENTPOS.y, ENTPOS.z, 0, 500000, 1, false, true);
			}
	}

	// weapon
	if (featureWeaponFireAmmo)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_FIRE_AMMO_THIS_FRAME(player);
	}
	if (featureWeaponExplosiveAmmo)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_EXPLOSIVE_AMMO_THIS_FRAME(player);
	}
	if (featureWeaponExplosiveMelee)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_EXPLOSIVE_MELEE_THIS_FRAME(player);
	}

	if (featureVisionDrunkUpdated)
	{
		featureVisionDrunkUpdated = false;
		if (featureVisionDrunk)
		{
			STREAMING::REQUEST_ANIM_SET("move_m@drunk@verydrunk");
			while (!STREAMING::HAS_ANIM_SET_LOADED("move_m@drunk@verydrunk"))
			{
				WAIT(0);
			}
			PED::SET_PED_MOVEMENT_CLIPSET(playerPed, "move_m@drunk@verydrunk", 1.0f);
			CAM::SHAKE_GAMEPLAY_CAM("DRUNK_SHAKE", 1.0f);
		}
		else
		{
			PED::RESET_PED_MOVEMENT_CLIPSET(playerPed, 1.0f);
			CAM::STOP_GAMEPLAY_CAM_SHAKING(true);
		}
		AUDIO::SET_PED_IS_DRUNK(playerPed, featureVisionDrunk);
	}


	// weapon no reload
	if (bPlayerExists && featureWeaponNoReload)
	{
		Hash cur;
		if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &cur, 1))
		{
			if (WEAPON::IS_WEAPON_VALID(cur))
			{
				int maxAmmo;
				if (WEAPON::GET_MAX_AMMO(playerPed, cur, &maxAmmo))
				{
					WEAPON::SET_PED_AMMO(playerPed, cur, maxAmmo);

					maxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, cur, 1);
					if (maxAmmo > 0)
						WEAPON::SET_AMMO_IN_CLIP(playerPed, cur, maxAmmo);
				}
			}
		}
	}

	// player's vehicle invincible
	if (featureVehInvincibleUpdated)
	{
		if (bPlayerExists && !featureVehInvincible && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			ENTITY::SET_ENTITY_INVINCIBLE(veh, false);
			ENTITY::SET_ENTITY_PROOFS(veh, 0, 0, 0, 0, 0, 0, 0, 0);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 1);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, 1);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, 1);
		}
		featureVehInvincibleUpdated = false;
	}
	if (featureVehInvincible)
	{
		if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			ENTITY::SET_ENTITY_INVINCIBLE(veh, true);
			ENTITY::SET_ENTITY_PROOFS(veh, 1, 1, 1, 1, 1, 1, 1, 1);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 0);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, 0);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, 0);
		}
	}

	// player's vehicle invincible wheels, usefull with custom handling
	if (featureVehInvincibleWheelsUpdated)
	{
		if (bPlayerExists && !featureVehInvincibleWheels && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, true);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, true);
			VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(veh, false);
		}
		featureVehInvincibleWheelsUpdated = false;
	}
	if (featureVehInvincibleWheels)
	{
		if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, false);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, false);
			VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(veh, true);
		}
	}

	// seat belt
	const int PED_FLAG_CAN_FLY_THRU_WINDSCREEN = 32;
	if (featureVehSeatbeltUpdated)
	{
		if (bPlayerExists && !featureVehSeatbelt)
			PED::SET_PED_CONFIG_FLAG(playerPed, PED_FLAG_CAN_FLY_THRU_WINDSCREEN, true);
		featureVehSeatbeltUpdated = false;
	}
	if (featureVehSeatbelt)
	{
		if (bPlayerExists)
		{
			if (PED::GET_PED_CONFIG_FLAG(playerPed, PED_FLAG_CAN_FLY_THRU_WINDSCREEN, true))
				PED::SET_PED_CONFIG_FLAG(playerPed, PED_FLAG_CAN_FLY_THRU_WINDSCREEN, false);
		}
	}

	// player's vehicle boost
	if (featureVehSpeedBoost && bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		bool bUp = IsKeyDown(VK_NUMPAD9);
		bool bDown = IsKeyDown(VK_NUMPAD3);

		if (bUp || bDown)
		{
			float speed = ENTITY::GET_ENTITY_SPEED(veh);
			if (bUp)
			{
				if (speed < 3.0f) speed = 3.0f;
				speed += speed * 0.05f;
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, speed);
			}
			else
				if (ENTITY::IS_ENTITY_IN_AIR(veh) || speed > 5.0)
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 0.0);
		}
	}

	if (featureMiscFlyMode)
	{
		flyMode();
	}


	// time pause
	if (featureTimePausedUpdated)
	{
		TIME::PAUSE_CLOCK(featureTimePaused);
		featureTimePausedUpdated = false;
	}

	// time sync
	if (featureTimeSynced)
	{
		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);
		TIME::SET_CLOCK_TIME(t.tm_hour, t.tm_min, t.tm_sec);
	}




	//Player Invisible
	if (featurePlayerInvisibleUpdated)
	{
		featurePlayerInvisibleUpdated = false;
		if (bPlayerExists && featurePlayerInvisible)
			ENTITY::SET_ENTITY_VISIBLE(playerPed, false, true);
		else if (bPlayerExists) { ENTITY::SET_ENTITY_VISIBLE(playerPed, true, false); }
	}


	// hide hud
	if (featureMiscHideHud)
		UI::HIDE_HUD_AND_RADAR_THIS_FRAME();                                    

	if (featureMiscMobile)
		AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);

	if (featureVehAllMod)
	{

		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 1);
		VEHICLE::SET_VEHICLE_COLOURS(veh, 120, 120);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "CRUSADER");
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, 1);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, 1);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, 1);
		VEHICLE::SET_VEHICLE_MOD(veh, 16, 5, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 12, 2, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 11, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 14, 14, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 15, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 13, 2, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, 6);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 5);
		VEHICLE::SET_VEHICLE_MOD(veh, 23, 19, 1);
		VEHICLE::SET_VEHICLE_MOD(veh, 0, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 1, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 2, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 3, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 4, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 5, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 6, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 7, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 8, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 9, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 10, 1, 0);
	}

	if (featureVehStickyCar)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		{
			int myVehicle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(myVehicle);
		}
	}

}


LPCSTR pedModels[2][10] = {
	{ "player_zero", "player_one", "player_two", "a_c_boar", "a_c_chimp", "a_c_cow", "a_c_coyote", "a_c_deer", "a_c_fish", "a_c_hen" },
	{ "a_c_cat_01", "a_c_chickenhawk", "a_c_cormorant", "a_c_crow", "a_c_dolphin", "a_c_humpback", "a_c_killerwhale", "a_c_pigeon", "a_c_seagull", "a_c_sharkhammer" },
};

LPCSTR pedModelNames[2][10] = {
	{ "MICHAEL", "FRANKLIN", "TREVOR", "BOAR", "CHIMP", "COW", "COYOTE", "DEER", "FISH", "HEN" },
	{ "CAT", "HAWK", "CORMORANT", "CROW", "DOLPHIN", "HUMPBACK", "WHALE", "PIGEON", "SEAGULL", "SHARKHAMMER" },

};


int skinchangerActiveLineIndex = 0;
int skinchangerActiveItemIndex = 0;

bool process_skinchanger_menu()
{
	DWORD waitTime = 150;
	const int lineCount = 2;
	const int itemCount = 10;
	const int itemCountLastLine = 10;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			char caption[32];
			sprintf_s(caption, "MODEL CHANGER   %d / %d", skinchangerActiveLineIndex + 1, lineCount);
			draw_menu_line(caption, 250.0, 0.6, 14.0, 600.0, 4.0, false, true);
			for (int i = 0; i < itemCount; i++)
				if (strlen(pedModels[skinchangerActiveLineIndex][i]) || strcmp(pedModelNames[skinchangerActiveLineIndex][i], "NONE") == 0)
					draw_menu_line(pedModelNames[skinchangerActiveLineIndex][i], 100.0f, 5.0f, 200.0f, 100.0f + i * 110.0f, 5.0f, i == skinchangerActiveItemIndex, false, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		bool bSelect, bBack, bUp, bDown, bLeft, bRight;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, &bLeft, &bRight);

		if (bSelect)
		{
			menu_beep();
			DWORD model = GAMEPLAY::GET_HASH_KEY((char *)pedModels[skinchangerActiveLineIndex][skinchangerActiveItemIndex]);
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
			{
				STREAMING::REQUEST_MODEL(model);
				while (!STREAMING::HAS_MODEL_LOADED(model))	WAIT(0);
				//STREAMING::LOAD_ALL_OBJECTS_NOW();
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
				STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
				GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
				GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
				WAIT(0);
				//PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), false);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				skinchanger_used = true;
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
				waitTime = 200;
			}
		}
		else
			if (bBack)
			{
				menu_beep();
				break;
			}
			else
				if (bRight)
				{
					menu_beep();
					skinchangerActiveItemIndex++;
					int itemsMax = (skinchangerActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
					if (skinchangerActiveItemIndex == itemsMax)
						skinchangerActiveItemIndex = 0;
					waitTime = 100;
				}
				else
					if (bLeft)
					{
						menu_beep();
						if (skinchangerActiveItemIndex == 0)
							skinchangerActiveItemIndex = (skinchangerActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
						skinchangerActiveItemIndex--;
						waitTime = 100;
					}
					else
						if (bUp)
						{
							menu_beep();
							if (skinchangerActiveLineIndex == 0)
								skinchangerActiveLineIndex = lineCount;
							skinchangerActiveLineIndex--;
							waitTime = 200;
						}
						else
							if (bDown)
							{
								menu_beep();
								skinchangerActiveLineIndex++;
								if (skinchangerActiveLineIndex == lineCount)
									skinchangerActiveLineIndex = 0;
								waitTime = 200;
							}
		if (skinchangerActiveLineIndex == (lineCount - 1))
			if (skinchangerActiveItemIndex >= itemCountLastLine)
				skinchangerActiveItemIndex = 0;
	}
	return false;
}

void spawnChauffer()
{
	int WaypointHandle = UI::GET_FIRST_BLIP_INFO_ID(8);
	if (UI::DOES_BLIP_EXIST(WaypointHandle))
	{
		Vector3 waypoint1 = UI::GET_BLIP_COORDS(WaypointHandle);
		STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("marshall"));
		while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("marshall"))) WAIT(0);
		Vector3 pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0);
		Vehicle veh = VEHICLE::CREATE_VEHICLE(GAMEPLAY::GET_HASH_KEY("marshall"), pos.x, pos.y, pos.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 1);
		if (veh != 0)
		{
			Ped Driver = PED::CREATE_RANDOM_PED_AS_DRIVER(veh, false);
			PED::SET_PED_INTO_VEHICLE(Driver, veh, -1);
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, 0);
			AI::TASK_VEHICLE_DRIVE_TO_COORD(Driver, veh, waypoint1.x, waypoint1.y, waypoint1.z, 40, 1, ENTITY::GET_ENTITY_MODEL(veh), 7, 6, -1);
			char* plateText = ("CRUSADER");
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plateText);
			set_status_text("~g~Chauffer Spawned!");
		}
	}
	else
		set_status_text("~r~Please choose a waypoint first.");
}

int teleportActiveLineIndex = 0;

bool process_teleport_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 20;

	std::string caption = "Teleport Options";

	static struct {
		LPCSTR  text;
		float x;
		float y;
		float z;
	} lines[lineCount] = {
		{ "Marker ~h~~o~>>" },
		{ "Micheal's House", -852.4f, 160.0f, 65.6f },
		{ "Franklin's House", 7.9f, 548.1f, 175.5f },
		{ "Trevor's Trailer", 1985.7f, 3812.2f, 32.2f },
		{ "Airport Enterance", -1034.6f, -2733.6f, 13.8f },
		{ "Aiport Field", -1336.0f, -3044.0f, 13.9f },
		{ "Elysian Island", 338.2f, -2715.9f, 38.5f },
		{ "Strip Club DJ Booth", 126.135F, -1278.583F, 29.270F },
		{ "Stripclub", 127.4f, -1307.7f, 29.2f },
		{ "Elburro Heights", 1384.0f, -2057.1f, 52.0f },
		{ "Ferris Wheel", -1670.7f, -1125.0f, 13.0f },
		{ "Chumash", -3192.6f, 1100.0f, 20.2f },
		{ "Windfarm", 2354.0f, 1830.3f, 101.1f },
		{ "Military Base", -2047.4f, 3132.1f, 32.8f },
		{ "Maze Bank", -75.015F, -818.215F, 326.176F },
		{ "Desert Airfield", 1747.0f, 3273.7f, 41.1f },
		{ "Chilliad", 425.4f, 5614.3f, 766.5f },
		{ "North Yankton", 3360.19f, -4849.67f, 111.8f },
		{ "Torture Warehouse", 136.514F, -2203.15F, 7.30914F },
		{ "Plane Crash", 2395.096F, 3049.616F, 60.053F },

	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != teleportActiveLineIndex)
					draw_menu_line(lines[i].text, lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(lines[teleportActiveLineIndex].text, lineWidth + 1.0, 2.0, 60.0 + teleportActiveLineIndex * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;


		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// get entity to teleport
			Entity e = PLAYER::PLAYER_PED_ID();
			if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
				e = PED::GET_VEHICLE_PED_IS_USING(e);

			// get coords
			Vector3 coords;
			bool success = false;
			if (teleportActiveLineIndex == 0) // marker
			{
				bool blipFound = false;
				// search for marker blip
				int blipIterator = UI::_GET_BLIP_INFO_ID_ITERATOR();
				for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(blipIterator); UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(blipIterator))
				{
					if (UI::GET_BLIP_INFO_ID_TYPE(i) == 4)
					{
						coords = UI::GET_BLIP_INFO_ID_COORD(i);
						blipFound = true;
						break;
					}
				}
				if (blipFound)
				{
					// load needed map region and check height levels for ground existence
					bool groundFound = false;
					static float groundCheckHeight[] = {
						100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
						450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
					};
					for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
						WAIT(100);
						if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z, false))
						{
							groundFound = true;
							coords.z += 3.0;
							break;
						}
					}
					// if ground not found then set Z in air and give player a parachute
					if (!groundFound)
					{
						coords.z = 1000.0;
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
					}
					success = true;
				}
				else
				{
					set_status_text("~r~ERROR: ~w~map marker isn't set");
				}

			}
			else // predefined coords
			{
				coords.x = lines[teleportActiveLineIndex].x;
				coords.y = lines[teleportActiveLineIndex].y;
				coords.z = lines[teleportActiveLineIndex].z;
				success = true;
			}

			// set player pos
			if (success)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
				STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
				GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
				GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
				WAIT(0);
				set_status_text("teleported!");
				return true;
			}

			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (teleportActiveLineIndex == 0)
						teleportActiveLineIndex = lineCount;
					teleportActiveLineIndex--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						teleportActiveLineIndex++;
						if (teleportActiveLineIndex == lineCount)
							teleportActiveLineIndex = 0;
						waitTime = 150;
					}
	}
	return false;
}

int activeLineIndexClothing = 0;
bool process_clothing_menu()
{


	Hash model1 = GAMEPLAY::GET_HASH_KEY("player_one");
	Hash model2 = GAMEPLAY::GET_HASH_KEY("player_zero");
	Hash model3 = GAMEPLAY::GET_HASH_KEY("player_two");


	const float lineWidth = 260.0;
	const int lineCount = 3;

	std::string caption = "Players";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Franklin" },
		{ "Micheal " },
		{ "Trevor" },

	};
	DWORD waitTime = 200;
	while (TRUE)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexClothing)
					draw_menu_line(lines[i].text, lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(lines[activeLineIndexClothing].text, lineWidth + 1.0, 2.0, 60.0 + activeLineIndexClothing * 24.0, 4.0, 7.0, true, false);
			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			menu_beep();
			switch (activeLineIndexClothing)
			{

			case 0:
				STREAMING::REQUEST_MODEL(model1);
				while (!STREAMING::HAS_MODEL_LOADED(model1))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model1);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model1);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 1:
				STREAMING::REQUEST_MODEL(model2);
				while (!STREAMING::HAS_MODEL_LOADED(model2))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model2);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model2);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 2:
				STREAMING::REQUEST_MODEL(model3);
				while (!STREAMING::HAS_MODEL_LOADED(model3))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model3);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model3);
				waitTime = 200;
				skinchanger_used = true;
				break;

			
				// switchable features
			default:
				if (lines[activeLineIndexClothing].pState)
					*lines[activeLineIndexClothing].pState = !(*lines[activeLineIndexClothing].pState);
				if (lines[activeLineIndexClothing].pUpdated)
					*lines[activeLineIndexClothing].pUpdated = TRUE;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexClothing == 0)
						activeLineIndexClothing = lineCount;
					activeLineIndexClothing--;
					waitTime = 200;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexClothing++;
						if (activeLineIndexClothing == lineCount)
							activeLineIndexClothing = 0;
						waitTime = 200;
					}
	}
}

std::string line_as_str(std::string text, bool *pState)
{
	while (text.size() < 18) text += " ";
	return text + (pState ? (*pState ? "~g~On" : "~r~Off") : "");
}

int activeLineIndexAnimals2 = 0;


bool process_animal_menu1()
{
	Hash model1 = GAMEPLAY::GET_HASH_KEY("a_c_rabbit_01");
	Hash model2 = GAMEPLAY::GET_HASH_KEY("a_c_rat");
	Hash model3 = GAMEPLAY::GET_HASH_KEY("a_c_retriever");
	Hash model4 = GAMEPLAY::GET_HASH_KEY("a_c_rhesus");
	Hash model5 = GAMEPLAY::GET_HASH_KEY("a_c_rottweiler");
	Hash model6 = GAMEPLAY::GET_HASH_KEY("a_c_seagull");
	Hash model7 = GAMEPLAY::GET_HASH_KEY("a_c_sharkhammer");
	Hash model8 = GAMEPLAY::GET_HASH_KEY("a_c_sharktiger");
	Hash model9 = GAMEPLAY::GET_HASH_KEY("a_c_shepherd");
	Hash model10 = GAMEPLAY::GET_HASH_KEY("a_c_stingray");
	Hash model11 = GAMEPLAY::GET_HASH_KEY("a_c_westy");



	const float lineWidth = 260.0;
	const int lineCount = 11;

	std::string caption = "Animals (2)";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Rabbit" },
		{ "Rat" },
		{ "Retriever" },
		{ "Rhesus Monkey" },
		{ "Rottweiler" },
		{ "Seagull" },
		{ "Sharkhammer" },
		{ "Sharktiger" },
		{ "Shepherd" },
		{ "Stingray" },
		{ "Westy" },
	

	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexAnimals2)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexAnimals2].text,
				lines[activeLineIndexAnimals2].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexAnimals2 * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexAnimals2)
			{

			case 0:
				STREAMING::REQUEST_MODEL(model1);
				while (!STREAMING::HAS_MODEL_LOADED(model1))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model1);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model1);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 1:
				STREAMING::REQUEST_MODEL(model2);
				while (!STREAMING::HAS_MODEL_LOADED(model2))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model2);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model2);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 2:
				STREAMING::REQUEST_MODEL(model3);
				while (!STREAMING::HAS_MODEL_LOADED(model3))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model3);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model3);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 3:
				STREAMING::REQUEST_MODEL(model4);
				while (!STREAMING::HAS_MODEL_LOADED(model4))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model4);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model3);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 4:
				STREAMING::REQUEST_MODEL(model5);
				while (!STREAMING::HAS_MODEL_LOADED(model5))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model5);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model3);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 5:
				STREAMING::REQUEST_MODEL(model6);
				while (!STREAMING::HAS_MODEL_LOADED(model6))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model6);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model3);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 6:
				STREAMING::REQUEST_MODEL(model7);
				while (!STREAMING::HAS_MODEL_LOADED(model7))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model7);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model3);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 7:
				STREAMING::REQUEST_MODEL(model8);
				while (!STREAMING::HAS_MODEL_LOADED(model8))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model8);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model3);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 8:
				STREAMING::REQUEST_MODEL(model9);
				while (!STREAMING::HAS_MODEL_LOADED(model9))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model9);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model3);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 9:
				STREAMING::REQUEST_MODEL(model10);
				while (!STREAMING::HAS_MODEL_LOADED(model10))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model10);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model3);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 10:
				STREAMING::REQUEST_MODEL(model11);
				while (!STREAMING::HAS_MODEL_LOADED(model11))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model11);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model3);
				waitTime = 200;
				skinchanger_used = true;
				break;



				// switchable features
			default:
				if (lines[activeLineIndexAnimals2].pState)
					*lines[activeLineIndexAnimals2].pState = !(*lines[activeLineIndexAnimals2].pState);
				if (lines[activeLineIndexAnimals2].pUpdated)
					*lines[activeLineIndexAnimals2].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexAnimals2 == 0)
						activeLineIndexAnimals2 = lineCount;
					activeLineIndexAnimals2--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexAnimals2++;
						if (activeLineIndexAnimals2 == lineCount)
							activeLineIndexAnimals2 = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexAnimals = 0;

bool process_animal_menu()
{
	Hash model1 = GAMEPLAY::GET_HASH_KEY("a_c_boar");
	Hash model2 = GAMEPLAY::GET_HASH_KEY("a_c_cat_01");
	Hash model3 = GAMEPLAY::GET_HASH_KEY("a_c_chickenhawk");
	Hash model4 = GAMEPLAY::GET_HASH_KEY("a_c_chimp");
	Hash model5 = GAMEPLAY::GET_HASH_KEY("a_c_chop");
	Hash model6 = GAMEPLAY::GET_HASH_KEY("a_c_cormorant");
	Hash model7 = GAMEPLAY::GET_HASH_KEY("a_c_cow");
	Hash model8 = GAMEPLAY::GET_HASH_KEY("a_c_coyote");
	Hash model9 = GAMEPLAY::GET_HASH_KEY("a_c_crow");
	Hash model10 = GAMEPLAY::GET_HASH_KEY("a_c_deer");
	Hash model11 = GAMEPLAY::GET_HASH_KEY("a_c_dolphin");
	Hash model12 = GAMEPLAY::GET_HASH_KEY("a_c_fish");
	Hash model13 = GAMEPLAY::GET_HASH_KEY("a_c_humpback");
	Hash model14 = GAMEPLAY::GET_HASH_KEY("a_c_husky");
	Hash model15 = GAMEPLAY::GET_HASH_KEY("a_c_killerwhale");
	Hash model16 = GAMEPLAY::GET_HASH_KEY("a_c_pig");
	Hash model17 = GAMEPLAY::GET_HASH_KEY("a_c_pigeon");
	Hash model18 = GAMEPLAY::GET_HASH_KEY("a_c_poodle");
	Hash model19 = GAMEPLAY::GET_HASH_KEY("a_c_pug");



	const float lineWidth = 260.0;
	const int lineCount = 20;

	std::string caption = "Animals";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Boar" },
		{ "Cat" },
		{ "Chickenhawk" },
		{ "Chimp" },
		{ "Chop" },
		{ "Cormorant" },
		{ "Cow" },
		{ "Coyote" },
		{ "Crow" },
		{ "Deer" },
		{ "Dolphin" },
		{ "Fish" },
		{ "Humpback" },
		{ "Husky" },
		{ "Killer Whale" },
		{ "Pig" },
		{ "Pigeon" },
		{ "Poodle" },
		{ "Pug" },
		{ "Next Page ~h~~o~>>" },



	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexAnimals)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexAnimals].text,
				lines[activeLineIndexAnimals].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexAnimals * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexAnimals)
			{

			case 0:
				STREAMING::REQUEST_MODEL(model1);
				while (!STREAMING::HAS_MODEL_LOADED(model1))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model1);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model1);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 1:
				STREAMING::REQUEST_MODEL(model2);
				while (!STREAMING::HAS_MODEL_LOADED(model2))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model2);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model2);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 2:
				STREAMING::REQUEST_MODEL(model3);
				while (!STREAMING::HAS_MODEL_LOADED(model3))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model3);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model3);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 3:
				STREAMING::REQUEST_MODEL(model4);
				while (!STREAMING::HAS_MODEL_LOADED(model4))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model4);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model4);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 4:
				STREAMING::REQUEST_MODEL(model5);
				while (!STREAMING::HAS_MODEL_LOADED(model5))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model5);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model5);
				waitTime = 200;
				break;
				skinchanger_used = true;
			case 5:
				STREAMING::REQUEST_MODEL(model6);
				while (!STREAMING::HAS_MODEL_LOADED(model6))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model6);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model6);
				waitTime = 200;
				break;
				skinchanger_used = true;
			case 6:
				STREAMING::REQUEST_MODEL(model7);
				while (!STREAMING::HAS_MODEL_LOADED(model7))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model7);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model8);
				waitTime = 200;
				break;
				skinchanger_used = true;
			case 7:
				STREAMING::REQUEST_MODEL(model8);
				while (!STREAMING::HAS_MODEL_LOADED(model8))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model8);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model8);
				waitTime = 200;
				break;
				skinchanger_used = true;
			case 8:
				STREAMING::REQUEST_MODEL(model9);
				while (!STREAMING::HAS_MODEL_LOADED(model9))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model9);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model9);
				waitTime = 200;
				break;
				skinchanger_used = true;
			case 9:
				STREAMING::REQUEST_MODEL(model10);
				while (!STREAMING::HAS_MODEL_LOADED(model10))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model10);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 10:
				STREAMING::REQUEST_MODEL(model11);
				while (!STREAMING::HAS_MODEL_LOADED(model11))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model11);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 11:
				STREAMING::REQUEST_MODEL(model12);
				while (!STREAMING::HAS_MODEL_LOADED(model12))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model12);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 12:
				STREAMING::REQUEST_MODEL(model13);
				while (!STREAMING::HAS_MODEL_LOADED(model13))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model13);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 13:
				STREAMING::REQUEST_MODEL(model14);
				while (!STREAMING::HAS_MODEL_LOADED(model14))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model14);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 14:
				STREAMING::REQUEST_MODEL(model15);
				while (!STREAMING::HAS_MODEL_LOADED(model15))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model15);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 15:
				STREAMING::REQUEST_MODEL(model16);
				while (!STREAMING::HAS_MODEL_LOADED(model16))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model16);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 16:
				STREAMING::REQUEST_MODEL(model17);
				while (!STREAMING::HAS_MODEL_LOADED(model17))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model17);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 17:
				STREAMING::REQUEST_MODEL(model18);
				while (!STREAMING::HAS_MODEL_LOADED(model18))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model18);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 18:
				STREAMING::REQUEST_MODEL(model19);
				while (!STREAMING::HAS_MODEL_LOADED(model19))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model19);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 19:
				if (process_animal_menu1()) return 0;
				break; 


				// switchable features
			default:
				if (lines[activeLineIndexAnimals].pState)
					*lines[activeLineIndexAnimals].pState = !(*lines[activeLineIndexAnimals].pState);
				if (lines[activeLineIndexAnimals].pUpdated)
					*lines[activeLineIndexAnimals].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexAnimals == 0)
						activeLineIndexAnimals = lineCount;
					activeLineIndexAnimals--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexAnimals++;
						if (activeLineIndexAnimals == lineCount)
							activeLineIndexAnimals = 0;
						waitTime = 150;
					}
	}
}


int activeLineIndexModel = 0;

bool process_model_menu()
{
	Hash model1 = GAMEPLAY::GET_HASH_KEY("u_m_y_babyd");
	Hash model2 = GAMEPLAY::GET_HASH_KEY("u_m_y_zombie_01");
	Hash model3 = GAMEPLAY::GET_HASH_KEY("u_m_y_pogo_01");
	Hash model4 = GAMEPLAY::GET_HASH_KEY("s_f_y_stripper_01");
	Hash model5 = GAMEPLAY::GET_HASH_KEY("u_m_m_jesus_01");
	Hash model6 = GAMEPLAY::GET_HASH_KEY("u_m_y_imporage");
	Hash model7 = GAMEPLAY::GET_HASH_KEY("a_f_y_topless_01");
	Hash model8 = GAMEPLAY::GET_HASH_KEY("s_m_y_swat_01");
	Hash model9 = GAMEPLAY::GET_HASH_KEY("s_m_y_mime");
	Hash model10 = GAMEPLAY::GET_HASH_KEY("s_m_m_movspace_01");
	Hash model11 = GAMEPLAY::GET_HASH_KEY("S_M_M_MovAlien_01");
	Hash model12 = GAMEPLAY::GET_HASH_KEY("u_m_m_prolsec_01");
	Hash model13 = GAMEPLAY::GET_HASH_KEY("u_m_m_streetart_01");
	Hash model14 = GAMEPLAY::GET_HASH_KEY("u_m_m_spyactor");
	Hash model15 = GAMEPLAY::GET_HASH_KEY("u_m_o_filmnoir");
	Hash model16 = GAMEPLAY::GET_HASH_KEY("u_f_m_drowned_01");
	Hash model17 = GAMEPLAY::GET_HASH_KEY("mp_m_freemode_01");
	Hash model18 = GAMEPLAY::GET_HASH_KEY("s_m_y_cop_01");
	Hash model19 = GAMEPLAY::GET_HASH_KEY("u_m_y_rsranger_01");



	const float lineWidth = 260.0;
	const int lineCount = 21;

	std::string caption = "Model Changer";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Animals ~h~~o~>>" },
		{ "Players ~h~~o~>>" },
		{ "Buff Dude" },
		{ "Zombie" },
		{ "Pogo" },
		{ "Stripper" },
		{ "Jesus" },
		{ "Impotent Rage" },
		{ "Topless Chick" },
		{ "Swat" },
		{ "Mime" },
		{ "Astronaut" },
		{ "Alien " },
		{ "Undead Cop" },
		{ "Street Artist" },
		{ "Spy" },
		{ "Filmnoir" },
		{ "Drowned" },
		{ "Free Mode" },
		{ "Cop" },
		{ "Space Ranger" },


	};


	DWORD waitTime = 150; 
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexModel)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexModel].text,
				lines[activeLineIndexModel].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexModel * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexModel)
			{
				// next radio track
			case 0:
				if (process_animal_menu()) return 0;
				break;
			case 1:
				if (process_clothing_menu()) return 0;
				break;
            case 2:
				STREAMING::REQUEST_MODEL(model1);
				while (!STREAMING::HAS_MODEL_LOADED(model1))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model1);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model1);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 3:
				STREAMING::REQUEST_MODEL(model2);
				while (!STREAMING::HAS_MODEL_LOADED(model2))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model2);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model2);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 4:
				STREAMING::REQUEST_MODEL(model3);
				while (!STREAMING::HAS_MODEL_LOADED(model3))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model3);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model3);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 5:
				STREAMING::REQUEST_MODEL(model4);
				while (!STREAMING::HAS_MODEL_LOADED(model4))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model4);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model4);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 6:
				STREAMING::REQUEST_MODEL(model5);
				while (!STREAMING::HAS_MODEL_LOADED(model5))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model5);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model5);
				waitTime = 200;
				break;
				skinchanger_used = true;
			case 7:
				STREAMING::REQUEST_MODEL(model6);
				while (!STREAMING::HAS_MODEL_LOADED(model6))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model6);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model6);
				waitTime = 200;
				break;
				skinchanger_used = true;
			case 8:
				STREAMING::REQUEST_MODEL(model7);
				while (!STREAMING::HAS_MODEL_LOADED(model7))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model7);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model8);
				waitTime = 200;
				break;
				skinchanger_used = true;
			case 9:
				STREAMING::REQUEST_MODEL(model8);
				while (!STREAMING::HAS_MODEL_LOADED(model8))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model8);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model8);
				waitTime = 200;
				break;
				skinchanger_used = true;
			case 10:
				STREAMING::REQUEST_MODEL(model9);
				while (!STREAMING::HAS_MODEL_LOADED(model9))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model9);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model9);
				waitTime = 200;
				break;
				skinchanger_used = true;
			case 11:
				STREAMING::REQUEST_MODEL(model10);
				while (!STREAMING::HAS_MODEL_LOADED(model10))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model10);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 12:
				STREAMING::REQUEST_MODEL(model11);
				while (!STREAMING::HAS_MODEL_LOADED(model11))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model11);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 13:
				STREAMING::REQUEST_MODEL(model12);
				while (!STREAMING::HAS_MODEL_LOADED(model12))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model12);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 14:
				STREAMING::REQUEST_MODEL(model13);
				while (!STREAMING::HAS_MODEL_LOADED(model13))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model13);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 15:
				STREAMING::REQUEST_MODEL(model14);
				while (!STREAMING::HAS_MODEL_LOADED(model14))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model14);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 16:
				STREAMING::REQUEST_MODEL(model15);
				while (!STREAMING::HAS_MODEL_LOADED(model15))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model15);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 17:
				STREAMING::REQUEST_MODEL(model16);
				while (!STREAMING::HAS_MODEL_LOADED(model16))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model16);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 18:
				STREAMING::REQUEST_MODEL(model17);
				while (!STREAMING::HAS_MODEL_LOADED(model17))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model17);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 19:
				STREAMING::REQUEST_MODEL(model18);
				while (!STREAMING::HAS_MODEL_LOADED(model18))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model18);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;
			case 20:
				STREAMING::REQUEST_MODEL(model19);
				while (!STREAMING::HAS_MODEL_LOADED(model19))	WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model19);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				WAIT(0);
				for (int i = 0; i < 12; i++)
					for (int j = 0; j < 100; j++)
					{
						int drawable = rand() % 10;
						int texture = rand() % 10;
						if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), i, drawable, texture))
						{
							PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, drawable, texture, 0);
							break;
						}
					}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model10);
				waitTime = 200;
				skinchanger_used = true;
				break;



				// switchable features
			default:
				if (lines[activeLineIndexModel].pState)
					*lines[activeLineIndexModel].pState = !(*lines[activeLineIndexModel].pState);
				if (lines[activeLineIndexModel].pUpdated)
					*lines[activeLineIndexModel].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexModel == 0)
						activeLineIndexModel = lineCount;
					activeLineIndexModel--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexModel++;
						if (activeLineIndexModel == lineCount)
							activeLineIndexModel = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexTake = 0;

bool process_take_menu()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 1);

	const float lineWidth = 260.0;
	const int lineCount = 10;

	std::string caption = "Take Money Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Take ~r~$1", NULL, NULL },
		{ "Take ~r~$100", NULL, NULL },
		{ "Take ~r~$1,000", NULL, NULL },
		{ "Take ~r~$10,000", NULL, NULL },
		{ "Take ~r~$100,000", NULL, NULL },
		{ "Take ~r~$1,000,000", NULL, NULL },
		{ "Take ~r~$5,000,000", NULL, NULL },
		{ "Take ~r~$10,000,000", NULL, NULL },
		{ "Take ~r~$100,000,000", NULL, NULL },
		{ "Take ~r~$200,000,000", NULL, NULL },




	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexTake)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexTake].text,
				lines[activeLineIndexTake].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexTake * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexTake)
			{
				// next radio track
			case 0:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val -= 1;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~r~-$1 ~w~Taken");
				break;
			case 1:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val -= 100;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~r~-$100 ~w~Taken");
				break;
			case 2:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val -= 1000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~r~-$1,000 ~w~Taken");
				break;
			case 3:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val -= 10000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~r~-$10,000 ~w~Taken");
				break;
			case 4:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val -= 100000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~r~-$100,000 ~w~Taken");
				break;
			case 5:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val -= 1000000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~r~-$1,000,000 ~w~Taken");
				break;
			case 6:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val -= 5000000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~r~-$5,000,000 ~w~Taken");
				break;
			case 7:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val -= 10000000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~r~-$10,000,000 ~w~Taken");
				break;
			case 8:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val -= 100000000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~r~-$100,000,000 ~w~Taken");
				break;
			case 9:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val -= 200000000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~r~-$200,000,000 ~w~Taken");
				break;
				// switchable features
			default:
				if (lines[activeLineIndexTake].pState)
					*lines[activeLineIndexTake].pState = !(*lines[activeLineIndexTake].pState);
				if (lines[activeLineIndexTake].pUpdated)
					*lines[activeLineIndexTake].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexTake == 0)
						activeLineIndexTake = lineCount;
					activeLineIndexTake--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexTake++;
						if (activeLineIndexTake == lineCount)
							activeLineIndexTake = 0;
						waitTime = 150;
					}
	}
}



int activeLineIndexAdd = 0;

bool process_add_menu()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 1);

	const float lineWidth = 260.0;
	const int lineCount = 10;

	std::string caption = "Add Money Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Add ~g~$1", NULL, NULL },
		{ "Add ~g~$100", NULL, NULL },
		{ "Add ~g~$1,000", NULL, NULL },
		{ "Add ~g~$10,000", NULL, NULL },
		{ "Add ~g~$100,000", NULL, NULL },
		{ "Add ~g~$1,000,000", NULL, NULL },
		{ "Add ~g~$5,000,000", NULL, NULL },
		{ "Add ~g~$10,000,000", NULL, NULL },
		{ "Add ~g~$100,000,000", NULL, NULL },
		{ "Add ~g~$200,000,000", NULL, NULL },




	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexAdd)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexAdd].text,
				lines[activeLineIndexAdd].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexAdd * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexAdd)
			{
				// next radio track
			case 0:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val += 1;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~g~+$1 ~w~Added");
				break;
			case 1:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val += 100;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~g~+$100 ~w~Added");
				break;
			case 2:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val += 1000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~g~+$1,000 ~w~Added");
				break;
			case 3:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val += 10000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~g~+$10,000 ~w~Added");
				break;
			case 4:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val += 100000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~g~+$100,000 ~w~Added");
				break;
			case 5:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val += 1000000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~g~+$1,000,000 ~w~Added");
				break;
			case 6:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val += 5000000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~g~+$5,000,000 ~w~Added");
				break;
			case 7:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val += 10000000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~g~+$10,000,000 ~w~Added");
				break;
			case 8:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val += 100000000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~g~+$100,000,000 ~w~Added");
				break;
			case 9:
				for (int i = 0; i < 3; i++)
				{
					char statNameFull[32];
					sprintf_s(statNameFull, "SP%d_TOTAL_CASH", i);
					Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
					int val;
					STATS::STAT_GET_INT(hash, &val, -1);
					val += 200000000;
					STATS::STAT_SET_INT(hash, val, 1);
				}
				set_status_text("~g~+$200,000,000 ~w~Added");
				break;
				// switchable features
			default:
				if (lines[activeLineIndexAdd].pState)
					*lines[activeLineIndexAdd].pState = !(*lines[activeLineIndexAdd].pState);
				if (lines[activeLineIndexAdd].pUpdated)
					*lines[activeLineIndexAdd].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexAdd == 0)
						activeLineIndexAdd = lineCount;
					activeLineIndexAdd--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexAdd++;
						if (activeLineIndexAdd == lineCount)
							activeLineIndexAdd = 0;
						waitTime = 150;
					}
	}
}




int activeLineIndexMoney = 0;

bool process_money_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 5;

	std::string caption = "Money options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Add Money Options ", NULL, NULL },
		{ "Take Money Options ", NULL, NULL },
		{ "2k Money Drop  ", &featurePlayermoneyDrop1, NULL },
		{ "Money Rain ", &featureRainMoney, NULL },
		{ "Give Player Money", &featurePlayermoneyDrop, NULL }
		

	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexMoney)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
					lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexMoney].text, lines[activeLineIndexMoney].pState),
				lineWidth + 1.0, 2.0, 60.0 + activeLineIndexMoney * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();


		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexMoney)
			{
				// next radio track
			case 0:
				if (process_add_menu())	return 0;
				break;
			case 1:
				if (process_take_menu()) return 0;
				break;
	
			default:
				if (lines[activeLineIndexMoney].pState)
					*lines[activeLineIndexMoney].pState = !(*lines[activeLineIndexMoney].pState);
				if (lines[activeLineIndexMoney].pUpdated)
					*lines[activeLineIndexMoney].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexMoney == 0)
						activeLineIndexMoney = lineCount;
					activeLineIndexMoney--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexMoney++;
						if (activeLineIndexMoney == lineCount)
							activeLineIndexMoney = 0;
						waitTime = 150;
					}
	}
}




int activeLineIndexPlayer = 0;

void process_player_menu()
{

	Player PLAYER_ID = PLAYER::PLAYER_ID();
	Ped PLAYER_PED_ID = PLAYER::PLAYER_PED_ID();
	BOOL PLAYER_EXIST = ENTITY::DOES_ENTITY_EXIST(PLAYER_PED_ID);
	Vehicle VEHICLE_ID = PED::GET_VEHICLE_PED_IS_USING(PLAYER_PED_ID);
	Vector3 ONLINE_PLAYER_COORDS = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER_ID, false);


	const float lineWidth = 260.0;
	const int lineCount = 19;

	std::string caption = "Player Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Model Changer ~h~~o~>>", NULL, NULL }, //0
		{ "Teleport options ~h~~o~>>", NULL, NULL }, //1
		{ "Money Options ~h~~o~>>", NULL, NULL }, //2 
		{ "Heal player", NULL, NULL },//3
		{ "Reset Skin", NULL, NULL },//4
		{ "Wanted Up", NULL, NULL },//5
		{ "Wanted Down", NULL, NULL },//6
		{ "Never Wanted", &featurePlayerNeverWanted, NULL },
		{ "God Mode", &featurePlayerInvincible, &featurePlayerInvincibleUpdated },
		{ "Police Ignored", &featurePlayerIgnored, &featurePlayerIgnoredUpdated },
		{ "Unlimited Ability", &featurePlayerUnlimitedAbility, NULL },
		{ "Noiseles", &featurePlayerNoNoise, &featurePlayerNoNoiseUpdated },
		{ "Fast Swim", &featurePlayerFastSwim, &featurePlayerFastSwimUpdated },
		{ "Super Jump", &featurePlayerSuperJump, NULL },
		{ "Fast Run", &featurePlayerFastRun, &featurePlayerFastRunUpdated },
		{ "Invisibility", &featurePlayerInvisible, &featurePlayerInvisibleUpdated },
		{ "Mega Jump", &featurePlayerUltraJump, NULL },
		{ "Superman Mode", &featureMiscFlyMode, NULL },
		{ "Mega Run", &featurePlayerSuperRun, NULL },

	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexPlayer)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
					lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexPlayer].text, lines[activeLineIndexPlayer].pState),
				lineWidth + 1.0, 2.0, 60.0 + activeLineIndexPlayer * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexPlayer)
			{
				// skin changer
			case 0:
				if (process_model_menu())	return;
				break;
				// teleport
			case 1:
				if (process_teleport_menu()) return;
				break;
				// fix player
			case 2:
				if (process_money_menu()) return;
				break;

			case 3:
			{
				ENTITY::SET_ENTITY_HEALTH(playerPed, ENTITY::GET_ENTITY_MAX_HEALTH(playerPed));
				PED::ADD_ARMOUR_TO_PED(playerPed, PLAYER::GET_PLAYER_MAX_ARMOUR(player) - PED::GET_PED_ARMOUR(playerPed));
				if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
					if (ENTITY::DOES_ENTITY_EXIST(playerVeh) && !ENTITY::IS_ENTITY_DEAD(playerVeh))
						VEHICLE::SET_VEHICLE_FIXED(playerVeh);
				}
				set_status_text("player fixed");
			}
			break;
			// reset model skin
			case 4:
			{
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(playerPed);
				set_status_text("using default model skin");
			}
			break;

			case 5:
				if (bPlayerExists && PLAYER::GET_PLAYER_WANTED_LEVEL(player) < 5)
				{
					PLAYER::SET_PLAYER_WANTED_LEVEL(player, PLAYER::GET_PLAYER_WANTED_LEVEL(player) + 1, 0);
					PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
					set_status_text("wanted up");
				}
				break;
				// wanted down
			case 6:
				if (bPlayerExists && PLAYER::GET_PLAYER_WANTED_LEVEL(player) > 0)
				{
					PLAYER::SET_PLAYER_WANTED_LEVEL(player, PLAYER::GET_PLAYER_WANTED_LEVEL(player) - 1, 0);
					PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
					set_status_text("wanted down");
				}
				break;
				// switchable features
			default:
				if (lines[activeLineIndexPlayer].pState)
					*lines[activeLineIndexPlayer].pState = !(*lines[activeLineIndexPlayer].pState);
				if (lines[activeLineIndexPlayer].pUpdated)
					*lines[activeLineIndexPlayer].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexPlayer == 0)
						activeLineIndexPlayer = lineCount;
					activeLineIndexPlayer--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexPlayer++;
						if (activeLineIndexPlayer == lineCount)
							activeLineIndexPlayer = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexWeapon = 0;

void process_weapon_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 14;

	std::string caption = "Weapon Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Get All Weapons", NULL, NULL },
		{ "Input Weapon", NULL, NULL },
		{ "Inf Amo", &featureWeaponNoReload, NULL },
		{ "Fire Amo", &featureWeaponFireAmmo, NULL },
		{ "Explosive Ammo", &featureWeaponExplosiveAmmo, NULL },
		{ "Explosive Melee", &featureWeaponExplosiveMelee, NULL },
		{ "One Hit Kill", &featureWeaponOneHit, NULL },
		{ "Rainbow Camo", &featureWeaponRainbowTint, NULL },
		{ "Money Gun", &featureWeaponMoneyGun, NULL },
		{ "Firework Gun", &featureWeaponFirework, NULL },
		{ "Rocket Gun", &featureWeaponRPG, NULL },
		{ "Whale Gun", &featureWeaponWhaleGun, NULL },
		{ "Vehicle Gun", &featureWepModVehGun, NULL },
		{ "Animal Gun", &featureWepModAnimalGun, NULL },
		


	};

	static LPCSTR weaponNames[] = {
		"WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR",
		"WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_MICROSMG", "WEAPON_SMG",
		"WEAPON_ASSAULTSMG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_MG",
		"WEAPON_COMBATMG", "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_BULLPUPSHOTGUN",
		"WEAPON_STUNGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_GRENADELAUNCHER", "WEAPON_GRENADELAUNCHER_SMOKE",
		"WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_SMOKEGRENADE", "WEAPON_BZGAS",
		"WEAPON_MOLOTOV", "WEAPON_FIREEXTINGUISHER", "WEAPON_PETROLCAN",
		"WEAPON_SNSPISTOL", "WEAPON_SPECIALCARBINE", "WEAPON_HEAVYPISTOL", "WEAPON_BULLPUPRIFLE", "WEAPON_HOMINGLAUNCHER",
		"WEAPON_PROXMINE", "WEAPON_SNOWBALL", "WEAPON_VINTAGEPISTOL", "WEAPON_DAGGER", "WEAPON_FIREWORK", "WEAPON_MUSKET",
		"WEAPON_MARKSMANRIFLE", "WEAPON_HEAVYSHOTGUN", "WEAPON_GUSENBERG", "WEAPON_HATCHET", "WEAPON_RAILGUN",
		"WEAPON_COMBATPDW", "WEAPON_KNUCKLE", "WEAPON_MARKSMANPISTOL",
		"WEAPON_FLASHLIGHT", "WEAPON_MACHETE", "WEAPON_MACHINEPISTOL",
		"WEAPON_SWITCHBLADE", "WEAPON_REVOLVER", "WEAPON_COMPACTRIFLE", "weapon_flaregun",
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexWeapon)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
					lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexWeapon].text, lines[activeLineIndexWeapon].pState),
				lineWidth + 1.0, 2.0, 60.0 + activeLineIndexWeapon * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexWeapon)
			{
			case 0:
				for (int i = 0; i < sizeof(weaponNames) / sizeof(weaponNames[0]); i++)
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), 1000, 0);
				set_status_text("all weapons added");
				break;
			case 1:
				if (bPlayerExists)
				{
					GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(TRUE, "FMMC_KEY_TIP8", "", "WEAPON_COMPACTRIFLE", "", "", "", 25);
					while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
					if (!WEAPON::IS_WEAPON_VALID(GAMEPLAY::GET_HASH_KEY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())))
					{
						std::string playerName = PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID());
						std::string text = " ~r~that was a invalid weapon hash.";
						set_status_text(playerName + text);
						return;
					}
				}
				// switchable features
			default:
				if (lines[activeLineIndexWeapon].pState)
					*lines[activeLineIndexWeapon].pState = !(*lines[activeLineIndexWeapon].pState);
				if (lines[activeLineIndexWeapon].pUpdated)
					*lines[activeLineIndexWeapon].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexWeapon == 0)
						activeLineIndexWeapon = lineCount;
					activeLineIndexWeapon--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexWeapon++;
						if (activeLineIndexWeapon == lineCount)
							activeLineIndexWeapon = 0;
						waitTime = 150;
					}
	}
}

LPCSTR vehicleModels[43][10] = {
	{ "NINEF", "NINEF2", "BLISTA", "ASEA", "ASEA2", "BOATTRAILER", "BUS", "ARMYTANKER", "ARMYTRAILER", "ARMYTRAILER2" },
	{ "SUNTRAP", "COACH", "AIRBUS", "ASTEROPE", "AIRTUG", "AMBULANCE", "BARRACKS", "BARRACKS2", "BALLER", "BALLER2" },
	{ "BJXL", "BANSHEE", "BENSON", "BFINJECTION", "BIFF", "BLAZER", "BLAZER2", "BLAZER3", "BISON", "BISON2" },
	{ "BISON3", "BOXVILLE", "BOXVILLE2", "BOXVILLE3", "BOBCATXL", "BODHI2", "BUCCANEER", "BUFFALO", "BUFFALO2", "BULLDOZER" },
	{ "BULLET", "BLIMP", "BURRITO", "BURRITO2", "BURRITO3", "BURRITO4", "BURRITO5", "CAVALCADE", "CAVALCADE2", "POLICET" },
	{ "GBURRITO", "CABLECAR", "CADDY", "CADDY2", "CAMPER", "CARBONIZZARE", "CHEETAH", "COMET2", "COGCABRIO", "COQUETTE" },
	{ "CUTTER", "GRESLEY", "DILETTANTE", "DILETTANTE2", "DUNE", "DUNE2", "HOTKNIFE", "DLOADER", "DUBSTA", "DUBSTA2" },
	{ "DUMP", "RUBBLE", "DOCKTUG", "DOMINATOR", "EMPEROR", "EMPEROR2", "EMPEROR3", "ENTITYXF", "EXEMPLAR", "ELEGY2" },
	{ "F620", "FBI", "FBI2", "FELON", "FELON2", "FELTZER2", "FIRETRUK", "FLATBED", "FORKLIFT", "FQ2" },
	{ "FUSILADE", "FUGITIVE", "FUTO", "GRANGER", "GAUNTLET", "HABANERO", "HAULER", "HANDLER", "INFERNUS", "INGOT" },
	{ "INTRUDER", "ISSI2", "JACKAL", "JOURNEY", "JB700", "KHAMELION", "LANDSTALKER", "LGUARD", "MANANA", "MESA" },
	{ "MESA2", "MESA3", "CRUSADER", "MINIVAN", "MIXER", "MIXER2", "MONROE", "MOWER", "MULE", "MULE2" },
	{ "ORACLE", "ORACLE2", "PACKER", "PATRIOT", "PBUS", "PENUMBRA", "PEYOTE", "PHANTOM", "PHOENIX", "PICADOR" },
	{ "POUNDER", "POLICE", "POLICE4", "POLICE2", "POLICE3", "POLICEOLD1", "POLICEOLD2", "PONY", "PONY2", "PRAIRIE" },
	{ "PRANGER", "PREMIER", "PRIMO", "PROPTRAILER", "RANCHERXL", "RANCHERXL2", "RAPIDGT", "RAPIDGT2", "RADI", "RATLOADER" },
	{ "REBEL", "REGINA", "REBEL2", "RENTALBUS", "RUINER", "RUMPO", "RUMPO2", "RHINO", "RIOT", "RIPLEY" },
	{ "ROCOTO", "ROMERO", "SABREGT", "SADLER", "SADLER2", "SANDKING", "SANDKING2", "SCHAFTER2", "SCHWARZER", "SCRAP" },
	{ "SEMINOLE", "SENTINEL", "SENTINEL2", "ZION", "ZION2", "SERRANO", "SHERIFF", "SHERIFF2", "SPEEDO", "SPEEDO2" },
	{ "STANIER", "STINGER", "STINGERGT", "STOCKADE", "STOCKADE3", "STRATUM", "SULTAN", "SUPERD", "SURANO", "SURFER" },
	{ "SURFER2", "SURGE", "TACO", "TAILGATER", "TAXI", "TRASH", "TRACTOR", "TRACTOR2", "TRACTOR3", "GRAINTRAILER" },
	{ "BALETRAILER", "TIPTRUCK", "TIPTRUCK2", "TORNADO", "TORNADO2", "TORNADO3", "TORNADO4", "TOURBUS", "TOWTRUCK", "TOWTRUCK2" },
	{ "UTILLITRUCK", "UTILLITRUCK2", "UTILLITRUCK3", "VOODOO2", "WASHINGTON", "STRETCH", "YOUGA", "ZTYPE", "SANCHEZ", "SANCHEZ2" },
	{ "SCORCHER", "TRIBIKE", "TRIBIKE2", "TRIBIKE3", "FIXTER", "CRUISER", "BMX", "POLICEB", "AKUMA", "CARBONRS" },
	{ "BAGGER", "BATI", "BATI2", "RUFFIAN", "DAEMON", "DOUBLE", "PCJ", "VADER", "VIGERO", "FAGGIO2" },
	{ "HEXER", "ANNIHILATOR", "BUZZARD", "BUZZARD2", "CARGOBOB", "CARGOBOB2", "CARGOBOB3", "SKYLIFT", "POLMAV", "MAVERICK" },
	{ "NEMESIS", "FROGGER", "FROGGER2", "CUBAN800", "DUSTER", "STUNT", "MAMMATUS", "JET", "SHAMAL", "LUXOR" },
	{ "TITAN", "LAZER", "CARGOPLANE", "SQUALO", "MARQUIS", "DINGHY", "DINGHY2", "JETMAX", "PREDATOR", "TROPIC" },
	{ "SEASHARK", "SEASHARK2", "SUBMERSIBLE", "TRAILERS", "TRAILERS2", "TRAILERS3", "TVTRAILER", "RAKETRAILER", "TANKER", "TRAILERLOGS" },
	{ "TR2", "TR3", "TR4", "TRFLAT", "TRAILERSMALL", "VELUM", "ADDER", "VOLTIC", "VACCA", "BIFTA" },
	{ "SPEEDER", "PARADISE", "KALAHARI", "JESTER", "TURISMOR", "VESTRA", "ALPHA", "HUNTLEY", "THRUST", "MASSACRO" },
	{ "MASSACRO2", "ZENTORNO", "BLADE", "GLENDALE", "PANTO", "PIGALLE", "WARRENER", "RHAPSODY", "DUBSTA3", "MONSTER" },
	{ "SOVEREIGN", "INNOVATION", "HAKUCHOU", "FUROREGT", "MILJET", "COQUETTE2", "BTYPE", "BUFFALO3", "DOMINATOR2", "GAUNTLET2" },
	{ "MARSHALL", "DUKES", "DUKES2", "STALION", "STALION2", "BLISTA2", "BLISTA3", "DODO", "SUBMERSIBLE2", "HYDRA" },
	{ "INSURGENT", "INSURGENT2", "TECHNICAL", "SAVAGE", "VALKYRIE", "KURUMA", "KURUMA2", "JESTER2", "CASCO", "VELUM2" },
	{ "GUARDIAN", "ENDURO", "LECTRO", "SLAMVAN", "SLAMVAN2", "RATLOADER2", "SWIFT2", "LUXOR2", "FELTZER3", "OSIRIS" },
	{ "VIRGO", "WINDSOR", "BESRA", "SWIFT", "BLIMP2", "VINDICATOR", "TORO", "T20", "COQUETTE3", "CHINO" },
	{ "BRAWLER", "BUCCANEER2", "CHINO2", "FACTION", "FACTION2", "MOONBEAM", "MOONBEAM2", "PRIMO2", "VOODOO", "LURCHER" },
	{ "BTYPE2", "BALLER3", "BALLER4", "BALLER5", "BALLER6", "CARGOBOB4", "COG55", "COG552", "COGNOSCENTI", "COGNOSCENTI2" },
	{ "DINGHY4", "LIMO2", "MAMBA", "NIGHTSHADE", "SCHAFTER3", "SCHAFTER4", "SCHAFTER5", "SCHAFTER6", "SEASHARK3", "SPEEDER2" },
	{ "SUPERVOLITO", "SUPERVOLITO2", "TORO2", "TROPIC2", "VALKYRIE2", "VERLIERER2", "TAMPA", "BANSHEE2", "SULTANRS", "BTYPE3" },
	{ "BF400", "BRIOSO", "CLIFFHANGER", "CONTENDER", "GARGOYLE", "LE7B", "LYNX", "OMNIS", "RALLYTRUCK", "SHEAVA" },
	{ "TAMPA2", "TROPHYTRUCK", "TROPHYTRUCK", "TROPOS", "BMX", "BESTIAGTS", "BRICKADE", "FMJ", "NIMBUS", "PFISTER811", },
	{ "PROTOTIPO", "REAPER", "RUMPO3", "SEVEN70", "TUG", "VOLATUS", "WINDSOR2", "XLS", "XLS2", "", }
};

int carspawnActiveLineIndex = 0;
int carspawnActiveItemIndex = 0;

bool process_carspawn_menu()
{
	DWORD waitTime = 150;
	const int lineCount = 43;
	const int itemCount = 10;
	const int itemCountLastLine = 9;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			char caption[32];
			sprintf_s(caption, "Car Spawner   %d / %d", carspawnActiveLineIndex + 1, lineCount);
			draw_menu_line(caption, 350.0, 15.0, 18.0, 0.0, 5.0, false, true);
			for (int i = 0; i < itemCount; i++)
				if (strlen(vehicleModels[carspawnActiveLineIndex][i]))
					draw_menu_line(vehicleModels[carspawnActiveLineIndex][i], 100.0, 5.0, 200.0, 100.0 + i * 110.0, 5.0, i == carspawnActiveItemIndex, false, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		bool bSelect, bBack, bUp, bDown, bLeft, bRight;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, &bLeft, &bRight);

		if (bSelect)
		{
			menu_beep();
			LPCSTR modelName = vehicleModels[carspawnActiveLineIndex][carspawnActiveItemIndex];
			DWORD model = GAMEPLAY::GET_HASH_KEY((char *)modelName);
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
			{
				STREAMING::REQUEST_MODEL(model);
				while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
				Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0);
				Vehicle veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, 0.0, 1, 1);
				STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2"); GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2"); GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, false, false, false);
				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);

				if (featureVehWrapInSpawned)
				{
					ENTITY::SET_ENTITY_HEADING(veh, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
					PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
					STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
					GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
					GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
				}

				WAIT(0);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
				ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);

				char statusText[32];
				sprintf_s(statusText, "%s spawned", modelName);
				set_status_text(statusText);

				return true;
			}
		}
		else
			if (bBack)
			{
				menu_beep();
				break;
			}
			else
				if (bRight)
				{
					menu_beep();
					carspawnActiveItemIndex++;
					int itemsMax = (carspawnActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
					if (carspawnActiveItemIndex == itemsMax)
						carspawnActiveItemIndex = 0;
					waitTime = 100;
				}
				else
					if (bLeft)
					{
						menu_beep();
						if (carspawnActiveItemIndex == 0)
							carspawnActiveItemIndex = (carspawnActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
						carspawnActiveItemIndex--;
						waitTime = 100;
					}
					else
						if (bUp)
						{
							menu_beep();
							if (carspawnActiveLineIndex == 0)
								carspawnActiveLineIndex = lineCount;
							carspawnActiveLineIndex--;
							waitTime = 200;
						}
						else
							if (bDown)
							{
								menu_beep();
								carspawnActiveLineIndex++;
								if (carspawnActiveLineIndex == lineCount)
									carspawnActiveLineIndex = 0;
								waitTime = 200;
							}
		if (carspawnActiveLineIndex == (lineCount - 1))
			if (carspawnActiveItemIndex >= itemCountLastLine)
				carspawnActiveItemIndex = 0;
	}
	return false;
}



int activeLineIndexVehCust = 0;

bool process_veh_menu_cust()
{
	const float lineWidth = 260.0;
	const int lineCount = 18;

	std::string caption = "Car Customization Menu";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "All Performance Upgrades", NULL, NULL },
		{ "All Armor Upgrades", NULL, NULL },
		{ "Change Suspension", NULL, NULL },
		{ "Change Spoiler", NULL, NULL },
		{ "Change Exhaust", NULL, NULL },
		{ "Change Hood", NULL, NULL },
		{ "Change Front Bumper", NULL, NULL },
		{ "Change Rear Bumper", NULL, NULL },
		{ "Change Grille", NULL, NULL },
		{ "Change Side Skirts", NULL, NULL },
		{ "Change Roof", NULL, NULL },
		{ "Change Window Tint", NULL, NULL },
		{ "Change Fenders", NULL, NULL },
		{ "Change Rollcage", NULL, NULL },
		{ "Change License Plate Type", NULL, NULL },
		{ "Add Xenon Headlights", NULL, NULL },
		{ "Change Wheel Category", NULL, NULL },
		{ "Change Wheels", NULL, NULL },
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexVehCust)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
					lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexVehCust].text, lines[activeLineIndexVehCust].pState),
				lineWidth + 1.0, 2.0, 60.0 + activeLineIndexVehCust * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexVehCust)
			{
			case 0: //Upgrade Performance
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						VEHICLE::SET_VEHICLE_MOD(veh, 11, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 11) - 1, 1); 
						VEHICLE::SET_VEHICLE_MOD(veh, 12, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 12) - 1, 1); 
						VEHICLE::SET_VEHICLE_MOD(veh, 13, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 13) - 1, 1); 
						VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, 1); 
						set_status_text("Added all Performance Upgrades");
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 1: //Upgrade Armor and Tires
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						VEHICLE::SET_VEHICLE_MOD(veh, 16, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 16) - 1, 1); 
						VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 0); 
						set_status_text("Added all Armor Upgrades and Bulletproof Tires");
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 2: //Upgrade Suspension
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 15) - 1;
						int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 15);
						if (currmod < nummods)
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 15, currmod + 1, 1); 
							set_status_text("Suspension Upgraded");
						}
						else
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 15, -1, 1); 
							set_status_text("Default Suspension");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 3: //Change Spoiler
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 0) - 1;
						int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 0);
						if (currmod < nummods)
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 0, currmod + 1, 1); 
							set_status_text("Changed Spoiler");
						}
						else
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 0, -1, 1); 
							set_status_text("Default Spoiler");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 4: //Change Exhaust
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 4) - 1;
						int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 4);
						if (currmod < nummods)
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 4, currmod + 1, 1); 
							set_status_text("Changed Exhaust");
						}
						else
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 4, -1, 1); 
							set_status_text("Default Exhaust");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 5: //Change Hood
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 7) - 1;
						int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 7);
						if (currmod < nummods)
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 7, currmod + 1, 1); 
							set_status_text("Changed Hood");
						}
						else
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 7, -1, 1); 
							set_status_text("Default Hood");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 6: //Change Front Bumper
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 1) - 1;
						int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 1);
						if (currmod < nummods)
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 1, currmod + 1, 1); 
							set_status_text("Changed Front Bumper");
						}
						else
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 1, -1, 1); 
							set_status_text("Default Front Bumper");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 7: //Change Rear Bumper
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 2) - 1;
						int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 2);
						if (currmod < nummods)
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 2, currmod + 1, 1); 
							set_status_text("Changed Rear Bumper");
						}
						else
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 2, -1, 1); 
							set_status_text("Default Rear Bumper");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 8: //Change Grille
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 6) - 1;
						int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 6);
						if (currmod < nummods)
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 6, currmod + 1, 1); 
							set_status_text("Changed Grille");
						}
						else
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 6, -1, 1); 
							set_status_text("Default Grille");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 9: //Change Side Skirts
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 3) - 1;
						int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 3);
						if (currmod < nummods)
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 3, currmod + 1, 1); 
							set_status_text("Changed Side Skirts");
						}
						else
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 3, -1, 1); 
							set_status_text("Default Side Skirts");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 10: //Change Roof
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 10) - 1;
						int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 10);
						if (currmod < nummods)
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 10, currmod + 1, 1); 
							set_status_text("Changed Roof");
						}
						else
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 10, -1, 1); 
							set_status_text("Default Roof");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 11: //Change Window Tint
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int currmod = VEHICLE::GET_VEHICLE_WINDOW_TINT(veh);
						if (currmod < 3)
						{
							VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, currmod + 1); 
							set_status_text("Changed Window Tint");
						}
						else
						{
							VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 0);
							set_status_text("Default Window Tint");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 12: //Change Fenders
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 8) - 1;
						int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 8);
						if (currmod < nummods)
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 8, currmod + 1, 1); 
							set_status_text("Changed Fenders");
						}
						else
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 8, -1, 1); 
							set_status_text("Default Fenders");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 13: //Change Rollcage
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 5) - 1;
						int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 5);
						if (currmod < nummods)
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 5, currmod + 1, 1); 
							set_status_text("Changed Rollcage");
						}
						else
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 5, -1, 1); 
							set_status_text("Default Rollcage");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 14: 
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int currmod = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh);
						if (currmod < 5)
						{
							VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, currmod + 1); 
						}
						else
						{
							VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, 0); 
						}
						set_status_text("Changed License Plate");
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 15: //Xenon Headlights
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, 1);
						set_status_text("Added Xenon Headlights");
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 16: //Change Wheel Category
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int currmod = VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh);
						if (PED::IS_PED_ON_ANY_BIKE(playerPed))
						{
							set_status_text("Bikes only have One Wheel Category");
						}
						else if (currmod < 5)
						{
							VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, currmod + 1); 
							VEHICLE::SET_VEHICLE_MOD(veh, 23, 1, 0); 
							set_status_text("Changed Wheel Category");
						}
						else if (currmod == 5)
						{
							VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, currmod + 2); 
							VEHICLE::SET_VEHICLE_MOD(veh, 23, 1, 0); 
							set_status_text("Changed Wheel Category");
						}
						else
						{
							VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, 0); 
							VEHICLE::SET_VEHICLE_MOD(veh, 23, 1, 0); 
							set_status_text("Changed Wheel Category");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

			case 17: //Change Wheels 
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						int nummods = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 23) - 1;
						int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 23);
						if (currmod < nummods)
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 23, currmod + 1, 0); 
							VEHICLE::SET_VEHICLE_MOD(veh, 24, currmod + 1, 0); 
							set_status_text("Changed Wheels");
						}
						else
						{
							VEHICLE::SET_VEHICLE_MOD(veh, 23, -1, 0); 
							VEHICLE::SET_VEHICLE_MOD(veh, 24, -1, 0); 
							set_status_text("Default Wheels");
						}
					}
					else
					{
						set_status_text("Player isn't in a vehicle");
					}
				}
				break;

				// switchable features
			default:
				if (lines[activeLineIndexVehCust].pState)
					*lines[activeLineIndexVehCust].pState = !(*lines[activeLineIndexVehCust].pState);
				if (lines[activeLineIndexVehCust].pUpdated)
					*lines[activeLineIndexVehCust].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexVehCust == 0)
						activeLineIndexVehCust = lineCount;
					activeLineIndexVehCust--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexVehCust++;
						if (activeLineIndexVehCust == lineCount)
							activeLineIndexVehCust = 0;
						waitTime = 150;
					}
	}
	return 0;
}


int activeLineIndexSec = 0;

bool process_paint_menu_Sec()
{
	const float lineWidth = 260.0;
	const int lineCount = 9;

	std::string caption = "Paint Menu";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Primary Colour ~h~~o~>>", NULL, NULL },
		{ "Secondary Colour ~h~~o~>>", NULL, NULL },

	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexSec)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
					lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexSec].text, lines[activeLineIndexSec].pState),
				lineWidth + 1.0, 2.0, 60.0 + activeLineIndexSec * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexSec)
			{
				// next radio track
			case 0:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) &&
					PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
					AUDIO::SKIP_RADIO_FORWARD();
				break;
				// switchable features
			default:
				if (lines[activeLineIndexSec].pState)
					*lines[activeLineIndexSec].pState = !(*lines[activeLineIndexSec].pState);
				if (lines[activeLineIndexSec].pUpdated)
					*lines[activeLineIndexSec].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexSec == 0)
						activeLineIndexSec = lineCount;
					activeLineIndexSec--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexSec++;
						if (activeLineIndexSec == lineCount)
							activeLineIndexSec = 0;
						waitTime = 150;
					}
	}
}


int activeLineIndexPrimBright = 0;

bool process_paint_menu_Prim_Bright()
{

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());


	const float lineWidth = 260.0;
	const int lineCount = 17;

	std::string caption = "Bright Colours";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Electric Lime", NULL, NULL },
		{ "Fluoresent Yellow", NULL, NULL },
		{ "Fuchsia", NULL, NULL },
		{ "Turquoise", NULL, NULL },
		{ "Silver", NULL, NULL },
		{ "Hot Pink", NULL, NULL },
		{ "Lime Green", NULL, NULL },
		{ "Purple", NULL, NULL },
		{ "Light Blue", NULL, NULL },
		{ "Midnight Blue", NULL, NULL },
		{ "Aventador Orange", NULL, NULL },
		{ "Cream", NULL, NULL },
		{ "White", NULL, NULL },
		{ "Green", NULL, NULL },
		{ "Gold", NULL, NULL },
		{ "Gray", NULL, NULL },
		{ "~o~R~y~a~r~n~p~d~b~o~g~m", NULL, NULL },





	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexPrimBright)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
					lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexPrimBright].text, lines[activeLineIndexPrimBright].pState),
				lineWidth + 1.0, 2.0, 60.0 + activeLineIndexPrimBright * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexPrimBright)
			{
				// next radio track
			case 0:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 204, 255, 00);
					return 0;
				}
			case 1:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 204, 255, 0);
					return 0;
				}
			case 2:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 255, 0, 205);
					return 0;
				}
			case 3:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 255, 255, 0);
					return 0;
				}
			case 4:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 192, 192, 192);
					return 0;
				}
			case 5:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 255, 0, 255);
					return 0;
				}
			case 6:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 134, 236, 0);
					return 0;
				}
			case 7:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 137, 0, 255);
					return 0;
				}
			case 8:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 0, 191, 255);
					return 0;
				}
			case 9:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 25, 25, 112);
					return 0;
				}
			case 10:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 255, 69, 0);
					return 0;
				}
			case 11:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 255, 253, 208);
					return 0;
				}
			case 12:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 255, 255, 255);
					return 0;
				}
			case 13:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 43, 183, 62);
					return 0;
				}
			case 14:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 122, 86, 3);
					return 0;
				}
			case 15:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 128, 128, 128);
					return 0;
				}
			case 16:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
					set_status_text("~r~WARNING: ~w~Can mess up setting primary and secondary colours!");
				}

				// switchable features
			default:
				if (lines[activeLineIndexPrimBright].pState)
					*lines[activeLineIndexPrimBright].pState = !(*lines[activeLineIndexPrimBright].pState);
				if (lines[activeLineIndexPrimBright].pUpdated)
					*lines[activeLineIndexPrimBright].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexPrimBright == 0)
						activeLineIndexPrimBright = lineCount;
					activeLineIndexPrimBright--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexPrimBright++;
						if (activeLineIndexPrimBright == lineCount)
							activeLineIndexPrimBright = 0;
						waitTime = 150;
					}
	}
}


int activeLineIndexPrimMatte = 0;

bool process_paint_menu_Prim_Matte()
{

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());


	const float lineWidth = 260.0;
	const int lineCount = 18;

	std::string caption = "Secondary Colours";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Black", NULL, NULL },
		{ "Red", NULL, NULL },
		{ "Blue", NULL, NULL },
		{ "Race Yellow", NULL, NULL },
		{ "Silver", NULL, NULL },
		{ "Hot Pink", NULL, NULL },
		{ "Lime Green", NULL, NULL },
		{ "Purple", NULL, NULL },
		{ "Light Blue", NULL, NULL },
		{ "Midnight Blue", NULL, NULL },
		{ "Aventador Orange", NULL, NULL },
		{ "Cream", NULL, NULL },
		{ "White", NULL, NULL },
		{ "Green", NULL, NULL },
		{ "Gold", NULL, NULL },
		{ "Gray", NULL, NULL },
		{ "Fluoresent Yellow", NULL, NULL },
		{ "~o~R~y~a~r~n~p~d~b~o~g~m", NULL, NULL },





	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexPrimMatte)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexPrimMatte].text,
				lines[activeLineIndexPrimMatte].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexPrimMatte * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexPrimMatte)
			{
				// next radio track
			case 0:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 0, 0, 0);
					return 0;
				}
			case 1:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 218, 53, 53);
					return 0;
				}
			case 2:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 0, 0, 205);
					return 0;
				}
			case 3:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 255, 255, 0);
					return 0;
				}
			case 4:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 192, 192, 192);
					return 0;
				}
			case 5:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 255, 0, 255);
					return 0;
				}
			case 6:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 134, 236, 0);
					return 0;
				}
			case 7:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 137, 0, 255);
					return 0;
				}
			case 8:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 0, 191, 255);
					return 0;
				}
			case 9:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 25, 25, 112);
					return 0;
				}
			case 10:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 255, 69, 0);
					return 0;
				}
			case 11:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 255, 253, 208);
					return 0;
				}
			case 12:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 255, 255, 255);
					return 0;
				}
			case 13:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 43, 183, 62);
					return 0;
				}
			case 14:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 122, 86, 3);
					return 0;
				}
			case 15:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 128, 128, 128);
					return 0;
				}
			case 16:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, 204, 255, 00);
					return 0;
				}
			case 17:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
					set_status_text("~r~WARNING: ~w~Can mess up setting primary and secondary colours!");
				}

				// switchable features
			default:
				if (lines[activeLineIndexPrimMatte].pState)
					*lines[activeLineIndexPrimMatte].pState = !(*lines[activeLineIndexPrimMatte].pState);
				if (lines[activeLineIndexPrimMatte].pUpdated)
					*lines[activeLineIndexPrimMatte].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexPrimMatte == 0)
						activeLineIndexPrimMatte = lineCount;
					activeLineIndexPrimMatte--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexPrimMatte++;
						if (activeLineIndexPrimMatte == lineCount)
							activeLineIndexPrimMatte = 0;
						waitTime = 150;
					}
	}
}


int activeLineIndexPrimClass = 0;

bool process_paint_menu_Smoke()
{

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());


	const float lineWidth = 260.0;
	const int lineCount = 10;

	int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);

	std::string caption = "Tire Smoke";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "White", NULL, NULL },
		{ "Black", NULL, NULL },
		{ "~b~Blue", NULL, NULL },
		{ "~y~Yellow", NULL, NULL },
		{ "~p~Purple", NULL, NULL },
		{ "~o~Orange", NULL, NULL },
		{ "~g~Green", NULL, NULL },
		{ "~r~Red", NULL, NULL },
		{ "~p~Pink", NULL, NULL },
		{ "~r~P~w~A~b~T~r~R~w~I~b~O~r~T", NULL, NULL },


	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexPrimClass)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexPrimClass].text,
				lines[activeLineIndexPrimClass].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexPrimClass * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexPrimClass)
			{
				// next radio track
			case 0:
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(VehID);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::TOGGLE_VEHICLE_MOD(VehID, 20, 1);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(VehID, 254, 254, 254);
				set_status_text("Tire Smoke: White");
				break;
			case 1:
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(VehID);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::TOGGLE_VEHICLE_MOD(VehID, 20, 1);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(VehID, 1, 1, 1);
				set_status_text("Tire Smoke: Black");
				break;
			case 2:
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(VehID);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::TOGGLE_VEHICLE_MOD(VehID, 20, 1);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(VehID, 0, 0, 255);
				set_status_text("Tire Smoke: ~b~Blue");
				break;
			case 3:
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(VehID);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::TOGGLE_VEHICLE_MOD(VehID, 20, 1);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(VehID, 255, 255, 0);
				set_status_text("Tire Smoke: ~y~Yellow");
				break;
			case 4:
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(VehID);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::TOGGLE_VEHICLE_MOD(VehID, 20, 1);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(VehID, 150, 0, 255);
				set_status_text("Tire Smoke: ~p~Purple");
				break;
			case 5:
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(VehID);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::TOGGLE_VEHICLE_MOD(VehID, 20, 1);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(VehID, 255, 175, 0);
				set_status_text("Tire Smoke: ~o~Orange");
				break;
			case 6:
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(VehID);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::TOGGLE_VEHICLE_MOD(VehID, 20, 1);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(VehID, 0, 255, 0);
				set_status_text("Tire Smoke: ~g~Green");
				break;
			case 7:
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(VehID);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::TOGGLE_VEHICLE_MOD(VehID, 20, 1);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(VehID, 255, 0, 0);
				set_status_text("Tire Smoke: ~r~Red");
				break;
			case 8:
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(VehID);
				VEHICLE::TOGGLE_VEHICLE_MOD(VehID, 20, 1);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(VehID, 255, 0, 255);
				set_status_text("Tire Smoke: ~p~Pink");
				break;
			case 9:
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(VehID);
				VEHICLE::TOGGLE_VEHICLE_MOD(VehID, 20, 1);
				VEHICLE::SET_VEHICLE_MOD_KIT(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(VehID, 0);
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(VehID, 0, 0, 0);
				set_status_text("Tire Smoke: ~r~P~w~A~b~T~r~R~w~I~b~O~r~T");
				break;

				// switchable features
			default:
				if (lines[activeLineIndexPrimClass].pState)
					*lines[activeLineIndexPrimClass].pState = !(*lines[activeLineIndexPrimClass].pState);
				if (lines[activeLineIndexPrimClass].pUpdated)
					*lines[activeLineIndexPrimClass].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexPrimClass == 0)
						activeLineIndexPrimClass = lineCount;
					activeLineIndexPrimClass--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexPrimClass++;
						if (activeLineIndexPrimClass == lineCount)
							activeLineIndexPrimClass = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexPrim = 0;

bool process_paint_menu_prim()
{

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	const float lineWidth = 260.0;
	const int lineCount = 18;

	std::string caption = "Primary Colours";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Black", NULL, NULL },
		{ "Silver", NULL, NULL },
		{ "Torino Red", NULL, NULL },
		{ "Hot Pink", NULL, NULL },
		{ "Lambo Orange", NULL, NULL },
		{ "Race Yellow", NULL, NULL },
		{ "Lime Green", NULL, NULL },
		{ "Bright Green", NULL, NULL },
		{ "Ultra Blue", NULL, NULL },
		{ "Schafter Purple", NULL, NULL },
		{ "Bright Purple", NULL, NULL },
		{ "Cream", NULL, NULL },
		{ "Ice White", NULL, NULL },
		{ "Pure Gold", NULL, NULL },
		{ "Brushed Aluminum", NULL, NULL },
		{ "Chrome", NULL, NULL },
		{ "Fluoresent Yellow", NULL, NULL },
		{ "~o~R~y~a~r~n~p~d~b~o~g~m", NULL, NULL },





	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexPrim)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexPrim].text,
				lines[activeLineIndexPrim].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexPrim * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexPrim)
			{
				// next radio track
			case 0:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 0, true);
					return 0;
				}
			case 1:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 4, true);
					return 0;
				}
			case 2:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 27, true);
					return 0;
				}
			case 3:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 135, true);
					return 0;
				}
			case 4:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 138, true);
					return 0;
				}
			case 5:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 89, true);
					return 0;
				}
			case 6:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 92, true);
					return 0;
				}
			case 7:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 53, true);
					return 0;
				}
			case 8:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 70, true);
					return 0;
				}
			case 9:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 71, true);
					return 0;
				}
			case 10:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 145, true);
					return 0;
				}
			case 11:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 107, true);
					return 0;
				}
			case 12:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 111, true);
					return 0;
				}
			case 13:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 158, true);
					return 0;
				}
			case 14:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 119, true);
					return 0;
				}
			case 15:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_COLOURS(veh, 120, true);
					return 0;
				}
			case 16:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, 204, 255, 00);
					return 0;
				}
			case 17:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
					set_status_text("~r~WARNING: ~w~Can mess up setting primary and secondary colours!");
				}

				// switchable features
			default:
				if (lines[activeLineIndexPrim].pState)
					*lines[activeLineIndexPrim].pState = !(*lines[activeLineIndexPrim].pState);
				if (lines[activeLineIndexPrim].pUpdated)
					*lines[activeLineIndexPrim].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexPrim == 0)
						activeLineIndexPrim = lineCount;
					activeLineIndexPrim--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexPrim++;
						if (activeLineIndexPrim == lineCount)
							activeLineIndexPrim = 0;
						waitTime = 150;
					}
	}
}


int activeLineIndexPaint = 0;

bool process_paint_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 2;

	std::string caption = "Paint Menu";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Primary ~h~~o~>>", NULL, NULL }, //prim
		{ "Secondary ~h~~o~>>", NULL, NULL }, //sec



	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexPaint)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexPaint].text,
				lines[activeLineIndexPaint].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexPaint * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexPaint)
			{
				// next radio track
			case 0:
				if (process_paint_menu_prim()) return 0;
				break;
			case 1:
				if (process_paint_menu_Prim_Matte()) return 0;
				break;

				// switchable features
			default:
				if (lines[activeLineIndexPaint].pState)
					*lines[activeLineIndexPaint].pState = !(*lines[activeLineIndexPaint].pState);
				if (lines[activeLineIndexPaint].pUpdated)
					*lines[activeLineIndexPaint].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexPaint == 0)
						activeLineIndexPaint = lineCount;
					activeLineIndexPaint--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexPaint++;
						if (activeLineIndexPaint == lineCount)
							activeLineIndexPaint = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexDrop = 0;

bool process_text_menu()
{

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	const float lineWidth = 260.0;
	const int lineCount = 9;

	std::string caption = "Plate Text";      

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "CRUSADER", NULL, NULL },
		{ "L33T", NULL, NULL },
		{ "h4x0r", NULL, NULL },
		{ "1337", NULL, NULL },
		{ "Modz", NULL, NULL },
		{ "GTA V", NULL, NULL },
		{ "No Text", NULL, NULL },
		{ "-_-", NULL, NULL },
		{ "Halloween", NULL, NULL },


	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexDrop)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexDrop].text,
				lines[activeLineIndexDrop].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexDrop * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexDrop)
			{
				// next radio track
			case 0:
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "CRUSADER");
				set_status_text("Plate Text: ~o~CRUSADER");
				break;
			case 1:
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "L33T");
				set_status_text("Plate Text: ~o~L33T");
				break;
			case 2:
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "h4x0r");
				set_status_text("Plate Text: ~o~h4x0r");
				break;
			case 3:
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "1337");
				set_status_text("Plate Text: ~o~1337");
				break;
			case 4:
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "Modz");
				set_status_text("Plate Text: ~o~Modz");
				break;
			case 5:
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "GTA V");
				set_status_text("Plate Text: ~o~GTA V");
				break;
			case 6:
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "");
				set_status_text("Plate Text: ");
				break;
			case 7:
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "-_-");
				set_status_text("Plate Text: ~o~-_-");
				break;
			case 8:
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "Halloween");
				set_status_text("Plate Text: ~o~Halloween ");
				break;
				// switchable features
			default:
				if (lines[activeLineIndexDrop].pState)
					*lines[activeLineIndexDrop].pState = !(*lines[activeLineIndexDrop].pState);
				if (lines[activeLineIndexDrop].pUpdated)
					*lines[activeLineIndexDrop].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexDrop == 0)
						activeLineIndexDrop = lineCount;
					activeLineIndexDrop--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexDrop++;
						if (activeLineIndexDrop == lineCount)
							activeLineIndexDrop = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexVeh = 0;

void process_veh_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 18;

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);

	std::string caption = "Vehicle Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Customize Menu ~h~~o~>>", NULL, NULL },
		{ "Paint Menu ~h~~o~>>", NULL, NULL },
		{ "Tire Smoke Menu ~h~~o~>>", NULL, NULL },
		{ "Car Spawner ~h~~o~>>", NULL, NULL },
		{ "Licence plate text ~h~~o~>>", NULL, NULL },
		{ "Paint Raindom", NULL, NULL },
		{ "Fix", NULL, NULL },
		{ "Rainbow Tiresmoke", &featureVehRainbowTire, NULL },
		{ "Rainbow Car", &featureVehRainbow, NULL },
		{ "Max Updrade", &featureVehAllMod, NULL },
		{ "Seat belt", &featureVehSeatbelt, &featureVehSeatbeltUpdated },
		{ "Spawn In Car", &featureVehWrapInSpawned, NULL },
		{ "Vehicle Godmode", &featureVehInvincible, &featureVehInvincibleUpdated },
		{ "Strong Wheels", &featureVehInvincibleWheels, &featureVehInvincibleWheelsUpdated },
		{ "Speed Boost", &featureVehSpeedBoost, NULL },
		{ "Crazy Drifts", &featureVehSlide, NULL },
		{ "Scrolling Plate Text ", &featureVehScrollingText, NULL },
		{ "Super RPM", &featureVehSuperRPM, NULL },


	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexVeh)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexVeh].text,
				lines[activeLineIndexVeh].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexVeh * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexVeh)
			{
			case 0:
				if (process_veh_menu_cust()) return;
				break;
			case 1:
				if (process_paint_menu()) return;
				break;
			case 2:
				if (process_paint_menu_Smoke()) return;
				break;

			case 3:
				if (process_carspawn_menu()) return;
				break;
			case 4:
				if (process_text_menu()) return;
				break;
			case 5:
				if (bPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
						VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
						if (VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(veh))
							VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
					}
					else
					{
						set_status_text("player isn't in a vehicle");
					}
				}
				break;
			case 6:
				if (bPlayerExists)
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
						VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(playerPed));
					else
						set_status_text("player isn't in a vehicle");
				break;
			case 7:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(PED::GET_VEHICLE_PED_IS_USING(playerPed));
				}
			default:
				if (lines[activeLineIndexVeh].pState)
					*lines[activeLineIndexVeh].pState = !(*lines[activeLineIndexVeh].pState);
				if (lines[activeLineIndexVeh].pUpdated)
					*lines[activeLineIndexVeh].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexVeh == 0)
						activeLineIndexVeh = lineCount;
					activeLineIndexVeh--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexVeh++;
						if (activeLineIndexVeh == lineCount)
							activeLineIndexVeh = 0;
						waitTime = 150;
					}
	}
}



int activeLineIndexWorld = 0;

void process_world_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 6;

	std::string caption = "World Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Moon Gravity", &featureWorldMoonGravity, NULL },
		{ "Random Cops", &featureWorldRandomCops, NULL },
		{ "Random Trains", &featureWorldRandomTrains, NULL },
		{ "Random Boats", &featureWorldRandomBoats, NULL },
		{ "Garbage Trucks", &featureWorldGarbageTrucks, NULL },
		{ "Crazy Waves", &featureWorldCrazywaves, NULL },

	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexWorld)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexWorld].text,
				lines[activeLineIndexWorld].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexWorld * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexWorld)
			{
			case 0:
				featureWorldMoonGravity = !featureWorldMoonGravity;
				GAMEPLAY::SET_GRAVITY_LEVEL(featureWorldMoonGravity ? 2 : 0);
				break;
			case 1:
				// featureWorldRandomCops being set in update_features
				PED::SET_CREATE_RANDOM_COPS(!featureWorldRandomCops);
				break;
			case 2:
				featureWorldRandomTrains = !featureWorldRandomTrains;
				VEHICLE::SET_RANDOM_TRAINS(featureWorldRandomTrains);
				break;
			case 3:
				featureWorldRandomBoats = !featureWorldRandomBoats;
				VEHICLE::SET_RANDOM_BOATS(featureWorldRandomBoats);
				break;
			case 4:
				featureWorldGarbageTrucks = !featureWorldGarbageTrucks;
				VEHICLE::SET_GARBAGE_TRUCKS(featureWorldGarbageTrucks);
				break;
			case 5:
				featureWorldCrazywaves = !featureWorldCrazywaves;
				WATER::_SET_WAVES_HEIGHT(200); (featureWorldCrazywaves);

				break;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexWorld == 0)
						activeLineIndexWorld = lineCount;
					activeLineIndexWorld--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexWorld++;
						if (activeLineIndexWorld == lineCount)
							activeLineIndexWorld = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexTime = 0;

void process_time_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 4;

	std::string caption = "Time Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Hour Foward", NULL, NULL },
		{ "Hour Backward", NULL, NULL },
		{ "Puase Time", &featureTimePaused, &featureTimePausedUpdated },
		{ "Set To Youre Time! ", &featureTimeSynced, NULL }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexTime)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexTime].text,
				lines[activeLineIndexTime].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexTime * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexTime)
			{
				// hour forward/backward
			case 0:
			case 1:
			{
				int h = TIME::GET_CLOCK_HOURS();
				if (activeLineIndexTime == 0) h = (h == 23) ? 0 : h + 1; else h = (h == 0) ? 23 : h - 1;
				int m = TIME::GET_CLOCK_MINUTES();
				TIME::SET_CLOCK_TIME(h, m, 0);
				char text[32];
				sprintf_s(text, "time %d:%d", h, m);
				set_status_text(text);
			}
			break;
			// switchable features
			default:
				if (lines[activeLineIndexTime].pState)
					*lines[activeLineIndexTime].pState = !(*lines[activeLineIndexTime].pState);
				if (lines[activeLineIndexTime].pUpdated)
					*lines[activeLineIndexTime].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexTime == 0)
						activeLineIndexTime = lineCount;
					activeLineIndexTime--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexTime++;
						if (activeLineIndexTime == lineCount)
							activeLineIndexTime = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexWeather = 0;

void process_weather_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 16;

	std::string caption = "Weather Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Wind", &featureWeatherWind, NULL },
		{ "Set Persist", &featureWeatherPers, NULL },
		{ "Extra Sunny", NULL, NULL },
		{ "Clear", NULL, NULL },
		{ "Clouds", NULL, NULL },
		{ "Smog", NULL, NULL },
		{ "Foggy", NULL, NULL },
		{ "Overcast", NULL, NULL },
		{ "Rainy", NULL, NULL },
		{ "Thunder", NULL, NULL },
		{ "Clearing", NULL, NULL },
		{ "Neuteral", NULL, NULL },
		{ "Snowy", NULL, NULL },
		{ "Blizzard", NULL, NULL },
		{ "Snowlight", NULL, NULL },
		{ "Christmas", NULL, NULL }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexWeather)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexWeather].text,
				lines[activeLineIndexWeather].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexWeather * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexWeather)
			{
				// wind
			case 0:
				featureWeatherWind = !featureWeatherWind;
				if (featureWeatherWind)
				{
					GAMEPLAY::SET_WIND(1.0);
					GAMEPLAY::SET_WIND_SPEED(11.99);
					GAMEPLAY::SET_WIND_DIRECTION(ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
				}
				else
				{
					GAMEPLAY::SET_WIND(0.0);
					GAMEPLAY::SET_WIND_SPEED(0.0);
				}
				break;
				// set persist
			case 1:
				featureWeatherPers = !featureWeatherPers;
				break;
				// set weather
			default:
				GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
				if (featureWeatherPers)
				{
					GAMEPLAY::SET_OVERRIDE_WEATHER((char *)lines[activeLineIndexWeather].text);
				}
				else
				{
					GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST((char *)lines[activeLineIndexWeather].text);
					GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
				}
				set_status_text(lines[activeLineIndexWeather].text);
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexWeather == 0)
						activeLineIndexWeather = lineCount;
					activeLineIndexWeather--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexWeather++;
						if (activeLineIndexWeather == lineCount)
							activeLineIndexWeather = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexObj = 0;

void process_obj_menu()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 1);

	const float lineWidth = 260.0;
	const int lineCount = 18;

	std::string caption = "Object Spawner";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Pumpkin", NULL, NULL },
		{ "ufo", NULL, NULL },
		{ "Weed Leaves", NULL, NULL },
		{ "Christmas Tree", NULL, NULL },
		{ "Windmill", NULL, NULL },
		{ "Alien Egg", NULL, NULL },
		{ "Dildo", NULL, NULL },
		{ "Toilet", NULL, NULL },
		{ "Ferris Wheel", NULL, NULL },
		{ "Cable Car", NULL, NULL },
		{ "Half Pipe", NULL, NULL },
		{ "Giant Orange", NULL, NULL },
		{ "Drugs", NULL, NULL },
		{ "Roller Car", NULL, NULL },
		{ "Laptop", NULL, NULL },
		{ "ATM Machine", NULL, NULL },
		{ "Trolly Of Gold", NULL, NULL },
		{ "Input Object ~h~~o~>>", NULL, NULL }


	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexObj)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexObj].text,
				lines[activeLineIndexObj].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexObj * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;


		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexObj)
			{
				// next radio track
			case 0:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(0x83C07621);
					while (!STREAMING::HAS_MODEL_LOADED(0x83C07621))
						WAIT(0);
					OBJECT::CREATE_OBJECT((0x83C07621), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
			case 1:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("p_spinning_anus_s"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("p_spinning_anus_s")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("p_spinning_anus_s"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
			case 2:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_weed_01"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_weed_01")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_weed_01"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
			case 3:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_xmas_tree_int"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_xmas_tree_int")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_xmas_tree_int"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 4:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_windmill1"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_windmill1")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_windmill1"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 5:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_alien_egg_01"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_alien_egg_01")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_alien_egg_01"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
			case 6:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_cs_dildo_01"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_cs_dildo_01")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_cs_dildo_01"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 7:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_ld_toilet_01"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_ld_toilet_01")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_ld_toilet_01"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 8:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_ld_ferris_wheel"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_ld_ferris_wheel")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_ld_ferris_wheel"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 9:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("p_cablecar_s"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("p_cablecar_s")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("p_cablecar_s"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 10:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_skate_halfpipe_cr"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_skate_halfpipe_cr")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_skate_halfpipe_cr"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 11:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_juicestand"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_juicestand")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_juicestand"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 12:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_weed_pallet"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_weed_pallet")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_weed_pallet"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 13:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_roller_car_01"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_roller_car_01")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_roller_car_01"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 14:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("hei_prop_hst_laptop"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("hei_prop_hst_laptop")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("hei_prop_hst_laptop"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 15:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_large_gold"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_large_gold")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_large_gold"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 16:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_sculpt_fix"));
					while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_sculpt_fix")))
						WAIT(0);
					OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_sculpt_fix"), position.x, position.y, position.z, 0, 3000, 1000);
					return;
					break;
				}
				break;
			case 17:
				// create object
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "FMMC_KEY_TIP8", "", "p_spinning_anus_s", "", "", "", 20);
					while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
					if (!STREAMING::IS_MODEL_IN_CDIMAGE(GAMEPLAY::GET_HASH_KEY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())))
					{
						return;
					}
					else
					{
						Vector3 get_entity_coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);
						char* get_onscreen_keyboard_result = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
						Hash get_hash_key = GAMEPLAY::GET_HASH_KEY(get_onscreen_keyboard_result);
						STREAMING::REQUEST_MODEL(get_hash_key);
						while (!STREAMING::HAS_MODEL_LOADED(get_hash_key))
							WAIT(0);
						Object create_object = OBJECT::CREATE_OBJECT(get_hash_key, get_entity_coords.x, get_entity_coords.y + 10.0f, get_entity_coords.z + 10.0f, true, true, false);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(get_hash_key);
					}
					std::string playerName = PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID());
					std::string text = " Spawned ~w~A Model ~g~Successfuly";
					set_status_text(playerName + text);
				}
				break;
				// switchable features
			default:
				if (lines[activeLineIndexObj].pState)
					*lines[activeLineIndexObj].pState = !(*lines[activeLineIndexObj].pState);
				if (lines[activeLineIndexObj].pUpdated)
					*lines[activeLineIndexObj].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexObj == 0)
						activeLineIndexObj = lineCount;
					activeLineIndexObj--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexObj++;
						if (activeLineIndexObj == lineCount)
							activeLineIndexObj = 0;
						waitTime = 150;
					}
	}
}





int activeLineIndexwep = 0;


void process_wep_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 6;

	std::string caption = "Vehicle Weapons";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Fireworks", &featureWeaponVehFire, NULL },
		{ "Rockets", &featureWeaponVehRockets, NULL },
		{ "Snow Balls", &featureWeaponVehSnow, NULL },
		{ "Green Laser", &featureWeaponVehGreen, NULL },
		{ "Red Laser", &featureWeaponVehRed, NULL },
		{ "Tank", &featureWeaponVehTank, NULL },



	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexwep)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexwep].text,
				lines[activeLineIndexwep].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexwep * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;


		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexwep)
			{



				// switchable features
			default:
				if (lines[activeLineIndexwep].pState)
					*lines[activeLineIndexwep].pState = !(*lines[activeLineIndexwep].pState);
				if (lines[activeLineIndexwep].pUpdated)
					*lines[activeLineIndexwep].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexwep == 0)
						activeLineIndexwep = lineCount;
					activeLineIndexwep--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexwep++;
						if (activeLineIndexwep == lineCount)
							activeLineIndexwep = 0;
						waitTime = 150;
					}
	}
}




int activeLineIndexNorth = 0;


void process_ny_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 1;

	std::string caption = "North Yankton";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Load", NULL, NULL },

	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexNorth)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexNorth].text,
				lines[activeLineIndexNorth].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexNorth * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexNorth)
			{
				// Load North Yankton
			case 0:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID())
					)
					STREAMING::REQUEST_IPL("plg_01");
				STREAMING::REQUEST_IPL("prologue01");
				STREAMING::REQUEST_IPL("prologue01_lod");
				STREAMING::REQUEST_IPL("prologue01c");
				STREAMING::REQUEST_IPL("prologue01c_lod");
				STREAMING::REQUEST_IPL("prologue01d");
				STREAMING::REQUEST_IPL("prologue01d_lod");
				STREAMING::REQUEST_IPL("prologue01e");
				STREAMING::REQUEST_IPL("prologue01e_lod");
				STREAMING::REQUEST_IPL("prologue01f");
				STREAMING::REQUEST_IPL("prologue01f_lod");
				STREAMING::REQUEST_IPL("prologue01g");
				STREAMING::REQUEST_IPL("prologue01h");
				STREAMING::REQUEST_IPL("prologue01h_lod");
				STREAMING::REQUEST_IPL("prologue01i");
				STREAMING::REQUEST_IPL("prologue01i_lod");
				STREAMING::REQUEST_IPL("prologue01j");
				STREAMING::REQUEST_IPL("prologue01j_lod");
				STREAMING::REQUEST_IPL("prologue01k");
				STREAMING::REQUEST_IPL("prologue01k_lod");
				STREAMING::REQUEST_IPL("prologue01z");
				STREAMING::REQUEST_IPL("prologue01z_lod");
				STREAMING::REQUEST_IPL("plg_02");
				STREAMING::REQUEST_IPL("prologue02");
				STREAMING::REQUEST_IPL("prologue02_lod");
				STREAMING::REQUEST_IPL("plg_03");
				STREAMING::REQUEST_IPL("prologue03");
				STREAMING::REQUEST_IPL("prologue03_lod");
				STREAMING::REQUEST_IPL("prologue03b");
				STREAMING::REQUEST_IPL("prologue03b_lod");
				//the commented code disables the 'Prologue' grave and
				//enables the 'Bury the Hatchet' grave
				//STREAMING::REQUEST_IPL("prologue03_grv_cov");
				//STREAMING::REQUEST_IPL("prologue03_grv_cov_lod");
				STREAMING::REQUEST_IPL("prologue03_grv_dug");
				STREAMING::REQUEST_IPL("prologue03_grv_dug_lod");
				//STREAMING::REQUEST_IPL("prologue03_grv_fun");
				STREAMING::REQUEST_IPL("prologue_grv_torch");
				STREAMING::REQUEST_IPL("plg_04");
				STREAMING::REQUEST_IPL("prologue04");
				STREAMING::REQUEST_IPL("prologue04_lod");
				STREAMING::REQUEST_IPL("prologue04b");
				STREAMING::REQUEST_IPL("prologue04b_lod");
				STREAMING::REQUEST_IPL("prologue04_cover");
				STREAMING::REQUEST_IPL("des_protree_end");
				STREAMING::REQUEST_IPL("des_protree_start");
				STREAMING::REQUEST_IPL("des_protree_start_lod");
				STREAMING::REQUEST_IPL("plg_05");
				STREAMING::REQUEST_IPL("prologue05");
				STREAMING::REQUEST_IPL("prologue05_lod");
				STREAMING::REQUEST_IPL("prologue05b");
				STREAMING::REQUEST_IPL("prologue05b_lod");
				STREAMING::REQUEST_IPL("plg_06");
				STREAMING::REQUEST_IPL("prologue06");
				STREAMING::REQUEST_IPL("prologue06_lod");
				STREAMING::REQUEST_IPL("prologue06b");
				STREAMING::REQUEST_IPL("prologue06b_lod");
				STREAMING::REQUEST_IPL("prologue06_int");
				STREAMING::REQUEST_IPL("prologue06_int_lod");
				STREAMING::REQUEST_IPL("prologue06_pannel");
				STREAMING::REQUEST_IPL("prologue06_pannel_lod");
				STREAMING::REQUEST_IPL("prologue_m2_door");
				STREAMING::REQUEST_IPL("prologue_m2_door_lod");
				STREAMING::REQUEST_IPL("plg_occl_00");
				STREAMING::REQUEST_IPL("prologue_occl");
				STREAMING::REQUEST_IPL("plg_rd");
				STREAMING::REQUEST_IPL("prologuerd");
				STREAMING::REQUEST_IPL("prologuerdb");
				STREAMING::REQUEST_IPL("prologuerd_lod");
				set_status_text("North Yankton ~o~Loaded ~w~Teleport using Teleport menu!");
				break;;


				// switchable features
			default:
				if (lines[activeLineIndexNorth].pState)
					*lines[activeLineIndexNorth].pState = !(*lines[activeLineIndexNorth].pState);
				if (lines[activeLineIndexNorth].pUpdated)
					*lines[activeLineIndexNorth].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexNorth == 0)
						activeLineIndexNorth = lineCount;
					activeLineIndexNorth--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexNorth++;
						if (activeLineIndexNorth == lineCount)
							activeLineIndexNorth = 0;
						waitTime = 150;
					}
	}
}




int activeLineIndexAnimation = 0;

void process_animation_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 17;

	std::string caption = "Animation Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Pole Dance 1", NULL, NULL },
		{ "Pole Dance 2", NULL, NULL },
		{ "Pole Dance 3", NULL, NULL },
		{ "Stunned", NULL, NULL },
		{ "Situps", NULL, NULL },
		{ "Pushups", NULL, NULL },
		{ "Wave Arms", NULL, NULL },
		{ "Suicide", NULL, NULL },
		{ "On The Can", NULL, NULL },
		{ "On Fire", NULL, NULL },
		{ "Cower", NULL, NULL },
		{ "Private Dance", NULL, NULL },
		{ "BJ", NULL, NULL },
		{ "Stungun", NULL, NULL },
		{ "Air Fuck", NULL, NULL },
		{ "Air Fuck 2", NULL, NULL },
		{ "Stop Animation", NULL, NULL }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexAnimation)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexAnimation].text,
				lines[activeLineIndexAnimation].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexAnimation * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexAnimation)
			{
				// Pole Dance 1
			case 0:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict0 = "mini@strip_club@pole_dance@pole_dance1";
					char *anim0 = "pd_dance_01";
					STREAMING::REQUEST_ANIM_SET(anim0);
					STREAMING::REQUEST_ANIM_DICT(dict0);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict0) && !STREAMING::HAS_ANIM_DICT_LOADED(dict0)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict0, anim0, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;
				// Pole Dance 2
			case 1:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict1 = "mini@strip_club@pole_dance@pole_dance2";
					char *anim1 = "pd_dance_02";
					STREAMING::REQUEST_ANIM_SET(anim1);
					STREAMING::REQUEST_ANIM_DICT(dict1);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict1) && !STREAMING::HAS_ANIM_DICT_LOADED(dict1)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict1, anim1, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;
				// Pole Dance 3
			case 2:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict2 = "mini@strip_club@pole_dance@pole_dance3";
					char *anim2 = "pd_dance_03";
					STREAMING::REQUEST_ANIM_SET(anim2);
					STREAMING::REQUEST_ANIM_DICT(dict2);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict2) && !STREAMING::HAS_ANIM_DICT_LOADED(dict2)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict2, anim2, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;
				// Stun
			case 3:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict3 = "ragdoll@human";
					char *anim3 = "electrocute";
					STREAMING::REQUEST_ANIM_SET(anim3);
					STREAMING::REQUEST_ANIM_DICT(dict3);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict3) && !STREAMING::HAS_ANIM_DICT_LOADED(dict3)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict3, anim3, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;


			case 4:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict5 = "amb@world_human_sit_ups@male@base";
					char *anim5 = "base";
					STREAMING::REQUEST_ANIM_SET(anim5);
					STREAMING::REQUEST_ANIM_DICT(dict5);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict5) && !STREAMING::HAS_ANIM_DICT_LOADED(dict5)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict5, anim5, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 5:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict6 = "amb@world_human_push_ups@male@base";
					char *anim6 = "base";
					STREAMING::REQUEST_ANIM_SET(anim6);
					STREAMING::REQUEST_ANIM_DICT(dict6);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict6) && !STREAMING::HAS_ANIM_DICT_LOADED(dict6)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict6, anim6, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 6:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict7 = "random@car_thief@waving_ig_1";
					char *anim7 = "waving";
					STREAMING::REQUEST_ANIM_SET(anim7);
					STREAMING::REQUEST_ANIM_DICT(dict7);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict7) && !STREAMING::HAS_ANIM_DICT_LOADED(dict7)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict7, anim7, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 7:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict8 = "mp_suicide";
					char *anim8 = "pistol_fp";
					STREAMING::REQUEST_ANIM_SET(anim8);
					STREAMING::REQUEST_ANIM_DICT(dict8);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict8) && !STREAMING::HAS_ANIM_DICT_LOADED(dict8)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict8, anim8, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;




			case 8:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict9 = "timetable@trevor@on_the_toilet";
					char *anim9 = "trevonlav_struggleloop";
					STREAMING::REQUEST_ANIM_SET(anim9);
					STREAMING::REQUEST_ANIM_DICT(dict9);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict9) && !STREAMING::HAS_ANIM_DICT_LOADED(dict9)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict9, anim9, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;




			case 9:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict10 = "ragdoll@human";
					char *anim10 = "on_fire";
					STREAMING::REQUEST_ANIM_SET(anim10);
					STREAMING::REQUEST_ANIM_DICT(dict10);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict10) && !STREAMING::HAS_ANIM_DICT_LOADED(dict10)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict10, anim10, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;



			case 10:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict11 = "random@arrests";
					char *anim11 = "kneeling_arrest_idle";
					STREAMING::REQUEST_ANIM_SET(anim11);
					STREAMING::REQUEST_ANIM_DICT(dict11);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict11) && !STREAMING::HAS_ANIM_DICT_LOADED(dict11)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict11, anim11, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;



			case 11:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict12 = "mini@strip_club@private_dance@part1";
					char *anim12 = "priv_dance_p1";
					STREAMING::REQUEST_ANIM_SET(anim12);
					STREAMING::REQUEST_ANIM_DICT(dict12);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict12) && !STREAMING::HAS_ANIM_DICT_LOADED(dict12)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict12, anim12, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;





			case 12:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict13 = "mini@prostitutes@sexnorm_veh";
					char *anim13 = "bj_loop_prostitute";
					STREAMING::REQUEST_ANIM_SET(anim13);
					STREAMING::REQUEST_ANIM_DICT(dict13);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict13) && !STREAMING::HAS_ANIM_DICT_LOADED(dict13)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict13, anim13, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;



			case 13:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict14 = "stungun@standing";
					char *anim14 = "Damage";
					STREAMING::REQUEST_ANIM_SET(anim14);
					STREAMING::REQUEST_ANIM_DICT(dict14);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict14) && !STREAMING::HAS_ANIM_DICT_LOADED(dict14)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict14, anim14, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 14:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict15 = "rcmpaparazzo_2";
					char *anim15 = "shag_loop_a";
					STREAMING::REQUEST_ANIM_SET(anim15);
					STREAMING::REQUEST_ANIM_DICT(dict15);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict15) && !STREAMING::HAS_ANIM_DICT_LOADED(dict15)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict15, anim15, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 15:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict16 = "rcmpaparazzo_2";
					char *anim16 = "shag_loop_poppy";
					STREAMING::REQUEST_ANIM_SET(anim16);
					STREAMING::REQUEST_ANIM_DICT(dict16);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict16) && !STREAMING::HAS_ANIM_DICT_LOADED(dict16)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict16, anim16, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;
			case 16:
				if (bPlayerExists)
				{ 
					Freeze(playerPed);
				}
				break;
				// switchable features
			default:
				if (lines[activeLineIndexAnimation].pState)
					*lines[activeLineIndexAnimation].pState = !(*lines[activeLineIndexAnimation].pState);
				if (lines[activeLineIndexAnimation].pUpdated)
					*lines[activeLineIndexAnimation].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexAnimation == 0)
						activeLineIndexAnimation = lineCount;
					activeLineIndexAnimation--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexAnimation++;
						if (activeLineIndexAnimation == lineCount)
							activeLineIndexAnimation = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexVis = 0;

void process_vision_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 6;

	std::string caption = "Vision Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Thermal Vision", &featureVisionThermal, NULL },
		{ "Night Vision", &featureVisionNight, NULL },
		{ "ESP", &featureESP, NULL },
		{ "Name ESP", &featureNameESP, NULL },
		{ "ESP Lines", &featurePlayerESPTrace, NULL },
		{ "Drunk Vision", &featureVisionDrunk, &featureVisionDrunkUpdated }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexVis)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexVis].text,
				lines[activeLineIndexVis].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexVis * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexVis)
			{
				// next radio track
			case 0:
				featureVisionThermal = !featureVisionThermal;
				GRAPHICS::SET_SEETHROUGH(featureVisionThermal);
				break;

				// Night Vision 
			case 1:
				featureVisionNight = !featureVisionNight;
				GRAPHICS::SET_NIGHTVISION(featureVisionNight);
				break;
				// switchable features
			default:
				if (lines[activeLineIndexVis].pState)
					*lines[activeLineIndexVis].pState = !(*lines[activeLineIndexVis].pState);
				if (lines[activeLineIndexVis].pUpdated)
					*lines[activeLineIndexVis].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexVis == 0)
						activeLineIndexVis = lineCount;
					activeLineIndexVis--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexVis++;
						if (activeLineIndexVis == lineCount)
							activeLineIndexVis = 0;
						waitTime = 150;
					}
	}
}

//WIP (Not Functional)

int activeLineIndexWalk = 0;

void process_walk_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 14;

	std::string caption = "WALK STYLES";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Tipsy ", NULL, NULL },
		{ "Drunk", NULL, NULL },
		{ "Tazered", NULL, NULL },
		{ "Cane", NULL, NULL },
		{ "Limp", NULL, NULL },
		{ "Crouched", NULL, NULL },
		{ "Heavy", NULL, NULL },
		{ "Female", NULL, NULL },
		{ "Female sexy", NULL, NULL },
		{ "Male casual", NULL, NULL },
		{ "Slouched", NULL, NULL },
		{ "Gangster", NULL, NULL },
		{ "Garbageman", NULL, NULL },
		{ "Very drunk", NULL, NULL },

	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true); 
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexWalk)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexWalk].text,
				lines[activeLineIndexWalk].pState), lineWidth, 13.6, 38.0 + activeLineIndexWalk * 26.0, 600.0, 4.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexWalk)
			{
				// Pole Dance 1
			case 0:
				if (bPlayerExists)
				{
					STREAMING::REQUEST_ANIM_SET("MOVE_M@DRUNK@SLIGHTLYDRUNK");
					while (!STREAMING::HAS_ANIM_SET_LOADED("MOVE_M@DRUNK@SLIGHTLYDRUNK"))
					{
						WAIT(0);
					}
					PED::SET_PED_MOVEMENT_CLIPSET(playerPed, "MOVE_M@DRUNK@SLIGHTLYDRUNK", 1.0f);
				}
				break;
			case 1:
				if (bPlayerExists)
				{
					STREAMING::REQUEST_ANIM_SET("MOVE_M@DRUNK@MODERATEDRUNK");
					while (!STREAMING::HAS_ANIM_SET_LOADED("MOVE_M@DRUNK@MODERATEDRUNK"))
					{
						WAIT(0);
					}
					PED::SET_PED_MOVEMENT_CLIPSET(playerPed, "MOVE_M@DRUNK@MODERATEDRUNK", 1.0f);
				}
				break;
			case 2:
				if (bPlayerExists)
				{
					STREAMING::REQUEST_ANIM_SET("MOVE_M@BAIL_BOND_TAZERED");
					while (!STREAMING::HAS_ANIM_SET_LOADED("MOVE_M@BAIL_BOND_TAZERED"))
					{
						WAIT(0);
					}
					PED::SET_PED_MOVEMENT_CLIPSET(playerPed, "MOVE_M@BAIL_BOND_TAZERED", 1.0f);
				}
				break;

				// switchable features
			default:
				if (lines[activeLineIndexWalk].pState)
					*lines[activeLineIndexWalk].pState = !(*lines[activeLineIndexWalk].pState);
				if (lines[activeLineIndexWalk].pUpdated)
					*lines[activeLineIndexWalk].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexWalk == 0)
						activeLineIndexWalk = lineCount;
					activeLineIndexWalk--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexWalk++;
						if (activeLineIndexWalk == lineCount)
							activeLineIndexWalk = 0;
						waitTime = 150;
					}
	}
}

void TeleportToClient(int Client)
{
	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(Client, 1);
	if (PED::IS_PED_SITTING_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID()))
		ENTITY::SET_ENTITY_COORDS(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), Coords.x, Coords.y, Coords.z, 1, 0, 0, 1);
	else
		ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z, 1, 0, 0, 1);
}

void GiveWeaponsClient()
{
	uint Weapons[] = { 0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B, 0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764, 0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x05FC3C11, 0x0C472FE2, 0x33058E22, 0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0x24B17070, 0x060EC506, 0x34A67B97, 0xFDBADCED, 0x23C9F95C, 0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xC0A3098D, 0xD205520E, 0xBFD21232, 0x7F229F94, 0x92A27487, 0x083839C4, 0x7F7497E5, 0xA89CB99E, 0x3AABBBAA, 0xC734385A, 0x787F0BB, 0x47757124, 0xD04C944D };
	for (int i = 0; i < (sizeof(Weapons) / 4); i++)
	{
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED(selectedPlayer), Weapons[i], 9999, 1);
		WAIT(10);
	}
}


void requestControl(DWORD entity)
{
	NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
	int tick = 0;
	while (tick <= 50)
	{
		if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity))
			WAIT(0);
		else
			return;
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
		tick++;
	}
}




void bodyguard(Ped ped, Player player)
{
	if (ENTITY::DOES_ENTITY_EXIST(ped))
	{
		GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "FMMC_KEY_TIP8", "", "a_f_y_topless_01", "", "", "", 20);
		while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
		if (!STREAMING::IS_MODEL_IN_CDIMAGE(GAMEPLAY::GET_HASH_KEY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())))
		{
			return;
		}
		else
		{
			Vector3 get_entity_coords = ENTITY::GET_ENTITY_COORDS(ped, 0);
			Ped get_ped_group_index = PED::GET_PED_GROUP_INDEX(ped);
			char* get_onscreen_keyboard_result = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
			Hash get_hash_key = GAMEPLAY::GET_HASH_KEY(get_onscreen_keyboard_result);
			STREAMING::REQUEST_MODEL(get_hash_key);
			while (!STREAMING::HAS_MODEL_LOADED(get_hash_key))
				WAIT(0);
			Ped create_ped = PED::CREATE_PED(1, get_hash_key, get_entity_coords.x, get_entity_coords.y + 5.0f, get_entity_coords.z, ENTITY::GET_ENTITY_HEADING(ped), true, true);
			WAIT(0);
			PED::SET_PED_CAN_RAGDOLL(create_ped, false);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(create_ped, false);
			PED::SET_PED_AS_GROUP_MEMBER(create_ped, get_ped_group_index);
			PED::SET_PED_COMBAT_RANGE(create_ped, 500);
			PED::SET_PED_ALERTNESS(create_ped, 100);
			PED::SET_PED_ACCURACY(create_ped, 100);
			PED::SET_PED_CAN_SWITCH_WEAPON(create_ped, 1);
			PED::SET_PED_SHOOT_RATE(create_ped, 200);
			PED::SET_PED_KEEP_TASK(create_ped, true);
			AI::TASK_COMBAT_HATED_TARGETS_AROUND_PED(create_ped, 5000.f, 0);
			WAIT(0);
			//for (int a = 0; a < sizeof(weaponNames) / sizeof(weaponNames[0]); a++)
			//{
			////	/WEAPON::GIVE_DELAYED_WEAPON_TO_PED(create_ped, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[a]), 1000, 0);
			//}
		//	WAIT(0);
			PED::SET_PED_CAN_SWITCH_WEAPON(create_ped, true);
			PED::SET_PED_KEEP_TASK(create_ped, true);
			PED::SET_PED_GENERATES_DEAD_BODY_EVENTS(create_ped, true);
			// misc invincible bodyguard
			if (featurePedestrianInvincibleBodyguardUpdate)
			{
				if (!featurePedestrianInvincibleBodyguard)
					ENTITY::SET_ENTITY_INVINCIBLE(create_ped, false);
				featurePedestrianInvincibleBodyguardUpdate = false;
			}
			if (featurePedestrianInvincibleBodyguard)
			{
				ENTITY::SET_ENTITY_INVINCIBLE(create_ped, true);
			}
			WAIT(0);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(get_hash_key);
		}

		std::string playerName = PLAYER::GET_PLAYER_NAME(player);
		std::string text = " A bodyguard was spawned for ~o~ ";
		set_status_text(text + playerName);
	}
}


int activeLineIndexGuard = 0;

void process_guard_menu()
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	const float lineWidth = 260.0;  //uptohere
	const int lineCount = 10;

	std::string caption = "Misc Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Skip Radio Track", NULL, NULL },
		{ "Input Bodyguard Model ~h~~o~>>", NULL, NULL },
		{ "Shake Camera", NULL, NULL },
		{ "Spawn Chauffer", NULL, NULL },
		{ "Clone Player", NULL, NULL },
		{ "Hide HUD", &featureMiscHideHud, NULL },
		{ "Mobile Radio", &featureMiscMobile, NULL },
		{ "ESP", &featureESP, NULL }, //trying to see if freeze worksd
		{ "Name ESP", &featureNameESP, NULL },
		{ "Freeze Self", &freezeplayer, NULL },


	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexGuard)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexGuard].text,
				lines[activeLineIndexGuard].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexGuard * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;


		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexGuard)
			{
				// next radio track
			case 0:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) &&
					PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
					AUDIO::SKIP_RADIO_FORWARD();
				break;
			case 1:
				// create bodyguard
				bodyguard(PLAYER::PLAYER_PED_ID(), PLAYER::PLAYER_ID());
				break;
				break;
			case 2:
				if (bPlayerExists)
				{
					CAM::SHAKE_GAMEPLAY_CAM("MEDIUM_EXPLOSION_SHAKE", 5.0f);
				}
				break;
			case 3:
				if (bPlayerExists)
				{
					spawnChauffer();
				}
			case 4:
				if (bPlayerExists)
				{
					int Ped = PED::CLONE_PED(PLAYER::GET_PLAYER_PED(selectedPlayer), ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED(selectedPlayer)), 1, 1);

				}
				break;
				// switchable features
			default:
				if (lines[activeLineIndexGuard].pState)
					*lines[activeLineIndexGuard].pState = !(*lines[activeLineIndexGuard].pState);
				if (lines[activeLineIndexGuard].pUpdated)
					*lines[activeLineIndexGuard].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexGuard == 0)
						activeLineIndexGuard = lineCount;
					activeLineIndexGuard--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexGuard++;
						if (activeLineIndexGuard == lineCount)
							activeLineIndexGuard = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexCredits = 0;

void process_credit_menu()
{
	const float lineWidth = 260.0;
	const int lineCount = 10;

	std::string caption = "Credits";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "The Crusader ~r~(Developer)",	NULL,					NULL },
		{ "And Thanks Too ~r~(For Some Codes):",	NULL,					NULL },
		{ "iTzNeutron",	NULL,					NULL },
		{ "Gaz'lam",	NULL,					NULL },
		{ "Alexander Blade",	NULL,					NULL },
		{ "gir489",	NULL,					NULL },
		{ "NTAuthority / citizenMP",	NULL,					NULL },
		{ "s0biet",	NULL,					NULL },
		{ "FreeOffset",	NULL,					NULL },
		{ "BiG_GHOST_GaMeR",	NULL,					NULL },

	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexCredits)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
						lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexCredits].text, lines[activeLineIndexCredits].pState),
				lineWidth + 0.0, 2.0, 60.0 + activeLineIndexCredits * 24.0, 4.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexCredits)
			{
				// next radio track
			case 0:

				break;
				// switchable features
			default:
				if (lines[activeLineIndexCredits].pState)
					*lines[activeLineIndexCredits].pState = !(*lines[activeLineIndexCredits].pState);
				if (lines[activeLineIndexCredits].pUpdated)
					*lines[activeLineIndexCredits].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexCredits == 0)
						activeLineIndexCredits = lineCount;
					activeLineIndexCredits--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexCredits++;
						if (activeLineIndexCredits == lineCount)
							activeLineIndexCredits = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexFX = 0;

void process_FX_menu()
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	const float lineWidth = 260.0;  
	const int lineCount = 16;

	std::string caption = "Scenario Options";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Stop Scenario", NULL, NULL },
		{ "Drilling", NULL, NULL },
		{ "Cop", NULL, NULL },
		{ "Chin Ups", NULL, NULL },
		{ "Binoculars", NULL, NULL },
		{ "Hammering", NULL, NULL },
		{ "420", NULL, NULL },
		{ "Leaf Blower", NULL, NULL },
		{ "Musician", NULL, NULL },
		{ "Drinking Coffee", NULL, NULL },
		{ "Jogging", NULL, NULL },
		{ "Fishing", NULL, NULL },
		{ "Push Ups", NULL, NULL },
		{ "Sit Ups", NULL, NULL },
		{ "Yoga", NULL, NULL },
		{ "Gym Lad", NULL, NULL },
	
	
	


	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexFX)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexFX].text,
				lines[activeLineIndexFX].pState), lineWidth + 1.0, 2.0, 60.0 + activeLineIndexFX * 24.0, 4.0, 7.0, true, false);


			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;


		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexFX)
			{
				// next radio track
			case 0:
				AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
				break;
			case 1:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_CONST_DRILL", 0, true);
				break;
			case 2:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "CODE_HUMAN_POLICE_INVESTIGATE", 0, true);
				break;
			case 3:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "PROP_HUMAN_MUSCLE_CHIN_UPS", 0, true);
				break;
			case 4:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_BINOCULARS", 0, true);
				break;
			case 5:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_HAMMERING", 0, true);
				break;
			case 6:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_SMOKING_POT", 0, true);
				break;
			case 7:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_GARDENER_LEAF_BLOWER", 0, true);
				break;
			case 8:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_MUSICIAN", 0, true);
				break;
			case 9:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_AA_COFFEE", 0, true);
				break;
			case 10:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_JOG_STANDING", 0, true);
				break;
			case 11:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_STAND_FISHING", 0, true);
				break;
			case 12:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_PUSH_UPS", 0, true);
				break;
			case 13:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_SIT_UPS", 0, true);
				break;
			case 14:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_YOGA", 0, true);
				break;
			case 15:
				AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_MUSCLE_FREE_WEIGHTS", 0, true);
				break;


				// switchable features
			default:
				if (lines[activeLineIndexFX].pState)
					*lines[activeLineIndexFX].pState = !(*lines[activeLineIndexFX].pState);
				if (lines[activeLineIndexFX].pUpdated)
					*lines[activeLineIndexFX].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexFX == 0)
						activeLineIndexFX = lineCount;
					activeLineIndexFX--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexFX++;
						if (activeLineIndexFX == lineCount)
							activeLineIndexFX = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexMain = 0;

void process_main_menu()
{                                                                                                                       
	
	const float lineWidth = 260.0;
	const int lineCount = 17;

	std::string caption = "Halloween";

	static LPCSTR lineCaption[lineCount] = {
		"Player Options ~h~~o~>>",
		"Weapon Options ~h~~o~>>",
		"Vehicle Options ~h~~o~>>",
		"World Options ~h~~o~>>",
		"Time Options ~h~~o~>>",
		"Weather Options ~h~~o~>>",
		"Object Spawner ~h~~o~>>",
		"Animation Options ~h~~o~>>",
		"North Yankton ~h~~o~>>",
		"Vehicle Weapons ~h~~o~>>",
		"Vision Options ~h~~o~>>",
		"Misc Options ~h~~o~>>",
		"Scenarios ~h~~o~>>",
		"Reset All Options ~h~~o~>>",
		"Credits ~h~~o~>>",
		"Bloodbath! ~h~~o~>>",
		"Exit ~h~~o~>>",
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{   
			// draw menu
			draw_menu_line(caption, lineWidth, 7.0, 13.6, 4.0, 4.0, false, true);  
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexMain)
					draw_menu_line(lineCaption[i], lineWidth, 4.0, 60.0 + i * 24.0, 4.0, 9.0, false, false);

			draw_menu_line(lineCaption[activeLineIndexMain], lineWidth + 0.0, 2.0, 60.0 + activeLineIndexMain * 24.0, 4.0, 7.0, true, false);
			drawNotification("Made By: ~o~The Crusader");
			std::string playerName = PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID());
			std::string welcome = "Welcome ~o~";
			set_status_text(welcome + playerName);
			drawNotification("Use The Menu At ~o~1920x1080p ~w~For The Best Experience.");
			

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;



		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{

			menu_beep();
			switch (activeLineIndexMain)
			{
			case 0:
				process_player_menu();
				break;
			case 1:
				process_weapon_menu();
				break;
			case 2:
				process_veh_menu();
				break;
			case 3:
				process_world_menu();
				break;
			case 4:
				process_time_menu();
				break;
			case 5:
				process_weather_menu();
				break;
			case 6:
				process_obj_menu();
				break;
			case 7:
			   process_animation_menu();
				break;
			case 8:
				process_ny_menu();
				break;
			case 9:
				process_wep_menu();
				break;
			case 10:
				process_vision_menu();
				break;
			case 11:
				process_guard_menu();
				break;
			case 12:
				process_FX_menu();
				break;
			case 13:
				    activeLineIndexMain =
					activeLineIndexPlayer =
					skinchangerActiveLineIndex =
					skinchangerActiveItemIndex =
					teleportActiveLineIndex =
					activeLineIndexWeapon =
					activeLineIndexVeh =
					carspawnActiveLineIndex =
					carspawnActiveItemIndex =
					activeLineIndexWorld =
					activeLineIndexWeather = 0;
				    weaponGravityGun =
					weaponDeleteObjects =


					featurePlayerSuperRun =
					featurePlayerHeadDisplay =
					featurePlayerBlipCone =
					featurePlayerNotifications =
					featureShowVoiceChatSpeaker =
					featurePlayerInvincible =
					featurePlayerInvincibleUpdated =
					featurePlayerNeverWanted =
					featurePlayerIgnored = 
					featurePlayerIgnoredUpdated =
					featureRainMoney =
					featurePlayerUnlimitedAbility =
					featurePlayerNoNoise =
					featurePlayerNoNoiseUpdated =
					featurePlayerFastSwim =
					featurePlayerFastSwimUpdated =
					featurePlayerFastRun =
					featurePlayerFastRunUpdated =
					featurePlayerInvisible =
					featurePlayerInvisibleUpdated =
					featurePlayerSuperJump =
					featurePlayermoneyDrop =
					featurePlayermoneyDrop1 =
					featureWeaponNoReload =
					featureWeaponFireAmmo =
					featureVehSuperRPM =
					featureWeaponExplosiveAmmo =
					featureWeaponExplosiveMelee =
					featureWeaponVehRockets =
					featureWeaponVehGreen =
					featureWeaponVehFlare =
					featureWeaponVehTank =
					featureWeaponVehRed =
					featureWeaponVehSnow =
					featureWeaponOneHit =
					featureWepModAnimalGun =
					featureWeaponVehFire =
					featureVehInvincible =
					featureVehInvincibleUpdated =
					featureVehSlide =
					featureVehScrollingText =
					featureVehInvincibleWheels =
					featureWepModVehGun =
					featureVehInvincibleWheelsUpdated =
					featureVehSeatbelt =
					featureVehRainbow =
					featureVehRainbowTire =
					featureWeaponRainbowTint =
					featurePlayerUltraJump =
					featureWeaponWhaleGun =
					featureVehSeatbeltUpdated =
					featureVehSpeedBoost =
					featureWorldMoonGravity =
					featureMiscFlyMode =
					featureVisionNight =
					featureVisionDrunk =
					featureVisionDrunkUpdated =
					featureVisionThermal =
					featureTimePaused =
					featureTimePausedUpdated =
					featureTimeSynced =
					featurePedestrianInvincibleBodyguard =
					featurePedestrianInvincibleBodyguardUpdate =
					featureWeatherWind =
					featurePlayerESPTrace =
					featureWeatherPers =
					featureMiscLockRadio =
					featureMiscHideHud = false;
				    featureMiscMobile = false;
				    featureVehAllMod = false;
				    featureVehStickyCar = false;
                    featureWorldRandomCops =
					featureWorldRandomTrains =
					featureVehWrapInSpawned =
					featureWorldRandomBoats =
					featureWorldGarbageTrucks = true;
				    featureWorldCrazywaves =
                    skinchanger_used = false;
				break;
			case 14:
				process_credit_menu();
				break;
			case 15:
				STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_solomon3");
				GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_solomon3");
				GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_trev4_747_blood_impact", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);

				STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_finalec2");
				GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_finalec2");
				GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_finale2_blood_entry", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);

				STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_michael2");
				GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_michael2");
				GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_mich2_blood_stab", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
				break;
            case 16:
				return ;
				break;


			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexMain == 0)
						activeLineIndexMain = lineCount;
					activeLineIndexMain--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexMain++;
						if (activeLineIndexMain == lineCount)
							activeLineIndexMain = 0;
						waitTime = 150;
					}
	}
}

void reset_globals()
{
	    activeLineIndexMain =
		activeLineIndexPlayer =
		skinchangerActiveLineIndex =
		skinchangerActiveItemIndex =
		teleportActiveLineIndex =
		activeLineIndexWeapon =
		activeLineIndexVeh =
		carspawnActiveLineIndex =
		carspawnActiveItemIndex =
		activeLineIndexWorld =
		activeLineIndexWeather = 0;
		weaponGravityGun =
		weaponDeleteObjects =


		featurePlayerSuperRun =
		featurePlayerHeadDisplay =
		featurePlayerBlipCone =
		featurePlayerNotifications =
		featureShowVoiceChatSpeaker =
	    featurePlayerInvincible =
		featurePlayerInvincibleUpdated =
		featurePlayerNeverWanted =
		featurePlayerIgnored =
		featurePlayerIgnoredUpdated =
		featureRainMoney =
		featurePlayerUnlimitedAbility =
		featurePlayerNoNoise =
		featurePlayerNoNoiseUpdated =
		featurePlayerFastSwim =
		featurePlayerFastSwimUpdated =
		featurePlayerFastRun =
		featurePlayerFastRunUpdated =
		featurePlayerInvisible =
		featurePlayerInvisibleUpdated =
		featurePlayerSuperJump =
		featurePlayermoneyDrop =
		featurePlayermoneyDrop1 =
		featureWeaponNoReload =
		featureWeaponFireAmmo =
		featureVehSuperRPM =
		featureWeaponExplosiveAmmo =
		featureWeaponExplosiveMelee =
		featureWeaponVehRockets =
		featureWeaponVehGreen =
		featureWeaponVehFlare =
		featureWeaponVehTank =
		featureWeaponVehRed =
		featureWeaponVehSnow =
		featureWeaponOneHit =
		featureWepModAnimalGun =
		featureWeaponVehFire =
		featureVehInvincible =
		featureVehInvincibleUpdated =
		featureVehSlide =
		featureVehScrollingText =
		featureVehInvincibleWheels =
		featureWepModVehGun =
		featureVehInvincibleWheelsUpdated =
		featureVehSeatbelt =
		featureVehRainbow =
		featureVehRainbowTire =
		featureWeaponRainbowTint =
		featurePlayerUltraJump =
		featureWeaponWhaleGun =
		featureVehSeatbeltUpdated =
		featureVehSpeedBoost =
		featureWorldMoonGravity =
		featureMiscFlyMode =
		featureVisionNight =
		featureVisionDrunk =
		featureVisionDrunkUpdated =
		featureVisionThermal =
		featureTimePaused =
		featureTimePausedUpdated =
		featureTimeSynced =
		featurePedestrianInvincibleBodyguard =
		featurePedestrianInvincibleBodyguardUpdate =
		featureWeatherWind =
		featurePlayerESPTrace =
		featureWeatherPers =
		featureMiscLockRadio =
		featureMiscHideHud = false;
	    featureMiscMobile = false;
	    featureVehAllMod = false;
	    featureVehStickyCar = false;

	    featureWorldRandomCops =
		featureWorldRandomTrains =
		featureVehWrapInSpawned =
		featureWorldRandomBoats =
		featureWorldGarbageTrucks = true;
	    featureWorldCrazywaves =

		skinchanger_used = false;
}

void main()
{
	reset_globals();

	while (true)
	{
	
			menu_beep();
			process_main_menu();
		

		update_features();
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
