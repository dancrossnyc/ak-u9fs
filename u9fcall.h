#include <fcall.h>

typedef struct fcall Fcall;
typedef struct qid Qid;
typedef struct dir Dir;

void	initfcallconv(void);
void	initdirconv(void);
void	dirmodeconv(char buf[static 12], uint32_t m);
