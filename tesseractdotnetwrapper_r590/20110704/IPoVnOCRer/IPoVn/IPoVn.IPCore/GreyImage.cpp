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
#include "GreyImage.h"
#include "GenericImageIO.h"

BEGIN_NAMSPACE

USING_COMMON_SYSTEM

GreyImage::GreyImage(void)
{
}

GreyImage::~GreyImage(void)
{
}

GreyImage* GreyImage::FromFile(String* filePath)
{
	GreyImage *greyImage = null;
	System::Drawing::Bitmap* bitmap = null;
	try
	{
		greyImage = FromImage(bitmap);
	}
	catch (System::Exception *exp)
	{
		SAFE_DISPOSE(greyImage);
		throw exp;
	}
	__finally
	{
		SAFE_DISPOSE(bitmap);
	}

	return greyImage;
}

GreyImage* GreyImage::FromImage(System::Drawing::Bitmap* bitmap)
{
	int width = bitmap->Width;
	int height = bitmap->Height;

	GreyImage* greyImage = new GreyImage(width, height);
	ushort* data = (ushort*)greyImage->Data;

	GenericImageIO::LoadGreyImage(bitmap, data);

	return greyImage;
}

END_NAMESPACE