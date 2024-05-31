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

#pragma once

#include <windows.h>

typedef DWORD Void;
typedef DWORD Any;
typedef DWORD uint;
typedef DWORD Hash;
typedef int Entity;
typedef int Player;
typedef int FireId;
typedef int Ped;
typedef int Vehicle;
typedef int Cam;
typedef int CarGenerator;
typedef int Group;
typedef int Train;
typedef int Pickup;
typedef int Object;
typedef int Weapon;
typedef int Interior;
typedef int Blip;
typedef int Texture;
typedef int TextureDict;
typedef int CoverPoint;
typedef int Camera;
typedef int TaskSequence;
typedef int ColourIndex;
typedef int Sphere;
typedef int ScrHandle;

#pragma pack(push, 1)
typedef struct
{
	float x;
	DWORD _paddingx;
	float y;
	DWORD _paddingy;
	float z;
	DWORD _paddingz;
} Vector3;
#pragma pack(pop)