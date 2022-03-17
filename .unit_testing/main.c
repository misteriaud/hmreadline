#include "../includes/libhmreadline.h"

char *get_cwd()
{
	return ("Coucou");
}

int main(void)
{
	printf("%s\n", xreadline(&get_cwd, 0));
	return 0;
}
