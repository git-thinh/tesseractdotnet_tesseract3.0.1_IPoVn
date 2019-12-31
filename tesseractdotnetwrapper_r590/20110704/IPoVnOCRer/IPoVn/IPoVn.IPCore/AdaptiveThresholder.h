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

public __value enum eAdaptiveThresholdType : int
{
	None = 0,	
	KeepGreaterThanOrEquals = 1,	
	KeepLessThanOrEquals = 2
};

public __gc class AdaptiveThresholder : public ImageThresholder
{
protected:
	int _kWidth;
	int _kHeight;
	eAdaptiveThresholdType _type;

public:
	__property int get_kWidth()
	{
		return _kWidth;
	}

	__property void set_kWidth(int kSz)
	{
		_kWidth = ((((int)Math::Abs(kSz)) >> 1) << 1) + 1;
	}

	__property int get_kHeight()
	{
		return _kHeight;
	}

	__property void set_kHeight(int kSz)
	{
		_kHeight = ((((int)Math::Abs(kSz)) >> 1) << 1) + 1;
	}

public:
	AdaptiveThresholder(void);
	~AdaptiveThresholder(void);

public:
	BinaryImage* Threshold(GreyImage* image);

protected:
	BinaryImage* Threshold1(GreyImage* image);
	BinaryImage* Threshold2(GreyImage* image);
};

END_NAMESPACE