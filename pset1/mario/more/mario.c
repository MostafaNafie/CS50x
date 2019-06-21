#include <cs50.h>
#include <stdio.h>


int main(void)
{
    int h;
    // get the number of rows from the user
    // used a do-while loop to ensure that the number of rows is between 1 and 8 only
    do 
    {     
        h = get_int("Height: ");
    } while ( h < 1 || h > 8);
    // print the rows
    for(int r = 1; r <= h; r++) {
        // print the cloumns
        for (int c = 0; c < 2*r; c++)
        {
            // print spaces at the begining of each row
            if (c == 0)
            {
                for (int s = 0; s < h - r; s++)
                {
                printf(" ");
                }
            }
            
            // print the blocks of the pyramids
            printf("#");
            
            // print the space in the middle between the 2 pyramids
            if (c == r-1)
            {
                printf("  ");
            }
        } 
        // end the row
        printf("\n");
    }
}
