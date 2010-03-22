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
// imgage.cpp - image container

#include "../library.h"





/*-----------------------------------------------------------------------------
	EImage :
-----------------------------------------------------------------------------*/

EImage::EImage(void)
{
	CreateImage(1,1);
}

EImage::EImage(uint w, uint h)
{
	CreateImage(w, h);
}


EImage::~EImage(void)
{
}

/*-----------------------------------------------------------------------------
	Texture Loading And Saving Stuff :
-----------------------------------------------------------------------------*/

void EImage::CreateImage(uint w, uint h,const EVec4 &fill_color)
{
	if (w<1 || h<1) {
		RAISE_EXCEPTION("Image size should be greater or equal 1x1");

	}
	width	=	w;
	height	=	h;
	image_data.resize(w*h, fill_color);
}


#if 0
//
//	EImage::LoadTGA
//
void EImage::LoadTGA(const char *path)
{
	EFile	file;
	
	if (!file.Open(path, FS_READ)) {
		RAISE_EXCEPTION(va("File \"%s\" not found.\r\n", path));
	}
	
	tga_header_t	header;
	
	file.Read(&header.IndFiled,				1);
	file.Read(&header.ColorMapType,			1);
	file.Read(&header.ImageTypeCode,		1);

	file.Read(&header.ColorMapOrigin,		2);
	file.Read(&header.ColorMapLength,		2);
	file.Read(&header.ColorMapEntrySize,	1);

	file.Read(&header.XOrigin,				2);
	file.Read(&header.YOrigin,				2);
	file.Read(&header.Width,				2);
	file.Read(&header.Height,				2);
	file.Read(&header.ImagePixelSize,		1);
	file.Read(&header.ImageDescriptorByte,	1);
	
	if (header.ImageTypeCode!=2) {
		file.Close();
		RAISE_EXCEPTION("failed (typecode!=2)\r\n");
		return;
	}
	if (header.ImagePixelSize<24) {
		file.Close();
		RAISE_EXCEPTION("failed (pixelsize<24)\r\n");
		return;
	}
	
	file.Seek(18 + header.IndFiled, FS_BEGIN);
	
	vector<byte> temp_buffer;
	uint size	= header.Width * header.Height * header.ImagePixelSize / 8;
	uint width	= header.Width;
	uint height	= header.Height;
	uint point_size = header.ImagePixelSize / 8;

	temp_buffer.resize(size);
	
	file.Read(&temp_buffer[0], size);
	
	file.Close();
	
	CreateImage(header.Width, header.Height);		

	EVec4 clr;
	for (int x=0; x<header.Width; x++) {
		for (int y=0; y<header.Height; y++) {
			if (point_size==3) {
				clr.x = temp_buffer[ ((header.Height - y - 1) * header.Width + x) * point_size + 2] / 255.0f;
				clr.y = temp_buffer[ ((header.Height - y - 1) * header.Width + x) * point_size + 1] / 255.0f;
				clr.z = temp_buffer[ ((header.Height - y - 1) * header.Width + x) * point_size + 0] / 255.0f;
				clr.w = 1.0f;
			} else 
			if (point_size==4) {
				clr.z = temp_buffer[ ((header.Height - y - 1) * header.Width + x) * point_size + 0] / 255.0f;
				clr.y = temp_buffer[ ((header.Height - y - 1) * header.Width + x) * point_size + 1] / 255.0f;
				clr.x = temp_buffer[ ((header.Height - y - 1) * header.Width + x) * point_size + 2] / 255.0f;
				clr.w = temp_buffer[ ((header.Height - y - 1) * header.Width + x) * point_size + 3] / 255.0f;
			}
			WriteC(x, y, clr);
		}
	}
}


