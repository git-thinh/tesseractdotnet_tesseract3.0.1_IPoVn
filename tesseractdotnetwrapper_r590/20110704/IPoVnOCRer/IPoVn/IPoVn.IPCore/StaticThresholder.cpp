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
#include "StaticThresholder.h"


BEGIN_NAMSPACE

USING_COMMON_SYSTEM

StaticThresholder::StaticThresholder(void)
{
}

StaticThresholder::~StaticThresholder(void)
{
}

BinaryImage* StaticThresholder::Threshold(GreyImage* image)
{
	BinaryImage* result = null;

	switch (_type)
	{
	case eStaticThresholdType::None:
		break;
	case eStaticThresholdType::KeepGreaterThanOrEquals:
		result = DoThresholding1(image);
		break;
	case eStaticThresholdType::KeepLessThanOrEquals:
		result = DoThresholding2(image);
		break;
	case eStaticThresholdType::KeepInRange:
		result = DoThresholding4(image);
		break;
	default:
		break;
	}

	return result;	
}

BinaryImage* StaticThresholder::DoThresholding1(GreyImage* image)
{
	BinaryImage* result = new BinaryImage(image->Width, image->Height);

	ushort* greyData = image->GreyData;
	byte* binData = result->BinaryData;

	int length = image->Width * image->Height;
	for (int i=0; i<length; i++, greyData++, binData++)
	{
		if (*greyData >= _threshold2)
			*binData = 1;
	}

	return result;
}

BinaryImage* StaticThresholder::DoThresholding2(GreyImage* image)
{
	BinaryImage* result = new BinaryImage(image->Width, image->Height);

	ushort* greyData = image->GreyData;
	byte* binData = result->BinaryData;

	int length = image->Width * image->Height;
	for (int i=0; i<length; i++, greyData++, binData++)
	{
		if (*greyData <= _threshold1)
			*binData = 1;
	}

	return result;
}

BinaryImage* StaticThresholder::DoThresholding4(GreyImage* image)
{
	BinaryImage* result = new BinaryImage(image->Width, image->Height);

	ushort* greyData = image->GreyData;
	byte* binData = result->BinaryData;

	int length = image->Width * image->Height;
	for (int i=0; i<length; i++, greyData++, binData++)
	{
		if (*greyData >= _threshold1 && *greyData <= _threshold2)
			*binData = 1;
	}

	return result;
}

END_NAMESPACE
