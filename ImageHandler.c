/*
 * A class to represent an image and calculate which
 * diods should light up given the current position.
 */

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "ImageHandler.h"
#include "OutputInterpreter.h"

#define IMG_LEN (imgs[currImg][0])

static int currImg = 16;
static int lastIndex = 0;

#define NUM_IMAGES 22


unsigned int *imgs[NUM_IMAGES] =
		{
			//"  TIFO!  "
			(unsigned int[])
			{49, 0x00000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xFFFFFFFF, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xC0060000, 0xC0060000, 0xC0060000, 0xC0060000, 0xC0060000, 0xC0060000, 0xC0060000, 0xC0060000, 0xC0000000, 0xC0000000, 0x00000000, 0x00000000, 0x007FFE00, 0x07C003E0, 0x1C000038, 0x3000000C, 0x60000006, 0x40000002, 0xC0000003, 0x80000001, 0x80000001, 0xC0000003, 0x40000002, 0x60000006, 0x3000000C, 0x1C000038, 0x07C003E0, 0x007FFE00, 0x00000000},
			// "DATA"
			(unsigned int[])
			{55,0x00000000, 0xFFFFFFFF, 0xC0000003, 0xC0000003, 0xC0000003, 0xC0000003, 0xC0000003, 0x60000006, 0x60000006, 0x60000006, 0x3000000C, 0x3000000C, 0x18000018, 0x0F0000F0, 0x01FFFF80, 0x00000000, //D
				0x00000000, 0x01FFFFFF, 0x07030000, 0x1C030000, 0x70030000, 0xC0030000, 0xC0030000, 0x70030000, 0x1C030000, 0x07030000, 0x01FFFFFF, 0x00000000, //A
				0x00000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xFFFFFFFF, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0x00000000, //T
				0x00000000, 0x01FFFFFF, 0x07030000, 0x1C030000, 0x70030000, 0xC0030000, 0xC0030000, 0x70030000, 0x1C030000, 0x07030000, 0x01FFFFFF, 0x00000000}, //A

			//Double Romb "<<>>"
			(unsigned int[])
			{32,0x00018000, 0x00024000, 0x00042000, 0x00081000, 0x00100800, 0x00200400, 0x00400200, 0x00800100, //<
				0x01018080, 0x02024040, 0x04042020, 0x08081010, 0x10100808, 0x20200404, 0x40400202, 0x80800101, //<<
				0x80800101, 0x40400202, 0x20200404, 0x10100808, 0x08081010, 0x04042020, 0x02024040, 0x01018080, //>>
				0x00800100, 0x00400200, 0x00200400, 0x00100800, 0x00081000, 0x00042000, 0x00024000, 0x00018000},//>
			//"  HEJA  "
			(unsigned int[])
			{53,0x00000000, 0x00000000, 0xFFFFFFFF, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0000003, 0xC0000003, 0x00000003, 0x00000000, 0x00000000, 0x00000FE0, 0x00000038, 0x0000000C, 0x00000006, 0x00000003, 0x00000003, 0x00000003, 0x00000006, 0x0000000C, 0x80000038, 0xFFFFFFE0, 0x80000000, 0x00000000, 0x00000000, 0x01FFFFFF, 0x07030000, 0x1C030000, 0x70030000, 0xC0030000, 0xC0030000, 0x70030000, 0x1C030000, 0x07030000, 0x01FFFFFF, 0x00000000},
			//Cursor
			(unsigned int[])
			{20, 0x00000000, 0xFFFFFFC0, 0x40000080, 0x20000100, 0x10000200, 0x08000400, 0x04000800, 0x02001800, 0x01000600, 0x00800180, 0x00406060, 0x00305818, 0x00004606, 0x00084181, 0x00044061, 0x00024019, 0x00014006, 0x0000C000, 0x00004000, 0x00000000},
			// Dickbutt
			(unsigned int[])
			{31, 0x00000000, 0x003E0000, 0x07C1C000, 0x0F403000, 0x3D400C00, 0x6FC00218, 0x42C00114, 0x85C0010B, 0x804000B3, 0x874000DC, 0x8F4018E0, 0x8D47E840, 0x8F47F840, 0x40003050, 0x3FFC1068, 0x00038054, 0x00000074, 0x00004003, 0x0002407E, 0x00028240, 0x00029040, 0x00018040, 0x00024040, 0x00064080, 0x00082100, 0x00709E00, 0x0041C800, 0x0036C800, 0x00087800, 0x00004000, 0x00000000},
			// "Chalmers"
			(unsigned int[])
			{118, 0x00000000, 0x03FFFFC0, 0x0C000030, 0x10000008, 0x20000004, 0x40000002, 0x40000002, 0x80000001, 0x80000001, 0x80000001, 0x40000002, 0x40000002, 0x20000004, 0x10000008, 0x0C000030, 0x038001C0, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x01FFFFFF, 0x07030000, 0x1C030000, 0x70030000, 0xC0030000, 0xC0030000, 0x70030000, 0x1C030000, 0x07030000, 0x01FFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x70000000, 0x1C000000, 0x07000000, 0x01C00000, 0x00700000, 0x000C0000, 0x00070000, 0x000C0000, 0x00700000, 0x01C00000, 0x07000000, 0x1C000000, 0x70000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0000003, 0xC0000003, 0x00000003, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0xC0030000, 0x4003C000, 0x60067800, 0x300C0F00, 0x180801E0, 0x0E70003C, 0x03C00007, 0x00000000, 0x00000000, 0x07F000E0, 0x1C0C0038, 0x3006000C, 0x60020006, 0x40030002, 0xC0010003, 0x80010001, 0x80010001, 0x80010001, 0xC0010003, 0x40018002, 0x60008006, 0x3000C00C, 0x1C007038, 0x07001FE0, 0x00000000},
			//Triforce
			(unsigned int[])
			{65, 0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000F, 0x0000001F, 0x0000003F, 0x0000007F, 0x000000FF, 0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF, 0x00001FFF, 0x00003FFF, 0x00007FFF, 0x0000FFFF, 0x00017FFF, 0x00033FFF, 0x00071FFF, 0x000F0FFF, 0x000F07FF, 0x003F03FF, 0x007F01FF, 0x00FF00FF, 0x01FF007F, 0x03FF003F, 0x07FF001F, 0x0FFF000F, 0x1FFF0007, 0x3FFF0003, 0x7FFF0001, 0xFFFF0000, 0x7FFF0001, 0x3FFF0003, 0x1FFF0007, 0x0FFF000F, 0x07FF001F, 0x03FF003F, 0x01FF007F, 0x00FF00FF, 0x007F01FF, 0x003F03FF, 0x000F07FF, 0x000F0FFF, 0x00071FFF, 0x00033FFF, 0x00017FFF, 0x0000FFFF, 0x00007FFF, 0x00003FFF, 0x00001FFF, 0x00000FFF, 0x000007FF, 0x000003FF, 0x000001FF, 0x000000FF, 0x0000007F, 0x0000003F, 0x0000001F, 0x0000000F, 0x00000007, 0x00000003, 0x00000001, 0x00000000},
			// "DOMMARJÄVEL"
			(unsigned int[])
			{161, 0x00000000, 0xFFFFFFFF, 0xC0000003, 0xC0000003, 0xC0000003, 0xC0000003, 0xC0000003, 0x60000006, 0x60000006, 0x60000006, 0x3000000C, 0x3000000C, 0x18000018, 0x0F0000F0, 0x01FFFF80, 0x00000000, 0x00000000, 0x007FFE00, 0x07C003E0, 0x1C000038, 0x3000000C, 0x60000006, 0x40000002, 0xC0000003, 0x80000001, 0x80000001, 0xC0000003, 0x40000002, 0x60000006, 0x3000000C, 0x1C000038, 0x07C003E0, 0x007FFE00, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x70000000, 0x1C000000, 0x07000000, 0x01C00000, 0x00700000, 0x000C0000, 0x00070000, 0x000C0000, 0x00700000, 0x01C00000, 0x07000000, 0x1C000000, 0x70000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x70000000, 0x1C000000, 0x07000000, 0x01C00000, 0x00700000, 0x000C0000, 0x00070000, 0x000C0000, 0x00700000, 0x01C00000, 0x07000000, 0x1C000000, 0x70000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x01FFFFFF, 0x07030000, 0x1C030000, 0x70030000, 0xC0030000, 0xC0030000, 0x70030000, 0x1C030000, 0x07030000, 0x01FFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0xC0030000, 0x4003C000, 0x60067800, 0x300C0F00, 0x1C3801E0, 0x07F0003C, 0x00000007, 0x00000000, 0x00000000, 0x00000FE0, 0x00000038, 0x0000000C, 0x00000006, 0x00000003, 0x00000003, 0x00000003, 0x00000006, 0x0000000C, 0x80000038, 0xFFFFFFE0, 0x80000000, 0x00000000, 0x00000000, 0xC1FFFFFF, 0xC7030000, 0x1C030000, 0x70030000, 0xC0030000, 0xC0030000, 0x70030000, 0x1C030000, 0xC7030000, 0xC1FFFFFF, 0x00000000, 0x00000000, 0xFFFF8000, 0x0000F000, 0x00001E00, 0x000003C0, 0x00000078, 0x0000000F, 0x00000078, 0x000003C0, 0x00001E00, 0x0000F000, 0xFFFF8000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0000003, 0xC0000003, 0x00000003, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000000},
			// "SOLNA TATTARE"
			(unsigned int[])
			{61, 0x00000000, 0x3C084000, 0x42044000, 0x81024000, 0x81027FFF, 0x81024000, 0x40844000, 0x30784000, 0x00000000, 0x00000000, 0x3FF803FF, 0x40040C80, 0x80023080, 0x80024080, 0x80023080, 0x40040C80, 0x3FF803FF, 0x00000000, 0x00000000, 0xFFFE4000, 0x00024000, 0x00024000, 0x00027FFF, 0x00024000, 0x00024000, 0x00004000, 0x00000000, 0xFFFE0000, 0x30004000, 0x0C004000, 0x03004000, 0x00C07FFF, 0x00304000, 0x000C4000, 0xFFFE4000, 0x00000000, 0x00000000, 0x07FE03FF, 0x19000C80, 0x61003080, 0x81004080, 0x61003080, 0x19000C80, 0x07FE03FF, 0x07FE0000, 0x19000000, 0x61007FFF, 0x81004100, 0x610041C0, 0x19004130, 0x07FE220C, 0x07FE1C03, 0x00000000, 0x00000000, 0x00007FFF, 0x00004081, 0x00004081, 0x00004081, 0x00004001, 0x00004001, 0x00000000},
			// "SWOSH"
			//(((unsigned int[])
			//{84, 0x00000000, 0x07F000E0, 0x1C0C0038, 0x3006000C, 0x60020006, 0x40030002, 0xC0010003, 0x80010001, 0x80010001, 0x80010001, 0xC0010003, 0x40018002, 0x60008006, 0x3000C00C, 0x1C007038, 0x07001FE0, 0x00000000, 0x00000000, 0xFFFFF800, 0x00000F00, 0x000001E0, 0x0000003C, 0x00000007, 0x00000007, 0x0000003C, 0x000001E0, 0x0FFFFF00, 0x000001E0, 0x0000003C, 0x00000007, 0x00000007, 0x0000003C, 0x000001E0, 0x00000F00, 0xFFFFF800, 0x00000000, 0x00000000, 0x000FFE00, 0x01F001E0, 0x07000038, 0x0C00000C, 0x58000006, 0x50000002, 0xF0000003, 0xA0000001, 0xA0000001, 0xF0000003, 0x50000002, 0x58000006, 0x0C00000C, 0x07000038, 0x07C003E0, 0x007FFE00, 0x00000000, 0x00000000, 0x07F000E0, 0x1C0C0038, 0x3006000C, 0x60020006, 0x40030002, 0xC0010003, 0x80010001, 0x80010001, 0x80010001, 0xC0010003, 0x40018002, 0x60008006, 0x3000C00C, 0x1C007038, 0x07001FE0, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0xFFFFFFFF, 0x00000000},
			// "HENRIK"
			(unsigned int[])
			{70, 0x00000000, 0xFFFFFFFF, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0000003, 0xC0000003, 0x00000003, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x3C000000, 0x07800000, 0x00F00000, 0x000E0000, 0x0003C000, 0x00007800, 0x00000F00, 0x000001E0, 0x0000003C, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0xC0030000, 0x4003C000, 0x60067800, 0x300C0F00, 0x180801E0, 0x0E70003C, 0x03C00007, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00003800, 0x0000E000, 0x0003C000, 0x000E7000, 0x00381C00, 0x00F00700, 0x038001C0, 0x0E000070, 0x3800001C, 0xE0000007, 0x00000000},
			// "PATRIK"
			(unsigned int[])
			{73, 0x00000000, 0xFFFFFFFF, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0xC0030000, 0x40030000, 0x60060000, 0x300C0000, 0x1C380000, 0x07F00000, 0x00000000, 0x00000000, 0x01FFFFFF, 0x07030000, 0x1C030000, 0x70030000, 0xC0030000, 0xC0030000, 0x70030000, 0x1C030000, 0x07030000, 0x01FFFFFF, 0x00000000, 0x00000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xFFFFFFFF, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0xC0000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0xC0030000, 0x4003C000, 0x60067800, 0x300C0F00, 0x1C3801E0, 0x07F0003C, 0x00000007, 0x00000000, 0x00000000, 0xFFFFFFFF ,0x00000000, 0x00000000, 0xFFFFFFFF, 0x00003800, 0x0000E000, 0x0003C000, 0x000E7000, 0x00381C00, 0x00F00700, 0x038001C0, 0x0E000070, 0x3800001C, 0xE0000007, 0x00000000, 0x00000000},
			// "ALBIN"
			(unsigned int[])
			{57, 0x00000000, 0x01FFFFFF, 0x07030000, 0x1C030000, 0x70030000, 0xC0030000, 0xC0030000, 0x70030000, 0x1C030000, 0x07030000, 0x01FFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x80010001, 0x80010001, 0x80010001, 0x80010001, 0x80010001, 0x80010001, 0xC0038003, 0x40028002, 0x6006C006, 0x300C600C, 0x1C383838, 0x07F00FE0, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x3C000000, 0x07800000, 0x00F00000, 0x000E0000, 0x0003C000, 0x00007800, 0x00000F00, 0x000001E0, 0x0000003C, 0xFFFFFFFF, 0x00000000},
			// "VANESSA"
			(unsigned int[])
			{97, 0x00000000, 0xFFFF8000, 0x0000F000, 0x00001E00, 0x000003C0, 0x00000078, 0x0000000F, 0x00000078, 0x000003C0, 0x00001E00, 0x0000F000, 0xFFFF8000, 0x00000000, 0x00000000, 0x01FFFFFF, 0x07030000, 0x1C030000, 0x70030000, 0xC0030000, 0xC0030000, 0x70030000, 0x1C030000, 0x07030000, 0x01FFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x3C000000, 0x07800000, 0x00F00000, 0x000E0000, 0x0003C000, 0x00007800, 0x00000F00, 0x000001E0, 0x0000003C, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0000003, 0xC0000003, 0x00000003, 0x00000000, 0x00000000, 0x07F000E0, 0x1C0C0038, 0x3006000C, 0x60020006, 0x40030002, 0xC0010003, 0x80010001, 0x80010001, 0x80010001, 0xC0010003, 0x40018002, 0x60008006, 0x3000C00C, 0x1C007038, 0x07001FE0, 0x00000000, 0x00000000, 0x07F000E0, 0x1C0C0038, 0x3006000C, 0x60020006, 0x40030002, 0xC0010003, 0x80010001, 0x80010001, 0x80010001, 0xC0010003, 0x40018002, 0x60008006, 0x3000C00C, 0x1C007038, 0x07001FE0, 0x00000000, 0x00000000, 0x01FFFFFF, 0x07030000, 0x1C030000, 0x70030000, 0xC0030000, 0xC0030000, 0x70030000, 0x1C030000, 0x07030000, 0x01FFFFFF, 0x00000000},
			// "JESPER"
			(unsigned int[])
			{86, 0x00000000, 0x00000FE0, 0x00000038, 0x0000000C, 0x00000006, 0x00000003, 0x00000003, 0x00000003, 0x00000006, 0x0000000C, 0x80000038, 0xFFFFFFE0, 0x80000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0000003, 0xC0000003, 0x00000003, 0x00000000, 0x00000000, 0x07F000E0, 0x1C0C0038, 0x3006000C, 0x60020006, 0x40030002, 0xC0010003, 0x80010001, 0x80010001, 0x80010001, 0xC0010003, 0x40018002, 0x60008006, 0x3000C00C, 0x1C007038, 0x07001FE0, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0xC0030000, 0x40030000, 0x60060000, 0x300C0000, 0x1C380000, 0x07F00000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0000003, 0xC0000003, 0x00000003, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0x80010000, 0xC0030000, 0x4003C000, 0x60067800, 0x300C0F00, 0x1C3801E0, 0x07F0003C, 0x00000007, 0x00000000},
			// "GAIS"
			(unsigned int[])
			{51, 0x00000000, 0x07FFFFE0, 0x1C000038, 0x3000000C, 0x60000006, 0x40000002, 0xC0000003, 0x80003001, 0x80003001, 0x80003001, 0xC0003003, 0x40003002, 0x60003006, 0x3000300C, 0x1C003038, 0x07803FFF, 0x00000000, 0x00000000, 0x01FFFFFF, 0x07030000, 0x1C030000, 0x70030000, 0xC0030000, 0xC0030000, 0x70030000, 0x1C030000, 0x07030000, 0x01FFFFFF, 0x00000000, 0x00000000, 0x80000001, 0xFFFFFFFF, 0x80000001, 0x00000000, 0x00000000, 0x07F000E0, 0x1C0C0038, 0x3006000C, 0x60020006, 0x40030002, 0xC0010003, 0x80010001, 0x80010001, 0x80010001, 0xC0010003, 0x40018002, 0x60008006, 0x3000C00C, 0x1C007038, 0x07001FE0, 0x00000000},
			//cactus
			(unsigned int[])
			{18, 0x00000000, 0x003FFF80, 0x00400080, 0x00400080, 0x003F80FF, 0x00400000, 0x00400000, 0x7FFF8000, 0x90000000, 0x90000000, 0x7FFF8000, 0x00400000, 0x00400000, 0x003F80FF, 0x00400080, 0x00400080, 0x003FFF80, 0x00000000},
			//android
			(unsigned int[])
			{25, 0x00000000, 0x000FF000, 0x003FF800, 0x000FF000, 0x00000000, 0x01BFFE00, 0x87BFFF00, 0x4FBFFF00, 0x3FBFFFFE, 0x1BBFFFFF, 0x3FBFFFFE, 0x3FBFFF00, 0x3FBFFF00, 0x3FBFFF00, 0x3FBFFFFE, 0x1BBFFFFF, 0x3FBFFFFE, 0x4FBFFF00, 0x87BFFF00, 0x01BFFE00, 0x00000000, 0x000FF000, 0x003FF800, 0x000FF000, 0x00000000},


			//Smiley
			(unsigned int[])
			{34,0x0000,
				0x000ff000, 0x007ffe00, 0x00ffff00, 0x03fff3c0,	//Leftface
				0x07fff1e0, 0x0ffff8f0, 0x1ffff878, 0x1ffff878,
				0x3e03f83c, 0x7cf9f81e, 0x79fcf81e, 0x79bcf81e,
				0xfcf9f80f, 0xfe03f80f, 0xfffff80f, 0xfffff80f,
				0xfffff80f, 0xfffff80f, 0xfe03f80f, 0xfcf9f80f,	//Rightface
				0x79fcf81e, 0x79bcf81e, 0x7cf9f81e, 0x3e03f83c,
				0x1ffff878, 0x1ffff878, 0x0ffff8f0, 0x07fff1e0,
				0x03fff3c0, 0x00ffff00, 0x007ffe00, 0x000ff000,
				0x0000},
			//Thumbs up
			//(unsigned int[])
			//{18,0x0000, 0x00fe, 0x00fe, 0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x7fff,
			//	0xffff, 0xffff, 0x73ff, 0x03ff, 0x03fe, 0x03fc, 0x01f0, 0x00c0, 0x0000},
			//Thumbs down
			//(unsigned int[])
			//{18,0x0000, 0x7f00, 0x7f00, 0xff80, 0xffc0, 0xffe0, 0xfff0, 0xfff8, 0xfffe,
			//	0xffff, 0xffff, 0xffce, 0xffc0, 0x7fc0, 0x3fc0, 0x0f80, 0x0300, 0x0000},
			//"HEJ"
			(unsigned int[])
			{39, 0x00000000, 0xFFFFFFFF, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0x00018000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0018003, 0xC0000003, 0xC0000003, 0x00000003, 0x00000000, 0x00000000, 0x80000FE0, 0x80000038, 0x8000000C, 0x80000006, 0x80000003, 0x80000003, 0x80000003, 0x80000006, 0x8000000C, 0x80000038, 0xFFFFFFE0, 0x00000000},
			//Yingyang
			(unsigned int[])
			{34, 0x00000000, 0x0007E000, 0x003FFC00, 0x00FFFF00, 0x03FFFFC0, 0x07FFFFE0, 0x0FFFFFF0, 0x1FFFFFF8, 0x1FFFFFF8, 0x3FFFFC7C, 0x3FFFF01C, 0x7FFFE00E, 0x7FFFC006, 0x7FFF8002, 0xFE7F8181, 0xFC3F03C1, 0xF80F07E1, 0xF80F07E1, 0xFC3F03C1, 0xFE7E0181, 0x7FFE0002, 0x5FFC0002, 0x4FF80002, 0x27F00004, 0x21C00004, 0x10000008, 0x10000008, 0x08000010, 0x04000020, 0x030000C0, 0x00C00300, 0x00381C00, 0x0007E000, 0x00000000},
			//Football
			(unsigned int[])
			{34,0x0000,
				0x000ff000, 0x007f8e00, 0x00fe6100, 0x037818c0,
				0x04200420, 0x08200210, 0x102003a8, 0x102007e8,
				0x204007e4, 0x60400ff2, 0x60601ff2, 0x60f067fa,
				0xe1fd83e7, 0xfbfe01c1, 0xc7fc0081, 0xc3fc0081,
				0xc1f80081, 0x80f80101, 0x80e40101, 0x80820101,
				0x41010202, 0x41008f02, 0x42007f82, 0x22003f84,
				0x1c003ff8, 0x1f003f88, 0x0f81ff10, 0x07c60e20,
				0x03f802c0, 0x00c00100, 0x00700e00, 0x000ff000,
				0x0000}
		};

