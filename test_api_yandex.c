#include <stdio.h>
#include <stdlib.h>
#include "src/api.h"


int main() {
    char *rresult = malloc(400);
    rresult[0] = '\0';

    download_link(121099045, 29022270, &rresult);
    printf("%s", rresult);
    free(rresult);



    return 0;
}
