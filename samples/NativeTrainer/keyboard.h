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

// parameters are the same as with aru's ScriptHook for IV
void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);

bool IsKeyDown(DWORD key);
bool IsKeyJustUp(DWORD key, bool exclusive = true);
void ResetKeyState(DWORD key);