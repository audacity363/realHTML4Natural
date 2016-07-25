#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"
#include "hexdump.h"
#include "utils.h"

int main()
{
    struct variables *anker = malloc(sizeof(struct variables));
    generate_page(anker, " ", " ", " ");
}
