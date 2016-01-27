#include <plan9.h>

ssize_t
readn(int f, void *av, ssize_t n)
{
	char *a;
	ssize_t m, t;

	a = av;
	t = 0;
	while(t < n){
		m = read(f, a+t, n-t);
		if(m <= 0){
			if(t == 0)
				return m;
			break;
		}
		t += m;
	}
	return t;
}
