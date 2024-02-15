/*******************************************************************************
 * Copyright 2019 Microchip FPGA Embedded Systems Solutions.
 *
 * SPDX-License-Identifier: MIT
 *
 * @file mss_ddr_test_pattern.c
 * @author Microchip FPGA Embedded Systems Solutions
 * @brief 
 *
 */

#include <stdint.h>

const uint32_t ddr_test_pattern[768] =
{
    0x48b301bc, 0x79330115, 0xf5330139, 0x693301bc,
  0x893b00a9, 0x093b0128, 0x0d3b00c9, 0x551b00cd,
  0x161b0077, 0x8d510197, 0x0127581b, 0x00e7161b,
  0x01066633, 0x561b8d31, 0x8d310037, 0x8f3b9da9,
  0xd61b01e5, 0x959b0112, 0xd51b00f2, 0x8e4d0132,
  0x00d2959b, 0x8e2d8dc9, 0x00a2d29b, 0x005642b3,
  0xa4507637, 0x005f0f3b, 0x00dac5b3, 0xceb6061b,
  0x00cf063b, 0x01a5f5b3, 0x0155c5b3, 0x01460a3b,
  0x00ba0a3b, 0x01ad161b, 0x006d559b, 0x015d151b,
  0x561b8dd1, 0x8e4900bd, 0x551b8db1, 0x161b019d,
  0x8e49007d, 0x05bb8db1, 0x161b00ba, 0x5a1b01e9,
  0x151b0029, 0x6a330139, 0x561b00ca, 0x8e4900d9,
  0x00ca4a33, 0x0169551b, 0x00a9161b, 0x4a338e49,
  0xe63300ca, 0xf533012d, 0x7633012d, 0x8e490196,
  0x00ca0a3b, 0x0077d51b, 0x0197961b, 0x00ba0a3b,
  0x00b989bb, 0x959b8d51, 0xd61b00e7, 0x8e4d0127,
  0xd59b8e29, 0x8e2d0037, 0xbef9a5b7, 0x3f75859b,
  0x9f319f2d, 0x01770bbb, 0x011fd61b, 0x00ff971b,
  0x013fd59b, 0x961b8f51, 0x8e4d00df, 0xdf9b8f31,
  0x4fb300af, 0xc73301f7, 0x8fbb01a6, 0x773301fb,
  0x8f350137, 0x015f8abb, 0x00ea8abb, 0x01a9961b,
  0x0069d71b, 0x0159959b, 0xd61b8f51, 0x8e4d00b9,
  0xd59b8f31, 0x961b0199, 0x8e4d0079, 0x873b8f31,
  0x161b00ea, 0x5a9b01ea, 0x159b002a, 0xeab3013a,
  0x561b00ca, 0x8e4d00da, 0x00cacab3, 0x016a559b,
  0x00aa161b, 0xcab38e4d, 0x663300ca, 0x75b30149,
  0x76330149, 0x8e4d01b6, 0x00ca8abb, 0x00ea8abb,
  0x00ec8cbb, 0x019b161b, 0x007b571b, 0x559b8f51,
  0x161b012b, 0x8e4d00eb, 0x5b1b8f31, 0x4b33003b,
  0x87370167, 0x071bc671, 0x9fb98f27, 0x016787bb,
  0x171b9cbd, 0x579b00ff, 0x561b011f, 0x8f5d013f,
  0x00df179b, 0x8f3d8fd1, 0x00af5f1b, 0x01e74f33,
  0x013d4733, 0x01e48f3b, 0x01977733, 0x01a74733,
  0x00df06bb, 0xd79b9eb9, 0x971b006c, 0x961b01ac,
  0x8fd9015c, 0x00bcd71b, 0x8fb98f51, 0x019cd61b,
  0x007c971b, 0x8fb98f51, 0x971b9ebd, 0xd79b01ea,
  0x961b002a, 0x8fd9013a, 0x00dad71b, 0x8fb98f51,
  0x016ad61b, 0x00aa971b, 0x8fb98f51, 0x015a6733,
  0x015a7633, 0x01277733, 0x9fb98f51, 0x9fb96722,
  0x9fb56702, 0x67e2c71c, 0x01578abb, 0x262377c2,
  0x8a3b0157, 0x77e20147, 0x01472823, 0x0127893b,
  0x2a2367c2, 0x8dbb0127, 0x778201b7, 0x00dd86bb,
  0x8cbbcf14, 0x77a20197, 0x01972e23, 0x013789bb,
  0x20236786, 0x8d3b0337, 0x222301a7, 0x742a03a7,
  0x696a748a, 0x6a2a69ca, 0x7b666a8a, 0x7c267bc6,
  0x6d667c86, 0x614d6dc6, 0xe7b78082, 0x87936a09,
  0xc51c6677, 0xbb67b7b7, 0xe8578793, 0xf7b7c55c,
  0x87933c6e, 0xc91c3727, 0xa54ff7b7, 0x53a78793,
  0x57b7c95c, 0x8793510e, 0xcd1c27f7, 0x9b0577b7,
  0x88c78793, 0xe7b7cd5c, 0x87931f83, 0xd11c9ab7,
  0x5be0d7b7, 0xd1978793, 0x00052023, 0x00052223,
  0x8082d15c, 0x7139ce79, 0xf426f822, 0xe852f04a,
  0xec4efc06, 0xe05ae456, 0x84aa411c, 0x073b892e,
  0xc11800f6, 0xfa138432, 0x756303f7, 0x415c00c7,
  0xc15c2785, 0x020a0f63, 0x04000993, 0x414989bb,
  0x0009879b, 0x02f46763, 0x8a931982, 0xd9930284,
  0x85ca0209, 0x8533864e, 0x50ef014a, 0x043b4b00,
  0x85d60144, 0xc0ef8526, 0x041bf7cf, 0x994efc04,
  0x89ca4a01, 0x00890b3b, 0x03f00a93, 0x85cea039,
  0xc0ef8526, 0x8993f60f, 0x07bb0409, 0xe8e3413b,
  0x579bfefa, 0x06130064, 0x063bfc00, 0x559b02f6,
  0x059a0064, 0x9e2195ca, 0x0006079b, 0x7442c38d,
  0x02848513, 0x74a270e2, 0x69e27902, 0x6b026aa2,
  0x6a429552, 0x92011602, 0x506f6121, 0x70e24400,
  0x74a27442, 0x69e27902, 0x6aa26a42, 0x61216b02,
  0x80828082, 0xf0227179, 0xf406ec26, 0x41544110,
  0x579b84ae, 0x969b01d6, 0x8fd50036, 0x0186d59b,
  0x0106d69b, 0x00d104a3, 0x0087d693, 0x0ff6f693,
  0x171b07a2, 0x8fd50036, 0x00f11523, 0x0187579b,
  0x00f10623, 0x0107579b, 0x06a38321, 0x771300f1,
  0x17930ff7, 0x8f5d00b6, 0x00b10423, 0x00e11723,
  0x03f67613, 0x03700793, 0xe763842a, 0x079312c7,
  0x863b0380, 0x852240c7, 0x00025597, 0x19858593,
  0xea5ff0ef, 0x8522002c, 0xf0ef4621, 0x4783e9bf,
  0x802300b4, 0x578300f4, 0x80a300a4, 0x441c00f4,
  0x0087d79b, 0x00f48123, 0x81a3441c, 0x478300f4,
  0x822300f4, 0x578300f4, 0x82a300e4, 0x445c00f4,
  0x0087d79b, 0x00f48323, 0x83a3445c, 0x478300f4,
  0x84230134, 0x578300f4, 0x84a30124, 0x481c00f4,
  0x0087d79b, 0x00f48523, 0x85a3481c, 0x478300f4,
  0xb303679c, 0x04e30407, 0x8522fe03, 0x60a26402,
  0x83020141, 0xe0221141, 0x7d1ce406, 0xef89842a,
  0x4501643c, 0xb303679c, 0x0d630287, 0x85220003,
  0x60a26402, 0x83020141, 0x679c67bc, 0xd3ed67bc,
  0xdd799782, 0x640260a2, 0x80820141, 0x679c653c,
  0x0307b303, 0x00030363, 0x45018302, 0x711d8082,
  0x102cf42e, 0xf832ec06, 0xe0bafc36, 0xe8c2e4be,
  0xe42eecc6, 0x261240ef, 0x612560e2, 0x11418082,
  0xe406e022, 0x842a611c, 0xc7914fbc, 0x70ef6128,
  0x30239d6f, 0x643c0404, 0x53fc679c, 0x6828c791,
  0x9c4f70ef, 0x04043823, 0xcf897c1c, 0x53386398,
  0x67bce709, 0x57fc679c, 0x6c28c791, 0x9a8f70ef,
  0x04043c23, 0x09042783, 0x177d777d, 0x60a28ff9,
  0x08f42823, 0x01416402, 0x71798082, 0xf406f022,
  0xe84aec26, 0xe052e44e, 0xc9795429, 0x09052783,
  0x440184aa, 0xc7e98b85, 0x00053983, 0xf0ef892e,
  0x842af4ff, 0x864aed55, 0x85264581, 0x0c6000ef,
  0xed0d842a, 0x0289b703, 0x00197a13, 0xa783cb29,
  0x77b30709, 0xf79300f9, 0xe7b36007, 0xc3a10147,
  0x97028526, 0x6490cd0d, 0x00029597, 0x43058593,
  0x0002c517, 0xb7850513, 0xf17ff0ef, 0xf0ef8526,
  0x842aec7f, 0x6490c535, 0x00029597, 0x41058593,
  0x0002c517, 0xbc850513, 0xef7ff0ef, 0x7c9ca891,
  0x639cc39d, 0xc3856bbc, 0x97828526, 0xcd01842a,
  0x95976490, 0x85930002, 0xc5173e65, 0x05130002,
  0xf0efb6e5, 0xa583ecdf, 0x79330709, 0x791300b9,
  0x69336009, 0x0d630149, 0x85260009, 0xed3ff0ef,
  0xac2357fd, 0xa78308f4, 0x9bf90904, 0x08f4a823,
  0x852270a2, 0x64e27402, 0x69a26942, 0x61456a02,
  0x71798082, 0xe84af022, 0xf406e44e, 0x7938ec26,
  0x892e87aa, 0x89b26304, 0xf8070513, 0xf8048493,
  0x07078413, 0x08050793, 0x00879463, 0xa8394501,
  0x00090a63, 0x8763611c, 0x60dc0127, 0x84938526,
  0xb7cdf807, 0xf0ef85ce, 0xd965ec5f, 0x740270a2,
  0x694264e2, 0x614569a2, 0x11018082, 0xec06e822,
  0x7508842a, 0x860a468d, 0x0002f597, 0x73858593,
  0x488000ef, 0x8522e911, 0x800ff0ef, 0xc11c4782,
  0xc51c4792, 0xc15c47a2, 0xf0ef8522, 0x60e2eb2f,
  0x61056442, 0x715d8082, 0xf84ae0a2, 0xe486f44e,
  0xf052fc26, 0xe85aec56, 0x653ce45e, 0x458189ae,
  0x842a679c, 0x63848932, 0x867ff0ef, 0x09042783,
  0x0693862a, 0x8b850200, 0x0693c399, 0x601c02b0,
  0x451785a6, 0x05130003, 0x63980265, 0x4a1784ce,
  0x0a130003, 0x40ef05aa, 0x4a977e02, 0x8a930003,
  0x4b17046a, 0x0b130003, 0x4b97036b, 0x8b930003,
  0xd063026b, 0x640c0404, 0x0002d517, 0xe6850513,
  0x0019191b, 0x7b2240ef, 0x29857824, 0x07040413,
  0xf8048493, 0x08048793, 0x02f41c63, 0x640660a6,
  0x85a64601, 0xb0ef8522, 0x86aa54f1, 0x85d2864e,
  0xa0ef855a, 0x8b9b4852, 0x86520019, 0x852285a6,
  0xa95ff0ef, 0x85a689de, 0x8522864a, 0x7601b0ef,
  0xbb7d84aa, 0x854e75a2, 0x45f2a0ef, 0x85d2bd69,
  0xa0ef854e, 0xb5e14552, 0x651785ee, 0x05130003,
  0x83635765, 0x855a000a, 0x43f2a0ef, 0x854e85d2,
  0x4372a0ef, 0xb7192a85, 0x856685ee, 0x000a8363,
  0xa0ef855a, 0x85d24252, 0xa0ef854e, 0x2a8541d2,
  0x9863bf25, 0x86560147, 0xe42e8522, 0x99fff0ef,
  0x865e65a2, 0xb0ef8522, 0x85aa6f61, 0x4158b799,
  0x00ff0637, 0x0187569b, 0x0187179b, 0x169b8fd5,
  0x8ef10087, 0x66c18fd5, 0xf0068693, 0x0087571b,
  0x8fd98f75, 0x93811782, 0x8082953e, 0xec4e7139,
  0x89aae852, 0x85328a2e, 0x00034597, 0xac058593,
  0xf04af426, 0xfc06e456, 0x8ab2f822, 0x84ba8936,
  0x00f290ef, 0x66c1e539, 0x85ce8652, 0x80ef842a,
  0x571b0132, 0x179b0185, 0x8fd90185, 0x00ff06b7,
  0x0085171b, 0x8fd98f75, 0x551b6741, 0x07130085,
  0x8d79f007, 0x20238d5d, 0x479100a9, 0x70e2c09c,
  0x74428522, 0x790274a2, 0x6a4269e2, 0x61216aa2,
  0xe5978082, 0x85930003, 0x855657e5, 0x7b2290ef,
  0xe909842a, 0x864a66c1, 0x854e85d2, 0x37d200ef,
  0xb7e947d1, 0x0003e597, 0x56458593, 0x90ef8556,
  0x842a7902, 0x66c1e911, 0x85d2864a, 0x40ef854e,
  0x07936302, 0xb75d0200, 0x00030597, 0x5b058593,
  0x90ef8556, 0x842a76c2, 0x66c1e909, 0x85d2864a,
  0xf0ef854e, 0x47c112c1, 0x547db751, 0x7131b749,
  0xf526f922, 0xed4ef14a, 0xe556e952, 0xfcdee15a,
  0xf4e6f8e2, 0xeceef0ea, 0xfd068936, 0x89ae84aa,
  0xb0ef8a32, 0x842a64e1, 0x00036b17, 0x168b0b13,
  0x02010b93, 0x00033c17, 0xb64c0c13, 0x03010a93,
  0x01c10c93, 0x02810d13, 0x01810d93, 0x5e632901,
  0x57e10004, 0x00f40663, 0x450557d5, 0x0ef41563,
  0x00036917, 0x1b090913, 0x4601a855, 0x852685a2,
  0x3351b0ef, 0x855ae42a, 0x740290ef, 0x862a67a2,
  0x853e85da, 0x6ea290ef, 0x865ee921, 0x852685a2,
  0xe5aff0ef, 0x0e051763, 0x85627582, 0x24b2a0ef,
  0x661786d6, 0x06130003, 0x85a22166, 0xb0ef8526,
  0xc90d75b1, 0x47915742, 0x02f71663, 0xc39d411c,
  0x00036517, 0x20850513, 0x21f2a0ef, 0x00036517,
  0x20c50513, 0xdf3fc0ef, 0x852685a2, 0x5b41b0ef,
  0xb7b5842a, 0x866a86e6, 0x852685a2, 0xe26ff0ef,
  0x7602e541, 0x86d6876e, 0x855285ca, 0xe21ff0ef,
  0x47e2e53d, 0x1f634672, 0x75a204f6, 0x90ef8556,
  0xdd4d0172, 0x00036917, 0x15c90913, 0x460185a2,
  0xb0ef8526, 0x842a2831, 0x85ce4601, 0xb0ef8526,
  0x86aa2771, 0x85ca8622, 0x00036517, 0x1a850513,
  0x1a72a0ef, 0x70ea4501, 0x74aa744a, 0x69ea790a,
  0x6aaa6a4a, 0x7be66b0a, 0x7ca67c46, 0x6de67d06,
  0x80826129, 0x00036917, 0x11c90913, 0x6917bf45,
  0x09130003, 0xb75d12a9, 0x00036917, 0x0d890913,
  0x6917bf71, 0x09130003, 0xbf490ae9, 0x00347179,
  0xf022860a, 0xf406ec26, 0x842ae84a, 0xf0ef84ae,
  0xcd1dcbef, 0x45814601, 0xb0ef8522, 0x892a1fb1,
  0x85a64601, 0xb0ef8522, 0x86aa1ef1, 0x6597864a,
  0x85930003, 0x6517ffa5, 0x05130003, 0xa0ef0125,
  0x45011192, 0x740270a2, 0x694264e2, 0x80826145,
  0x660266a2, 0x852285a6, 0xe17ff0ef, 0x715db7e5,
  0x00037597, 0xe5858593, 0xe486fc26, 0xf84ae0a2,
  0xf052f44e, 0xe85aec56, 0xc0ef84aa, 0x5a6301c1,
  0xc0ef0205, 0x862a3421, 0x00037597, 0xe3058593,
  0x00036517, 0x11050513, 0x0bf2a0ef, 0x60a64501,
  0x74e26406, 0x79a27942, 0x6ae27a02, 0x61616b42,
  0x842a8082, 0x651785a6, 0x05130003, 0xa0efa325
};

#ifdef INIT_PATTERN_ZERO
const uint32_t ddr_init_pattern[64U] =
{
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};
#else
const uint32_t ddr_init_pattern[64U] =
{
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF,
    0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF
};
#endif