void initImageHandler(){
	initOutputs();
}

/*
 * This function determines which part of the image or message to print out.
 * Also sends the determined column to the updateOutputs function.
 */
void updateImage(float pos) {

	// Compute which column to print
	int currIndex = pos*IMG_LEN;

	// If index is out of range (lower), set index to first possible position
	if (currIndex < 0) {
		currIndex = 0;
	// If index is out of range (higher), set index to last possible position
	} else if (currIndex >= IMG_LEN) {
		currIndex = IMG_LEN - 1;
	}
	// If current index differs from last index
	if (currIndex != lastIndex) {
		// Set current index to last index
		lastIndex = currIndex;
		// Send current column to output
		updateOutputs(imgs[currImg][currIndex+1]); //First value is the length of the array
	}
}

void accelDrawer(int16_t val) {
	int nbrOfLights = (((float)val)/32767)*16;
	int pixels = 0;
	int i = (nbrOfLights<0 ? -nbrOfLights : nbrOfLights);
	for(; i>0;i--) {
		if (nbrOfLights<0) {
			pixels <<= 1;
			pixels |= 0x10000;
		} else {
			pixels >>= 1;
			pixels |= 0x8000;
		}
	}
	updateOutputs(pixels);
}

void nextImage() {
	currImg = (currImg+1)%NUM_IMAGES;
}

void previousImage() {
	currImg = (currImg+NUM_IMAGES-1)%NUM_IMAGES;
}
