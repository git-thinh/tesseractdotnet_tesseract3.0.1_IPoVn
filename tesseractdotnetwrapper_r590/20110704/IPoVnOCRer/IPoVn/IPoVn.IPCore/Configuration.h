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

#define BEGIN_NAMSPACE	namespace	IPoVn {	namespace IPCore {

#define END_NAMESPACE	}; };

#define USING_COMMON_SYSTEM \
	using namespace System; \
	using namespace System::IO; \
	using namespace System::Text; \
	using namespace System::Collections; \
	using namespace System::Collections::Generic; \
	using namespace System::Drawing; \
	using namespace System::Drawing::Drawing2D; \
	using namespace System::Drawing::Imaging; \
	using namespace System::Drawing::Text; \
	using namespace System::Runtime::InteropServices;

#define USING_IPOVNSYSTEM \
	using namespace IPoVnSystem;

#ifndef NULL
#define NULL 0
#endif

#define null NULL

typedef unsigned char  uchar;
typedef unsigned char  byte;
typedef unsigned short ushort;

#define SAFE_DELETE(p)			if (p) {delete p; p = null;}
#define SAFE_DELETE_ARRAY(p)	if (p) {delete [] p; p = null;}
#define SAFE_DISPOSE(p)			if (p) { try {__try_cast<IDisposable*>(p)->Dispose(); } catch(Exception* /*e*/) {} __finally {p = null;} }

#define IntensityFromRGB(r, g, b) ((ushort)((r * 29 + g * 150 + b * 77 + 128) / 256.0F))
#define IntensityFromColor(color) (IntensityFromRGB(color.get_R(), color.get_G(), color.get_B()))
#define IntensityFromRGBA(r, g, b, a) ((ushort)(((ushort)((r * 29 + g * 150 + b * 77 + 128) / 256.0F)) * a / 255.0f))