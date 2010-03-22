/*
	The MIT License

	Copyright (c) 2010 IFMO/GameDev Studio

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

#include "../library.h"


struct	EImageTGAHeader_s	{
		char				IndFiled;
		char				ColorMapType;
		char				ImageTypeCode;
		// Color map specification 
		short unsigned int	ColorMapOrigin;
		short unsigned int	ColorMapLength;
		char				ColorMapEntrySize;
		// Image specification 
		short unsigned int	XOrigin;
		short unsigned int	YOrigin;
		short unsigned int	Width;
		short unsigned int	Height;
		char				ImagePixelSize;
		char				ImageDescriptorByte;
	};


/*-----------------------------------------------------------------------------
	TGA Import Function
	this function reads only 24- and 32-bit uncompressed TGA images
-----------------------------------------------------------------------------*/
//
//	ImageImportTGA
//
void ImageImportTGA(EImage &image, vector<byte> &input_data)
{
	EImageTGAHeader_s	header;
	if (input_data.size()<18) {
		RAISE_EXCEPTION("TGA file is too small");
	}
	
	//	Move pointer to TGA header :
	byte *scan = &input_data[0];
	
	memcpy(&header.IndFiled,		scan,	1); scan+=1;
	memcpy(&header.ColorMapType,	scan,	1);	scan+=1;
	memcpy(&header.ImageTypeCode,	scan,	1);	scan+=1;

	memcpy(&header.ColorMapOrigin,		scan,	2);	scan+=2;
	memcpy(&header.ColorMapLength,		scan,	2);	scan+=2;
	memcpy(&header.ColorMapEntrySize,	scan,	1);	scan+=1;

	memcpy(&header.XOrigin,				scan,	2); scan+=2;
	memcpy(&header.YOrigin,				scan,	2); scan+=2;
	memcpy(&header.Width,				scan,	2); scan+=2;
	memcpy(&header.Height,				scan,	2); scan+=2;
	memcpy(&header.ImagePixelSize,		scan,	1); scan+=1;
	memcpy(&header.ImageDescriptorByte,	scan,	1); scan+=1;
	
	if (header.ImageTypeCode!=2) {
		RAISE_EXCEPTION("failed (typecode!=2)\r\n");
		return;
	}
	if (header.ImagePixelSize<24) {
		RAISE_EXCEPTION("failed (pixelsize<24)\r\n");
		return;
	}
	
	// Move pointer to begin of image data :
	scan		= &input_data[18 + header.IndFiled];
	
	uint pixel_size = header.ImagePixelSize / 8;

	image.CreateImage(header.Width, header.Height);		

	EVec4 clr;
	for (int x=0; x<header.Width; x++) {
		for (int y=0; y<header.Height; y++) {
			if (pixel_size==3) {
				clr.x = scan[ ((header.Height - y - 1) * header.Width + x) * pixel_size + 2] / 255.0f;
				clr.y = scan[ ((header.Height - y - 1) * header.Width + x) * pixel_size + 1] / 255.0f;
				clr.z = scan[ ((header.Height - y - 1) * header.Width + x) * pixel_size + 0] / 255.0f;
				clr.w = 1.0f;
			} else 
			if (pixel_size==4) {
				clr.z = scan[ ((header.Height - y - 1) * header.Width + x) * pixel_size + 0] / 255.0f;
				clr.y = scan[ ((header.Height - y - 1) * header.Width + x) * pixel_size + 1] / 255.0f;
				clr.x = scan[ ((header.Height - y - 1) * header.Width + x) * pixel_size + 2] / 255.0f;
				clr.w = scan[ ((header.Height - y - 1) * header.Width + x) * pixel_size + 3] / 255.0f;
			}
			image.WriteC(x, y, clr);
		}
	}
}


/*-----------------------------------------------------------------------------
	TGA Export Function :
	this function stores only 32-bit uncompressed TGA images
-----------------------------------------------------------------------------*/
//
//	ImageExportTGA
//
void ImageExportTGA(EImage &image, vector<byte> &output_data)
{
	char footer[] = "TRUEVISION-XFILE.";
	EImageTGAHeader_s	header;
	
	output_data.resize(18 + 4 * image.Width() * image.Height() + strlen(footer), 0);
	byte *scan = &output_data[0];

	memset(&header, 0, sizeof(EImageTGAHeader_s));
	header.IndFiled				= 0;
	header.ColorMapType			= 0;
	header.ImageTypeCode		= 2;	// RGB/RGBA format
	header.XOrigin				= 0;
	header.YOrigin				= 0;
	header.Width				= (unsigned short)image.Width();
	header.Height				= (unsigned short)image.Height();
	header.ImagePixelSize		= 32;
	header.ImageDescriptorByte	= 0x08;	//  Photoshop writes this value to 0x08

	memcpy(scan, &header.IndFiled,			1);	scan+=1;
	memcpy(scan, &header.ColorMapType,		1);	scan+=1;
	memcpy(scan, &header.ImageTypeCode,		1);	scan+=1;

	memcpy(scan, &header.ColorMapOrigin,	2);	scan+=2;
	memcpy(scan, &header.ColorMapLength,	2);	scan+=2;
	memcpy(scan, &header.ColorMapEntrySize,	1);	scan+=1;

	memcpy(scan, &header.XOrigin,				2);	scan+=2;
	memcpy(scan, &header.YOrigin,				2);	scan+=2;
	memcpy(scan, &header.Width,					2);	scan+=2;
	memcpy(scan, &header.Height,				2);	scan+=2;
	memcpy(scan, &header.ImagePixelSize,		1);	scan+=1;
	memcpy(scan, &header.ImageDescriptorByte,	1);	scan+=1;

	//
	//	Prepare image buffer :
	//
	scan = &output_data[18];

	uint ptr = 0;

	for (int y=(int)image.Height()-1; y>=0; y--) {
		for (int x=0; x<(int)image.Width(); x++) {
			EVec4 color = image.ReadC(x,y);
			scan[ptr+0] = (byte)Clamp<float>(255.0f * color.z, 0, 255.0);
			scan[ptr+1] = (byte)Clamp<float>(255.0f * color.y, 0, 255.0);
			scan[ptr+2] = (byte)Clamp<float>(255.0f * color.x, 0, 255.0);
			scan[ptr+3] = (byte)Clamp<float>(255.0f * color.w, 0, 255.0);
			ptr+=4;
		}
	}

	for (uint i=0; i<strlen(footer); i++) {
		scan[ptr] = footer[i];
		ptr++;
	}	
}


