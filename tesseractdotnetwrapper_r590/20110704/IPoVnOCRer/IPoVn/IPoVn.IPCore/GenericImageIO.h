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

#pragma once

BEGIN_NAMSPACE

USING_COMMON_SYSTEM


public __gc class GenericImageIO
{
public:
	GenericImageIO(void);
	~GenericImageIO(void);

protected:
	static void RetrievePalette(Bitmap* bitmap, int &num_colors, ushort** mapColors)
	{
		PixelFormat pxFormat = bitmap->PixelFormat;
		if (pxFormat != PixelFormat::Format1bppIndexed &&
			pxFormat != PixelFormat::Format4bppIndexed &&
			pxFormat != PixelFormat::Format8bppIndexed)
			return;

		System::Drawing::Imaging::ColorPalette* palette = bitmap->get_Palette();
		Color colors __gc[] = palette->get_Entries();
		num_colors = colors->get_Length();
		ushort* ptr = new ushort[num_colors];
		for (int i=0; i<num_colors; i++)
		{
			*(ptr + i) = IntensityFromColor(colors[i]);
		}

		*mapColors = ptr;
	}

	static void Prepare(
		Bitmap* bitmap, BitmapData* bitmapData,  
		int &stride, int &num_colors, ushort** mapColors)
	{
		if (bitmap == null || bitmapData == null)
			throw new System::ArgumentNullException("Parameters is not set to a reference");

		ushort* colorsPtr = null;		
		try
		{
			// retrieve image information
			stride = bitmapData->get_Stride();
			
			// retrieve palette information
			RetrievePalette(bitmap, num_colors, &colorsPtr);
		}
		catch (System::Exception* exp)
		{
			SAFE_DELETE(colorsPtr);

			throw exp;
		}
		__finally
		{
			*mapColors = colorsPtr;
		}
	}

	static void Prepare(Bitmap* bitmap, BitmapData* bitmapData, 
		int &width, int& height, int &stride, ushort** buffer, int &num_colors, ushort** mapColors)
	{
		if (bitmap == null || bitmapData == null)
			throw new System::ArgumentNullException("Parameters is not set to a reference");

		ushort* colorsPtr = null;
		ushort* dataPtr = null;
		try
		{
			// retrieve image information
			width = bitmapData->get_Width();
			height = bitmapData->get_Height();
			stride = bitmapData->get_Stride();
			
			// allocate memory
			dataPtr = new ushort[width * height];
			if (dataPtr == null)
				throw new System::OutOfMemoryException();
			
			// retrieve palette information
			RetrievePalette(bitmap, num_colors, &colorsPtr);
		}
		catch (System::Exception* exp)
		{
			SAFE_DELETE(dataPtr);
			SAFE_DELETE(colorsPtr);

			throw exp;
		}
		__finally
		{
			*buffer = dataPtr;
			*mapColors = colorsPtr;
		}
	}
protected:
	static bool FromFormat1bppIndexed(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);
	
	static bool FromFormat4bppIndexed(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);

	static bool FromFormat8bppIndexed(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);
	
	static bool FromFormat16bppGrayScale(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);

	static bool FromFormat16bppArgb1555(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);
	
	static bool FromFormat16bppRgb555(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);

	static bool FromFormat16bppRgb565(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);
	
	static bool FromFormat24bppRgb(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);

	static bool FromFormat32bppRgb(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);
	
	static bool FromFormat32bppArgb(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);

	static bool FromFormat32bppPArgb(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);
	
	static bool FromFormat48bppRgb(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);

	static bool FromFormat64bppArgb(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);
	
	static bool FromFormat64bppPArgb(
		Bitmap* bitmap, BitmapData* bitmapData, ushort* mapColors,
		int width, int height, int stride, ushort* buffer);

public:
	static bool LoadGreyImage(
		Bitmap* bitmap, int &width, int &height, ushort** buffer)
	{
		if (bitmap == NULL)
			throw new System::ArgumentNullException("Invalid parameter");

		ushort* mapColors = null;
		ushort* dst_ptr = null;

		System::Drawing::Imaging::BitmapData* bitmapData = null;
		try
		{
			PixelFormat format = bitmap->PixelFormat;
			GraphicsUnit unit = GraphicsUnit::Pixel;
			System::Drawing::Rectangle rcBound(0, 0, bitmap->Width, bitmap->Height);
			bitmapData = bitmap->LockBits(rcBound, ImageLockMode::ReadOnly, format);
			PixelFormat src_pixFmt = bitmapData->get_PixelFormat();
			int stride = bitmapData->Stride;
			int num_colors = 0;
			Prepare(bitmap, bitmapData, width, height, stride, &dst_ptr, num_colors, &mapColors);

			if (src_pixFmt == PixelFormat::Format1bppIndexed)
				FromFormat1bppIndexed(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format4bppIndexed)
				FromFormat4bppIndexed(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format8bppIndexed)
				FromFormat8bppIndexed(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format16bppGrayScale)
				FromFormat16bppGrayScale(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format16bppArgb1555)
				FromFormat16bppArgb1555(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format16bppRgb555)
				FromFormat16bppRgb555(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format16bppRgb565)
				FromFormat16bppRgb565(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format24bppRgb)
				FromFormat24bppRgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format32bppArgb)
				FromFormat32bppArgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format32bppPArgb)
				FromFormat32bppPArgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format32bppRgb)
				FromFormat32bppRgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format48bppRgb)
				FromFormat48bppRgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format64bppArgb)
				FromFormat64bppArgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format64bppPArgb)
				FromFormat64bppPArgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);			
		}
		catch(System::Exception *exp)
		{
			SAFE_DELETE(dst_ptr);
			throw exp;
		}
		__finally
		{
			if (bitmapData != NULL)
			{
				bitmap->UnlockBits(bitmapData);
				bitmapData = NULL;
			}

			SAFE_DELETE(mapColors);

			if (dst_ptr != null)
			{
				*buffer = dst_ptr;
			}
		}

		return true;
	}

	static void LoadGreyImage(Bitmap* bitmap, ushort* imageData)
	{
		if (bitmap == null)
			throw new System::ArgumentNullException("Invalid parameter");

		ushort* mapColors = null;
		ushort* dst_ptr = null;

		System::Drawing::Imaging::BitmapData* bitmapData = null;
		try
		{
			PixelFormat format = bitmap->PixelFormat;
			GraphicsUnit unit = GraphicsUnit::Pixel;
			System::Drawing::Rectangle rcBound(0, 0, bitmap->Width, bitmap->Height);
			bitmapData = bitmap->LockBits(rcBound, ImageLockMode::ReadOnly, format);
			PixelFormat src_pixFmt = bitmapData->get_PixelFormat();
			int stride = bitmapData->Stride;
			int num_colors = 0;
			int width = bitmapData->Width;
			int height = bitmapData->Height;
			Prepare(bitmap, bitmapData, stride, num_colors, &mapColors);

			dst_ptr = imageData;

			if (src_pixFmt == PixelFormat::Format1bppIndexed)
			{
				FromFormat1bppIndexed(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			}
			else if (src_pixFmt == PixelFormat::Format4bppIndexed)
			{
				FromFormat4bppIndexed(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			}
			else if (src_pixFmt == PixelFormat::Format8bppIndexed)
			{
				FromFormat8bppIndexed(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			}
			else if (src_pixFmt == PixelFormat::Format16bppGrayScale)
				FromFormat16bppGrayScale(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format16bppArgb1555)
				FromFormat16bppArgb1555(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format16bppRgb555)
				FromFormat16bppRgb555(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format16bppRgb565)
				FromFormat16bppRgb565(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format24bppRgb)
				FromFormat24bppRgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format32bppArgb)
				FromFormat32bppArgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format32bppPArgb)
				FromFormat32bppPArgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format32bppRgb)
				FromFormat32bppRgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format48bppRgb)
				FromFormat48bppRgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format64bppArgb)
				FromFormat64bppArgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);
			else if (src_pixFmt == PixelFormat::Format64bppPArgb)
				FromFormat64bppPArgb(bitmap, bitmapData, mapColors, width, height, stride, dst_ptr);			
		}
		catch(System::Exception *exp)
		{
			throw exp;
		}
		__finally
		{
			if (bitmapData != null)
			{
				bitmap->UnlockBits(bitmapData);
				bitmapData = null;
			}

			dst_ptr = null;
			SAFE_DELETE(mapColors);
		}
	}

	static bool SaveGreyImage(
		int width, int height, ushort* data, 
		Imaging::ImageFormat* imgFmt, String* fileName)
	{
		Bitmap* bitmap = null;
		try
		{
			bitmap = new Bitmap(width, height, PixelFormat::Format8bppIndexed);
			PixelFormat format = bitmap->PixelFormat;
			System::Drawing::Rectangle rcBound(0, 0, bitmap->Width, bitmap->Height);
			BitmapData* bitmapData = null;
			try
			{
				bitmapData = bitmap->LockBits(rcBound, ImageLockMode::WriteOnly, format);
				
				// update palete
				System::Drawing::Imaging::ColorPalette* palette = bitmap->get_Palette();								
				Color colors __gc[] = palette->Entries;
				int num_colors = colors->Length;
				for (int i=0; i < num_colors; i++)
				{
					colors[i] = Color::FromArgb(i, i, i); 
				}
				bitmap->Palette = palette;

				PixelFormat src_pixFmt = bitmapData->get_PixelFormat();
				ushort *ptr = data;

				if (src_pixFmt == PixelFormat::Format8bppIndexed)
				{					
					int stride = bitmapData->Stride;
					uchar* dst_data = (uchar*)bitmapData->get_Scan0().ToPointer();
					uchar* dst_ptr = null;
					for (int y=0; y<height; y++)
					{
						dst_ptr = dst_data + y * stride;
						for (int x=0; x<width; x++, dst_ptr++, ptr++)
						{
							*dst_ptr = (uchar)(*ptr);
						}
					}

					ptr = null;

					bitmap->Save(fileName, imgFmt);
				}
			}
			catch (System::Exception *exp)
			{
				throw exp;
			}
			__finally
			{
				if (bitmap != null && bitmapData != null)
					bitmap->UnlockBits(bitmapData);
				bitmapData = null;
			}
		}
		catch (System::Exception *exp)
		{
			throw exp;
		}
		__finally
		{
			SAFE_DISPOSE(bitmap);
		}

		return true;
	}
};

END_NAMESPACE