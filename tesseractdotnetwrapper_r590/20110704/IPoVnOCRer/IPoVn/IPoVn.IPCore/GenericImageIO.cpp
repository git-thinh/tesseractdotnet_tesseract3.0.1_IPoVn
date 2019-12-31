/**
Copyright 2011, Cong Nguyen

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**/

#include "stdafx.h"
#include "GenericImageIO.h"


BEGIN_NAMSPACE

USING_COMMON_SYSTEM


GenericImageIO::GenericImageIO(void)
{
}

GenericImageIO::~GenericImageIO(void)
{
}

bool GenericImageIO::FromFormat1bppIndexed(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	uchar* src_data = (uchar*)bitmapData->get_Scan0().ToPointer();
	ushort* dst_ptr = buffer;

	try
	{
		uchar* src_ptr = null;
		int bit_offset = 0;
		for (int y=0; y < height; y++)
		{
			src_ptr = src_data + y * stride;
			for (int x=0; x < width; x++, dst_ptr++)
			{
				bit_offset = 7 - x % 8;				
				*dst_ptr = mapColors[(*(src_ptr + x / 8) >> bit_offset) & 0x1];
			}
		}
	}
	catch(System::Exception* exp)
	{
		throw exp;
	}
	__finally
	{
		src_data = null;
		dst_ptr = null;
	}

	return true;
}

bool GenericImageIO::FromFormat4bppIndexed(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	uchar* src_data = (uchar*)bitmapData->get_Scan0().ToPointer();
	ushort* dst_ptr = buffer;

	try
	{
		uchar* src_ptr = null;
		int bit_offset = 0;
		for (int y=0; y < height; y++)
		{			
			src_ptr = src_data + y * stride;
			for (int x=0; x < width; x++, dst_ptr++)
			{
				bit_offset = x % 2;
				if (bit_offset == 0)
				{
					uchar upper = (*(src_ptr + x / 2) >> 4) & 0x0F;
					*dst_ptr = mapColors[upper];
				}
				else
				{
					uchar lower = (*(src_ptr + x / 2) & 0x0F);
					*dst_ptr = mapColors[lower];
				}
			}
		}
	}
	catch(System::Exception* exp)
	{
		throw exp;
	}
	__finally
	{
		src_data = null;
		dst_ptr = null;
	}

	return true;
}

bool GenericImageIO::FromFormat8bppIndexed(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	uchar* src_data = (uchar*)bitmapData->get_Scan0().ToPointer();
	ushort* dst_ptr = buffer;

	try
	{
		uchar* src_ptr = null;
		
		for (int y=0; y < height; y++)
		{			
			src_ptr = src_data + y * stride;
			for (int x=0; x < width; x++, dst_ptr++, src_ptr++)
			{
				*dst_ptr = *src_ptr; //mapColors[*(src_ptr + x)];
			}
		}
	}
	catch(System::Exception* exp)
	{
		throw exp;
	}
	__finally
	{
		src_data = null;
		dst_ptr = null;
	}

	return true;
}

bool GenericImageIO::FromFormat16bppGrayScale(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	uchar* src_data = (uchar*)bitmapData->get_Scan0().ToPointer();
	ushort* dst_ptr = buffer;

	try
	{
		ushort* src_ptr = null;
		
		for (int y=0; y < height; y++)
		{
			src_ptr = (ushort*)(src_data + y * stride);
			for (int x=0; x < width; x++, dst_ptr++, src_ptr++)
			{
				*dst_ptr = (ushort)((*src_ptr + 128.0f) / 256.0f);
			}
		}
	}
	catch(System::Exception* exp)
	{
		throw exp;
	}
	__finally
	{
		src_data = null;
		dst_ptr = null;
	}

	return true;
}

bool GenericImageIO::FromFormat16bppArgb1555(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	uchar* src_data = (uchar*)bitmapData->get_Scan0().ToPointer();
	ushort* dst_ptr = buffer;

	try
	{
		ushort* src_ptr = null;
		
		for (int y=0; y < height; y++)
		{			
			src_ptr = (ushort*)(src_data + y * stride);
			for (int x=0; x < width; x++, dst_ptr++, src_ptr++)
			{
				*dst_ptr = IntensityFromRGB(((*src_ptr) & 37), (((*src_ptr) >> 5) & 37), (((*src_ptr) >> 10) & 37));
			}
		}
	}
	catch(System::Exception* exp)
	{
		throw exp;
	}
	__finally
	{
		src_data = null;
		dst_ptr = null;
	}

	return true;
}

bool GenericImageIO::FromFormat16bppRgb555(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	return FromFormat16bppArgb1555(
		bitmap, bitmapData, mapColors, width, height, stride, buffer);
}

bool GenericImageIO::FromFormat16bppRgb565(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	uchar* src_data = (uchar*)bitmapData->get_Scan0().ToPointer();
	ushort* dst_ptr = buffer;

	try
	{
		ushort* src_ptr = null;
		
		for (int y=0; y < height; y++)
		{		
			src_ptr = (ushort*)(src_data + y * stride);
			for (int x=0; x < width; x++, dst_ptr++, src_ptr++)
			{
				*dst_ptr = IntensityFromRGB(((*src_ptr) & 37), (((*src_ptr) >> 5) & 77), (((*src_ptr) >> 11) & 37));
			}
		}
	}
	catch(System::Exception* exp)
	{
		throw exp;
	}
	__finally
	{
		src_data = null;
		dst_ptr = null;
	}

	return true;
}

