#ifndef DEFINES_H_HEADER_GUARD
#define DEFINES_H_HEADER_GUARD

#define LOCO_EXTENTION_LENGTH			16
#define LOCO_PATH_LENGTH					256

#define LOCO_MURMUR_HASH_SEED			12345678

#ifndef LOCO_USE_HOT_RELOAD
	#define LOCO_USE_HOT_RELOAD
#endif // LOCO_USE_HOT_RELOAD

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))


#endif // DEFINES_H_HEADER_GUARD

