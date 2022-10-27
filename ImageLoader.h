#pragma once
#include "wincodec.h"
#include <winrt/base.h>
#include "Error.h"
#include "Graphics.h"

class ImageLoader {
private:
	winrt::com_ptr <IWICImagingFactory> pFactory;
public:
	ImageLoader();

	void load_image(LPCWSTR path, Texture& texture);

};