//
//	EImage::SaveTGA
//
void EImage::SaveTGA(const char *path)
{
	EFile file;
	
	if (!file.Open(path, FS_WRITE)) {
		RAISE_EXCEPTION(va("Cann`t create file \"%s\".\r\n", path));
	}
	
	//
	//	Write TGA header :
	//
	tga_header_t	header;

	memset(&header, 0, sizeof(tga_header_t));
	header.IndFiled				= 0;
	header.ColorMapType			= 0;
	header.ImageTypeCode		= 2;	// RGB/RGBA format
	header.XOrigin				= 0;
	header.YOrigin				= 0;
	header.Width				= (unsigned short)GetWidth();
	header.Height				= (unsigned short)GetHeight();
	header.ImagePixelSize		= 32;
	header.ImageDescriptorByte	= 0x08;	//  Photoshop writes this value to 0x08

	file.Write(&header.IndFiled,			1);
	file.Write(&header.ColorMapType,		1);
	file.Write(&header.ImageTypeCode,		1);

	file.Write(&header.ColorMapOrigin,		2);
	file.Write(&header.ColorMapLength,		2);
	file.Write(&header.ColorMapEntrySize,	1);

	file.Write(&header.XOrigin,				2);
	file.Write(&header.YOrigin,				2);
	file.Write(&header.Width,				2);
	file.Write(&header.Height,				2);
	file.Write(&header.ImagePixelSize,		1);
	file.Write(&header.ImageDescriptorByte,	1);

	//
	//	Prepare image buffer :
	//
	vector<byte> buffer;
	buffer.resize(header.Width * header.Height * 4);
	uint ptr = 0;

	for (int y=(int)GetHeight()-1; y>=0; y--) {
		for (int x=0; x<(int)GetWidth(); x++) {
			buffer[ptr+0] = (byte)Clamp<float>(255.0f * ReadC(x,y).z, 0, 255.0);
			buffer[ptr+1] = (byte)Clamp<float>(255.0f * ReadC(x,y).y, 0, 255.0);
			buffer[ptr+2] = (byte)Clamp<float>(255.0f * ReadC(x,y).x, 0, 255.0);
			buffer[ptr+3] = (byte)Clamp<float>(255.0f * ReadC(x,y).w, 0, 255.0);
			ptr+=4;
		}
	}
	
	//
	//	Write image data :
	//
	file.Seek(18, FS_BEGIN);
	file.Write(&buffer[0], header.Width * header.Height * 4);
	
	char end[] = "TRUEVISION-XFILE.";
	file.Write(end, (int)strlen(end)+1);
	
	file.Close();
}
#endif 


/*-----------------------------------------------------------------------------
	Image Data Access :
-----------------------------------------------------------------------------*/

uint EImage::OffsetC(int x, int y) const
{
	x = Clamp<int>(x, 0, width-1);
	y = Clamp<int>(y, 0, height-1);
	uint offset = y * width + x;
	
	ASSERT(offset < image_data.size());
	
	return offset;
}


uint EImage::OffsetW(int x, int y) const
{
	if (x>0) x = x % (int)width;
	if (y>0) y = y % (int)height;
	
	if (x<0) x = width + x;
	if (y<0) y = height + y;
	
	uint offset = y * width + x;
	
	ASSERT(offset < image_data.size());
	
	return offset;
}


EVec4 EImage::ReadC(int x, int y) const
{
	return image_data[ OffsetC(x,y) ];
}


EVec4 EImage::ReadW(int x, int y) const
{
	return image_data[ OffsetW(x,y) ];
}


void EImage::WriteC(int x, int y, const EVec4 &clr)
{
	if ( x>=(int)width || y>=(int)height || x<0 || y<0 ) return;
	image_data[ OffsetC(x,y) ] = clr;
}


void EImage::WriteW(int x, int y, const EVec4 &clr)
{
	image_data[ OffsetW(x,y) ] = clr;
}


EVec4 EImage::ReadLinearC(float x, float y) const
{
	// shift coordinates :
	x -= 0.5f;
	y -= 0.5f;

	float dx = x - floorf(x);	
	float dy = y - floorf(y);	
	
	int ix = (int)floorf(x);
	int iy = (int)floorf(y);
	
	EVec4 s00 = ReadC(ix+0, iy+0);
	EVec4 s01 = ReadC(ix+0, iy+1);
	EVec4 s11 = ReadC(ix+1, iy+1);
	EVec4 s10 = ReadC(ix+1, iy+0);

	EVec4 s0x = Vec4Lerp(s00, s01, dy);	
	EVec4 s1x = Vec4Lerp(s10, s11, dy);	
	
	EVec4 s = Vec4Lerp(s0x, s1x, dx);
	return s;
}









