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

public __gc class GreyImage : public ImageBase
{
public:
	GreyImage(void);
	~GreyImage(void);

	GreyImage(int width, int height) 
		: ImageBase(width, height, 2)
	{
	}

	GreyImage(int width, int height, ushort* data)
		: ImageBase(width, height, data)
	{
	}

public:
	__property ushort* get_GreyData()
	{
		return (ushort*)_data;
	}

public:
	static GreyImage* FromFile(String* filePath);
	static GreyImage* FromImage(System::Drawing::Bitmap* bitmap);

public:
	void Save(Imaging::ImageFormat* imgFmt, String* fileName)
	{
		GenericImageIO::SaveGreyImage(
			_width, _height, this->GreyData, imgFmt, fileName);
	}

public:
	void Invert()
	{
		int length = _width * _height;
		ushort* data = this->GreyData;
		for (int i=0; i<length; i++, data++)
		{
			*data = 255 - *data;
		}
	}

	ImageBase* Crop(int left, int top, int width, int height)
	{
		GreyImage* greyImage = new GreyImage(width, height);

		ushort* dst = greyImage->GreyData;
		ushort* src = this->GreyData;

		int copiedBytesPerRow = width * sizeof(ushort);
		for (int y = 0; y<height; y++)
		{
			memcpy(dst + y*width, src + (y + top) * _width + left, copiedBytesPerRow);
		}

		return greyImage;
	}
};

END_NAMESPACE