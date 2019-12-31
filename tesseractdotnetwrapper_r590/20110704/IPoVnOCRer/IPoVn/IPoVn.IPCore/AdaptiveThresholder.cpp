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
#include "AdaptiveThresholder.h"


BEGIN_NAMSPACE

USING_COMMON_SYSTEM

USING_IPOVNSYSTEM

AdaptiveThresholder::AdaptiveThresholder(void)
{
	_kWidth = 11;
	_kHeight = 11;
	_type = eAdaptiveThresholdType::KeepLessThanOrEquals;
}

AdaptiveThresholder::~AdaptiveThresholder(void)
{
}

BinaryImage* AdaptiveThresholder::Threshold(GreyImage* image)
{
	switch (_type)
	{
	case eAdaptiveThresholdType::None:
		break;
	case eAdaptiveThresholdType::KeepGreaterThanOrEquals:
		return Threshold1(image);
		break;
	case eAdaptiveThresholdType::KeepLessThanOrEquals:
		return Threshold2(image);
		break;
	default:
		break;
	}

	return null;
}

BinaryImage* AdaptiveThresholder::Threshold2(GreyImage* image)
{
	BinaryImage* result = null;		
	
	int width = image->Width;
	int height = image->Height;
	int length = image->Width * image->Height;
	double vNormalize = 128;
	double* integralImage = null;		
	try
	{
		result = new BinaryImage(image->Width, image->Height);

		ushort* greyData = image->GreyData;
		byte* binData = result->BinaryData;
		memset(binData, 1, length * sizeof(byte));

		integralImage = new double[length];
		Utilities::CalcIntegral(
			greyData, image->Width, image->Height, vNormalize, integralImage);

		double backgroundAmplitude = integralImage[length-1] / length + vNormalize;

		int offsetT_1L_1;
		int offsetT_1R;
		int offsetB_L_1;
		int offsetBR;
		KernelHelper::CalcOffsets(
			width, _kWidth, _kHeight, 
			&offsetT_1L_1, &offsetT_1R, &offsetB_L_1, &offsetBR);

		int offset = (_kHeight >> 1) * width + (_kWidth >> 1);

		int yStart = 1;
		int xStart = 1;
		int yEnd = height - _kHeight - 1;
		int xEnd = width - _kWidth - 1;
		int index = 0;
		int kLength = _kWidth * _kHeight;
		
		double v = 0, threshold = 0;

		for (int y = yStart; y <= yEnd; y++)
		{
			index = y * width + xStart;
			for (int x = xStart; x <= xEnd; x++, index++)
			{
				v = 
					integralImage[index + offsetBR] - 
					integralImage[index + offsetT_1R] - 
					integralImage[index + offsetB_L_1] + 
					integralImage[index + offsetT_1L_1];

				v = (v / kLength) + vNormalize;

				threshold = (v + backgroundAmplitude) * 0.5;

				binData[index + offset] = (greyData[index + offset] >= threshold ? 1 : 0);
			}
		}
	}
	catch (System::Exception *exp)
	{
		SAFE_DISPOSE(result);

		throw exp;
	}
	__finally
	{
		SAFE_DELETE(integralImage);
	}
	return result;
}

BinaryImage* AdaptiveThresholder::Threshold1(GreyImage* image)
{
	BinaryImage* result = null;		
	
	int width = image->Width;
	int height = image->Height;
	int length = image->Width * image->Height;
	double vNormalize = 128;
	double* integralImage = null;		
	try
	{
		result = new BinaryImage(image->Width, image->Height);

		ushort* greyData = image->GreyData;
		byte* binData = result->BinaryData;
		memset(binData, 1, length * sizeof(byte));

		integralImage = new double[length];
		Utilities::CalcIntegral(
			greyData, image->Width, image->Height, vNormalize, integralImage);

		double backgroundAmplitude = integralImage[length-1] / length + vNormalize;

		int offsetT_1L_1;
		int offsetT_1R;
		int offsetB_L_1;
		int offsetBR;
		KernelHelper::CalcOffsets(
			width, _kWidth, _kHeight, 
			&offsetT_1L_1, &offsetT_1R, &offsetB_L_1, &offsetBR);

		int offset = (_kHeight >> 1) * width + (_kWidth >> 1);

		int yStart = 1;
		int xStart = 1;
		int yEnd = height - _kHeight - 1;
		int xEnd = width - _kWidth - 1;
		int index = 0;
		int kLength = _kWidth * _kHeight;
		
		double v = 0, threshold = 0;

		for (int y = yStart; y <= yEnd; y++)
		{
			index = y * width + xStart;
			for (int x = xStart; x <= xEnd; x++, index++)
			{
				v = 
					integralImage[index + offsetBR] - 
					integralImage[index + offsetT_1R] - 
					integralImage[index + offsetB_L_1] + 
					integralImage[index + offsetT_1L_1];

				v = (v / kLength) + vNormalize;

				threshold = (v + backgroundAmplitude) * 0.5;
				
				binData[index + offset] = (greyData[index + offset] <= threshold ? 1 : 0);
			}
		}
	}
	catch (System::Exception *exp)
	{
		SAFE_DISPOSE(result);

		throw exp;
	}
	__finally
	{
		SAFE_DELETE(integralImage);
	}
	return result;
}

END_NAMESPACE
