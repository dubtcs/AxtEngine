#pragma once

#include <axt/Core.h>

namespace axt
{

	// using keycodes from glfw3.h

	using Keycode = uint16_t;

	namespace Key
	{

		enum : Keycode
		{

			// US Letters
			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,

			// Numbers and "operations"
			Space			= 32,
			Apostrophe		= 39, /* ' */
			Comma			= 44, /* , */
			Minus			= 45, /* - */
			Period			= 46,/* . */
			Slash			= 47, /* / */
			N0				= 48,
			N1				= 49,
			N2				= 50,
			N3				= 51,
			N4				= 52,
			N5				= 53,
			N6				= 54,
			N7				= 55,
			N8				= 56,
			N9				= 57,
			Semicolon		= 59, /* ; */
			Equal			= 61, /* = */

			BracketLeft		= 91,  /* [ */
			Backslash		= 92,  /* \ */
			BracketRight	= 93,  /* ] */
			GraveAccent		= 96,  /* ` */
			World1			= 161, /* non-US #1 */
			World2			= 162, /* non-US #2 */

			Escape          =  256,
			Enter           =  257,
			Tab             =  258,
			Backspace       =  259,
			Insert          =  260,
			Delete          =  261,
			Right           =  262,
			Left            =  263,
			Down            =  264,
			Up              =  265,
			PageUp			=  266,
			PageDown        =  267,
			Home            =  268,
			End             =  269,
			CapsLock        =  280,
			ScrollLock      =  281,
			NumLock         =  282,
			PrintScreen     =  283,
			Pause           =  284,
			F1              =  290,
			F2              =  291,
			F3              =  292,
			F4              =  293,
			F5              =  294,
			F6              =  295,
			F7              =  296,
			F8              =  297,
			F9              =  298,
			F10             =  299,
			F11             =  300,
			F12             =  301,
			F13             =  302,
			F14             =  303,
			F15             =  304,
			F16             =  305,
			F17             =  306,
			F18             =  307,
			F19             =  308,
			F20             =  309,
			F21             =  310,
			F22             =  311,
			F23             =  312,
			F24             =  313,
			F25             =  314,
			KP0             =  320,
			KP1             =  321,
			KP2             =  322,
			KP3             =  323,
			KP4             =  324,
			KP5             =  325,
			KP6             =  326,
			KP7             =  327,
			KP8             =  328,
			KP9             =  329,
			KPDecimal       =  330,
			KPDicide        =  331,
			KPMultiply      =  332,
			KPSubstract     =  333,
			KPAdd           =  334,
			KPEnter         =  335,
			KPEqual         =  336,
			ShiftLeft       =  340,
			ControlLeft     =  341,
			AltLeft         =  342,
			SuperLeft       =  343,
			ShiftRight      =  344,
			ControlRight    =  345,
			AltRight        =  346,
			SuperRight      =  347,
			Menu            =  348,

			Mouse1			= 0,
			Mouse2			= 1,
			Mouse3			= 2,
			Mouse4			= 3,
			Mouse5			= 4,
			Mouse6			= 5,
			Mouse7			= 6,
			Mouse8			= 7,
			MouseLast		= Mouse8,
			MouseLeft		= Mouse1,
			MouseRight		= Mouse2,
			MouseMiddle		= Mouse3

		};

	}

}
