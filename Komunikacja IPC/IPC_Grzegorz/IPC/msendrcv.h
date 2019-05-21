/* msendrcv.h */
#define QUEKEY	0x00FF00//adres w pamieci
#define PACKET 80

typedef struct {
	long mtype;
	char mtext[PACKET];
} packet;
