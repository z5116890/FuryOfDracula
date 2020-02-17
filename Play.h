// Play Typedef header
// It's easier to use the Play if we abstract it slightly.

#define LEN_PLAY 7

typedef char * Play;

Play newPlay(char* p);
Play copyPlay(Play p);
int checkSanity(Play p);
void destroyPlay(Play p);
