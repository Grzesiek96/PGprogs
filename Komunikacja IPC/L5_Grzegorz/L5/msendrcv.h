/* msendrcv.h */
#define QUEKEY	0x00FF00
#define PACKET 80

typedef struct {
	long mtype;
	char mtext[PACKET];
} packet;