bool GenericImageIO::FromFormat24bppRgb(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	uchar* src_data = (uchar*)bitmapData->get_Scan0().ToPointer();
	ushort* dst_ptr = buffer;

	try
	{
		uchar* src_ptr = null;
		
		for (int y=0; y < height; y++)
		{			
			src_ptr = src_data + y * stride;
			for (int x=0; x < width; x++, dst_ptr++, src_ptr += 3)
			{
				// I assumed here that offset for each channel are: r = 2, g = 1, b = 0
				// This should review later, :)
				// Cong.
				*dst_ptr = IntensityFromRGB(*(src_ptr + 2), *(src_ptr + 1), *src_ptr);
			}
		}
	}
	catch(System::Exception* exp)
	{
		throw exp;
	}
	__finally
	{
		src_data = null;
		dst_ptr = null;
	}

	return true;
}

bool GenericImageIO::FromFormat32bppRgb(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	uchar* src_data = (uchar*)bitmapData->get_Scan0().ToPointer();
	ushort* dst_ptr = buffer;

	try
	{
		uchar* src_ptr = null;
		
		for (int y=0; y < height; y++)
		{			
			src_ptr = src_data + y * stride;
			for (int x=0; x < width; x++, dst_ptr++, src_ptr += 4)
			{
				// I assumed here that offset for each channel are: r = 2, g = 1, b = 0, a = 3
				// This should review later, :)
				// Cong.
				*dst_ptr = IntensityFromRGB(*(src_ptr + 2), *(src_ptr + 1), *src_ptr);
			}
		}
	}
	catch(System::Exception* exp)
	{
		throw exp;
	}
	__finally
	{
		src_data = null;
		dst_ptr = null;
	}

	return true;
}

bool GenericImageIO::FromFormat32bppArgb(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	uchar* src_data = (uchar*)bitmapData->get_Scan0().ToPointer();
	ushort* dst_ptr = buffer;

	try
	{
		uchar* src_ptr = null;
		
		for (int y=0; y < height; y++)
		{			
			src_ptr = src_data + y * stride;
			for (int x=0; x < width; x++, dst_ptr++, src_ptr += 4)
			{
				// I assumed here that offset for each channel are: r = 2, g = 1, b = 0, a = 3
				// This should review later, :)
				// Cong.
				*dst_ptr = IntensityFromRGBA(*(src_ptr + 2), *(src_ptr + 1), *src_ptr, *(src_ptr + 3));
			}
		}
	}
	catch(System::Exception* exp)
	{
		throw exp;
	}
	__finally
	{
		src_data = null;
		dst_ptr = null;
	}

	return true;
}

bool GenericImageIO::FromFormat32bppPArgb(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	return FromFormat32bppRgb(
		bitmap, bitmapData, mapColors, width, height, stride, buffer);
}

bool GenericImageIO::FromFormat48bppRgb(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	uchar* src_data = (uchar*)bitmapData->get_Scan0().ToPointer();
	ushort* dst_ptr = buffer;

	try
	{
		ushort* src_ptr = null;
		
		for (int y=0; y < height; y++)
		{	
			src_ptr = (ushort*)(src_data + y * stride);
			for (int x=0; x < width; x++, dst_ptr++, src_ptr += 3)
			{
				// I assumed here that offset for each channel are: r = 2, g = 1, b = 0
				// This should review later, :)
				// Cong.
				//*dst_ptr = IntensityFromRGB((*(src_ptr + 2)) / 256, (*(src_ptr + 1)) / 256, (*src_ptr) / 256);
				*dst_ptr = IntensityFromRGB(((*(src_ptr + 2)) >> 8), ((*(src_ptr + 1)) >> 8), ((*src_ptr) >> 8));
			}
		}
	}
	catch(System::Exception* exp)
	{
		throw exp;
	}
	__finally
	{
		src_data = null;
		dst_ptr = null;
	}

	return true;
}

bool GenericImageIO::FromFormat64bppArgb(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	uchar* src_data = (uchar*)bitmapData->get_Scan0().ToPointer();
	ushort* dst_ptr = buffer;

	try
	{
		ushort* src_ptr = null;
		
		for (int y=0; y < height; y++)
		{	
			src_ptr = (ushort*)(src_data + y * stride);
			for (int x=0; x < width; x++, dst_ptr++, src_ptr += 4)
			{
				// I assumed here that offset for each channel are: r = 2, g = 1, b = 0, a = 3
				// This should review later, :)
				// Cong.
				*dst_ptr = IntensityFromRGBA(((*(src_ptr + 2)) >> 8), ((*(src_ptr + 1)) >> 8), ((*src_ptr) >> 8), ((*(src_ptr + 3)) >> 8));
			}
		}
	}
	catch(System::Exception* exp)
	{
		throw exp;
	}
	__finally
	{
		src_data = null;
		dst_ptr = null;
	}

	return true;
}

bool GenericImageIO::FromFormat64bppPArgb(
	Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
	int width, int height, int stride, ushort* buffer)
{
	uchar* src_data = (uchar*)bitmapData->get_Scan0().ToPointer();
	ushort* dst_ptr = buffer;

	try
	{
		ushort* src_ptr = null;
		
		for (int y=0; y < height; y++)
		{	
			src_ptr = (ushort*)(src_data + y * stride);
			for (int x=0; x < width; x++, dst_ptr++, src_ptr += 4)
			{
				// I assumed here that offset for each channel are: r = 2, g = 1, b = 0
				// This should review later, :)
				// Cong.
				*dst_ptr = IntensityFromRGB(((*(src_ptr + 2)) >> 8), ((*(src_ptr + 1)) >> 8), ((*src_ptr) >> 8));
			}
		}
	}
	catch(System::Exception* exp)
	{
		throw exp;
	}
	__finally
	{
		src_data = null;
		dst_ptr = null;
	}

	return true;
}


END_NAMESPACE