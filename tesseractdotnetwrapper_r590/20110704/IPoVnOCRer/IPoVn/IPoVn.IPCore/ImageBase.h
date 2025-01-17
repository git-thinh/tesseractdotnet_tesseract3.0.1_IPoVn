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

__gc public class ImageBase : public IDisposable
{
protected:
	int _width;
	int _height;
	int _length;
	void* _data;

public:
	__property int get_Width()
	{
		return _width;
	}

	__property int get_Height()
	{
		return _height;
	}

	__property void* get_Data()
	{
		return _data;
	}

protected:
	ImageBase(void);
	~ImageBase(void);

	ImageBase(int width, int height, int bytesPerPixel)
	{
		_width = width;
		_height = height;
		_length = _width * _height;

		int byteLength = _length * bytesPerPixel;
		_data = new byte[byteLength];
	}

	ImageBase(int width, int height, void* data)
	{
		_width = width;
		_height = height;
		_length = _width * _height;

		_data = data;
	}

protected:
	virtual void FreeMemory()
	{
		SAFE_DELETE(_data);
	}

public:
	void Dispose()
	{
		FreeMemory();
	}

public:
	virtual void Save(Imaging::ImageFormat* imgFmt, String* fileName)
	{
	}

public:
	virtual void Invert()
	{
	}

public:
	virtual ImageBase* Crop(int left, int top, int width, int height)
	{
		return null;
	}
};

END_NAMESPACE