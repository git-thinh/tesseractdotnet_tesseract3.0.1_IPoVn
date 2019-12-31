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

#include "ImageBase.h"
#include "GenericImageIO.h"

BEGIN_NAMSPACE

USING_COMMON_SYSTEM

public __gc class BinaryImage : public ImageBase
{
public:
	BinaryImage(void);
	~BinaryImage(void);

	BinaryImage(int width, int height) 
		: ImageBase(width, height, 1)
	{
	}

	BinaryImage(int width, int height, byte* data)
		: ImageBase(width, height, data)
	{
	}

public:
	__property byte* get_BinaryData()
	{
		return (byte*)_data;
	}
	
public:
	void Save(Imaging::ImageFormat* imgFmt, String* fileName)
	{
		int length = _width * _height;
		ushort* greyData = new ushort[length];

		memset(greyData, 0, length * sizeof(ushort));

		try
		{
			byte* binData = this->BinaryData;
			for (int i=0; i<length; i++)
			{
				if (*(binData + i) > 0)
					*(greyData + i) = 255;
				/*else
					*(greyData + i) = 0;*/
			}

			GenericImageIO::SaveGreyImage(
				_width, _height, greyData, imgFmt, fileName);
		}
		__finally
		{
			SAFE_DELETE(greyData);
		}
	}

public:
	void Invert()
	{
		int length = _width * _height;
		byte* data = this->BinaryData;
		for (int i=0; i<length; i++, data++)
		{
			*data = 1 - *data;
		}
	}

	ImageBase* Crop(int left, int top, int width, int height)
	{
		BinaryImage* binImage = new BinaryImage(width, height);

		byte* dst = binImage->BinaryData;
		byte* src = this->BinaryData;

		int copiedBytesPerRow = width * sizeof(byte);
		for (int y = 0; y<height; y++)
		{
			memcpy(dst + y*width, src + (y + top) * _width + left, copiedBytesPerRow);
		}

		return binImage;
	}
};

END_NAMESPACE