// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    
    // remeber magnification factor
    int n = atoi(argv[1]);

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    
    // determine infile padding for scanlines
    int inPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // save value of infile width and height
    int inFileBiWidth = bi.biWidth;
    int inFileBiHeight = bi.biHeight;
    // update outfile header data
    bi.biWidth *= n;
    bi.biHeight *= n;
    // determine padding for scanlines
    int outPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + outPadding) * abs(bi.biHeight);
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
    // DEBUGGING printf("size = %i, Width = %i, Height = %i\nMag = %i\n", bf.bfSize, bi.biWidth, bi.biHeight, n);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(inFileBiHeight); i < biHeight; i++)
        {
            // using the recopy method
            for (int h = 0; h < n-1; h++)
            {
                // iterate over pixels in scanline
                for (int j = 0; j < inFileBiWidth; j++)
                {
                    // temporary storage
                    RGBTRIPLE triple;
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    
                    // repeat the pixels n times (horizontal resizing)
                    for (int m = 0; m < n; m++)
                    {
                       // write RGB triple to outfile
                       fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr); 
                    }
                }
                // add padding
                for (int k = 0; k < outPadding; k++)
                {
                    fputc(0x00, outptr);
                }
                // send infile cursor back
                int seekNum =  (inFileBiWidth * sizeof(RGBTRIPLE));
                fseek(inptr, -seekNum, SEEK_CUR);
            }  
                // iterate over pixels in scanline
                for (int j = 0; j < inFileBiWidth; j++)
                {
                    // temporary storage
                    RGBTRIPLE triple;
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    
                    // repeat the pixels n times (horizontal resizing)
                    for (int m = 0; m < n; m++)
                    {
                       // write RGB triple to outfile
                       fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr); 
                    }
                }
        
                // add padding
                for (int k = 0; k < outPadding; k++)
                {
                    fputc(0x00, outptr);
                }
        
                // skip over padding, if any
                fseek(inptr, inPadding, SEEK_CUR);
    }
    

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
