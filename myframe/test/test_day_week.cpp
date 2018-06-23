#include "common_util.h"
#include "base_def.h"

int main(int argc, char *argv[])
{

    printf("%d\n", dayofweek(23, 6, 2018));
    printf("%d\n", dayofweek(23, 6, 2017));
    printf("%d\n", dayofweek(23, 6, 2016));

    return 0;
}
