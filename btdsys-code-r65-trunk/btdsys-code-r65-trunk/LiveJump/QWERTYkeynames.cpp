#include "stdafx.h"
#include "QWERTYkeynames.h"

char *KeyNames(int n)
{
	switch(n)
	{
	case 0x01: return "LBUTTON";
	case 0x02: return "RBUTTON";
	case 0x03: return "CANCEL";
	case 0x04: return "MBUTTON";

	case 0x08: return "Backspace";
	case 0x09: return "Tab";

	case 0x0C: return "CLEAR";
	case 0x0D: return "Return";

	case 0x10: return "Shift";
	case 0x11: return "Control";
	case 0x12: return "Menu";
	case 0x13: return "Pause";
	case 0x14: return "Caps Lock";

	case 0x15: return "KANA/VK_HANGEUL/VK_HANGUL";
	case 0x17: return "JUNJA";
	case 0x18: return "FINAL";
	case 0x19: return "HANJA/VK_KANJI";

	case 0x1B: return "Esc";

	case 0x1C: return "CONVERT";
	case 0x1D: return "NONCONVERT";
	case 0x1E: return "ACCEPT";
	case 0x1F: return "MODECHANGE";

	case 0x20: return "SPACE";
	case 0x21: return "Page Up";
	case 0x22: return "Page Down";
	case 0x23: return "End";
	case 0x24: return "Home";
	case 0x25: return "Left";
	case 0x26: return "Up";
	case 0x27: return "Right";
	case 0x28: return "Down";
	case 0x29: return "SELECT";
	case 0x2A: return "PRINT";
	case 0x2B: return "EXECUTE";
	case 0x2C: return "SNAPSHOT";
	case 0x2D: return "Insert";
	case 0x2E: return "Delete";
	case 0x2F: return "HELP";

	case 0x30: return "0";
	case 0x31: return "1";
	case 0x32: return "2";
	case 0x33: return "3";
	case 0x34: return "4";
	case 0x35: return "5";
	case 0x36: return "6";
	case 0x37: return "7";
	case 0x38: return "8";
	case 0x39: return "9";

	case 0x41: return "A";
	case 0x42: return "B";
	case 0x43: return "C";
	case 0x44: return "D";
	case 0x45: return "E";
	case 0x46: return "F";
	case 0x47: return "G";
	case 0x48: return "H";
	case 0x49: return "I";
	case 0x4a: return "J";
	case 0x4b: return "K";
	case 0x4c: return "L";
	case 0x4d: return "M";
	case 0x4e: return "N";
	case 0x4f: return "O";
	case 0x50: return "P";
	case 0x51: return "Q";
	case 0x52: return "R";
	case 0x53: return "S";
	case 0x54: return "T";
	case 0x55: return "U";
	case 0x56: return "V";
	case 0x57: return "W";
	case 0x58: return "X";
	case 0x59: return "Y";
	case 0x5a: return "Z";

	case 0x5B: return "Left Win";
	case 0x5C: return "Right Win";
	case 0x5D: return "Apps";

	case 0x60: return "NUMPAD0";
	case 0x61: return "NUMPAD1";
	case 0x62: return "NUMPAD2";
	case 0x63: return "NUMPAD3";
	case 0x64: return "NUMPAD4";
	case 0x65: return "NUMPAD5";
	case 0x66: return "NUMPAD6";
	case 0x67: return "NUMPAD7";
	case 0x68: return "NUMPAD8";
	case 0x69: return "NUMPAD9";
	case 0x6A: return "MULTIPLY";
	case 0x6B: return "ADD";
	case 0x6C: return "SEPARATOR";
	case 0x6D: return "SUBTRACT";
	case 0x6E: return "DECIMAL";
	case 0x6F: return "DIVIDE";
	case 0x70: return "F1";
	case 0x71: return "F2";
	case 0x72: return "F3";
	case 0x73: return "F4";
	case 0x74: return "F5";
	case 0x75: return "F6";
	case 0x76: return "F7";
	case 0x77: return "F8";
	case 0x78: return "F9";
	case 0x79: return "F10";
	case 0x7A: return "F11";
	case 0x7B: return "F12";
	case 0x7C: return "F13";
	case 0x7D: return "F14";
	case 0x7E: return "F15";
	case 0x7F: return "F16";
	case 0x80: return "F17";
	case 0x81: return "F18";
	case 0x82: return "F19";
	case 0x83: return "F20";
	case 0x84: return "F21";
	case 0x85: return "F22";
	case 0x86: return "F23";
	case 0x87: return "F24";

	case 0x90: return "Num Lock";
	case 0x91: return "Scroll Lock";

	case 0xE5: return "PROCESSKEY";

	case 0xF6: return "ATTN";
	case 0xF7: return "CRSEL";
	case 0xF8: return "EXSEL";
	case 0xF9: return "EREOF";
	case 0xFA: return "PLAY";
	case 0xFB: return "ZOOM";
	case 0xFC: return "NONAME";
	case 0xFD: return "PA1";
	case 0xFE: return "OEM_CLEAR";
	default: return NULL;
	}
}
