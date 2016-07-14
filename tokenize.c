#include <plan9.h>

int
getfields(char *str, char **args, int max, int mflag, char *set)
{
	int r;
	int intok, narg;

	if(max <= 0)
		return 0;

	narg = 0;
	args[narg] = str;
	if(!mflag)
		narg++;
	intok = 0;
	for(;; str++) {
		r = *str;
		if(r == 0)
			break;
		if(strchr(set, r)) {
			if(narg >= max)
				break;
			*str = 0;
			intok = 0;
			args[narg] = str + 1;
			if(!mflag)
				narg++;
		} else {
			if(!intok && mflag)
				narg++;
			intok = 1;
		}
	}
	return narg;
}

int
tokenize(char *str, char **args, int max)
{
	return getfields(str, args, max, 1, " \t\n\r");
}
