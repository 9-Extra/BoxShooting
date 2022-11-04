#pragma once
#include "wincodec.h"
#include <winrt/base.h>
#include "Error.h"
#include "ResourceManager.h"

class ImageLoader {
private:
	winrt::com_ptr <IWICImagingFactory> pFactory;
	ResourceManager& mng;
public:
	ImageLoader(ResourceManager& mng);

	void load_image(LPCWSTR path, const RES_TEXTURE texture_id);

};
