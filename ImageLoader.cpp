#include "ImageLoader.h"

ImageLoader::ImageLoader()
{
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(pFactory.put())
	);
	if (FAILED(hr)) {
		SysError(L"Fail to init IWICImagingFactory");
	}
}

void ImageLoader::load_image(LPCWSTR path, Texture& texture) {
	HRESULT hr;

	winrt::com_ptr<IWICBitmapDecoder> pDecoder;
	hr = pFactory->CreateDecoderFromFilename(path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, pDecoder.put());

	if (FAILED(hr)) {
		SysError(L"Fail to load image file");
	}

	winrt::com_ptr<IWICBitmapFrameDecode> pFrame;
	hr = pDecoder->GetFrame(0u, pFrame.put());
	if (FAILED(hr)) {
		SysError(NULL);
	}

	winrt::com_ptr<IWICFormatConverter> pConverter;
	hr = pFactory->CreateFormatConverter(pConverter.put());
	if (FAILED(hr)) {
		SysError(NULL);
	}

	hr = pConverter->Initialize(
		pFrame.get(),                  // Input source to convert
		GUID_WICPixelFormat32bppRGBA,   // Destination pixel format
		WICBitmapDitherTypeNone,         // Specified dither pattern
		NULL,                            // Specify a particular palette 
		1.f,                             // Alpha threshold
		WICBitmapPaletteTypeCustom       // Palette translation type
	);
	if (FAILED(hr)) {
		SysError(NULL);
	}

	pConverter->GetSize(&texture.width, &texture.height);
	texture.pixels.resize(texture.width * texture.height);

	pConverter->CopyPixels(NULL, sizeof(Color) * texture.width, sizeof(Color) * (unsigned int)texture.pixels.size(), (BYTE*)texture.pixels.data());
}