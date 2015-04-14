#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define APP_VERSION	"1.1.0"
#define APP_RELDATE	"December 1, 2003"
#define APP_NAME	"ifenslave"

static char *version =
APP_NAME ".c:v" APP_VERSION " (" APP_RELDATE ")\n"
"o Donald Becker (becker@cesdis.gsfc.nasa.gov).\n"
"o Detach support added on 2000/10/02 by Willy Tarreau (willy at meta-x.org).\n"
"o 2.4 kernel support added on 2001/02/16 by Chad N. Tindel\n"
"  (ctindel at ieee dot org).\n";


int main(void)
{
	printf("%s", version);

	return 0;
}
