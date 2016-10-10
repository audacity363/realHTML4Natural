#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    char buffer_one[29],
         buffer_two[29];

    int length = sizeof(buffer_one),
        i,
        val_length = 2;
    

    //buffer_one[29] = buffer_two[29] = '\0';


    memset(buffer_two, 0x30, sizeof(buffer_two));
    memset(buffer_one, 0x00, sizeof(buffer_one));

    buffer_one[0] = 0x31;
    buffer_one[1] = 0x38;

    i=0;
    while((buffer_one[i] != '0' && buffer_one[i] != 0x00) && i < length) i++;

    memcpy(buffer_two+((length-val_length)), buffer_one, val_length);

    printf("%d\n", length-val_length);

    printf("Offset: [%.29s]\n", buffer_two);

     

}
