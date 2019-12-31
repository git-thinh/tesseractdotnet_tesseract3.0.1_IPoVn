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

#include "ImageThresholder.h"
#include "GreyImage.h"
#include "BinaryImage.h"

BEGIN_NAMSPACE

USING_COMMON_SYSTEM

public __value enum eStaticThresholdType : int
{
	None = 0,	
	KeepGreaterThanOrEquals = 1,	
	KeepLessThanOrEquals = 2,
	KeepInRange = 4
};

public __gc class StaticThresholder : public ImageThresholder
{
protected:
	double _threshold1;
	double _threshold2;
	eStaticThresholdType _type;

public:
	__property double get_Threshold1()
	{
		return _threshold1;
	}

	__property void set_Threshold1(double val)
	{
		_threshold1 = val;
	}

	__property double get_Threshold2()
	{
		return _threshold2;
	}

	__property void set_Threshold2(double val)
	{
		_threshold2 = val;
	}
	
	__property eStaticThresholdType get_ThresholdType()
	{
		return _type;
	}

	__property void set_ThresholdType(eStaticThresholdType type)
	{
		_type = type;
	}
public:
	StaticThresholder(void);
	~StaticThresholder(void);

public:
	BinaryImage* Threshold(GreyImage* image);

protected:
	BinaryImage* DoThresholding1(GreyImage* image);
	BinaryImage* DoThresholding2(GreyImage* image);
	BinaryImage* DoThresholding4(GreyImage* image);
};

END_NAMESPACE