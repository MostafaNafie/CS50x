#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	//ensure proper usage
	if (argc != 2)
	{
		printf("usage ./recover image\n");
		return 1;
	}
	// get filename
	char *file = argv[1];

	// open file
	FILE *inptr = fopen(file, "r");
	if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s\n", file);
        return 2;
    }

    // declare a buffer (an array of characters) to store 512 bytes in a time
    unsigned char buffer[512];
    // filename of the output image of size 8 "###.jpg\0"
    char filename[8];
    // a pointer to the output image
	FILE *img = NULL;

    // whether I am in a JPEG or not
    _Bool inJPEG = false;
    // number of JPEGs found
    int countJPEG = 0;

    // read one byte at a time for 512 times, if there are not 512 bytes then I reached the EOF
    while (fread(buffer, 1, 512, inptr) == 512)
    {
    	// the start of a JPEG
	    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
	    {
	    	// I found one before so close it first
	    	if (inJPEG)
	    	{
	    		fclose(img);
	    	}

    		inJPEG = true;
    		// format the name of the JPEG
	    	sprintf(filename, "%03i.jpg", countJPEG);
    		// add a new JPEG to the counter
    		countJPEG++;
	    	// create the new image
	    	img = fopen(filename, "w");
	    	// write to it the start of the JPEG
	    	fwrite(buffer, 512, 1, img);
	    	
	    } else // not a start of a JPEG
	    {
	    	// if I am already in a JPEG then write to it
	    	if (inJPEG)
	    	{
	    		fwrite(buffer, 512, 1, img);
	    	}
	    }

    }

    // close the input file
    fclose(inptr);
    // close the last opened image file
    fclose(img);

    // success
	return 0;
}
