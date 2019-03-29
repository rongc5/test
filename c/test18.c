#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char buf[512] = "<div id=\"ad\">\n"
                    "<a href=\"%s\"> <img src=\"%s\" height=\"100%\" width=\"100%\" /> </a>\n"
                    "</div>\n";

    snprintf(buf, sizeof(buf), "<div id=\"ad\">\n"
                                "<a href=\"%s\"> <img src=\"%s\" height=\"100%\" width=\"100%\" /> </a>\n"
                                "</div>\n", "123", "456");

    printf("%s\n", buf);

    return 0;
}
