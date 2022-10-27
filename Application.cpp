#include "Application.h"

void Application::load_resources() {
	// The factory pointer
	winrt::com_ptr <IWICImagingFactory> pFactory;

	// Create the COM imaging factory
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(pFactory.put())
	);
	if (FAILED(hr)) {
		SysError(NULL);
	}

	winrt::com_ptr<IWICBitmapDecoder> pDecoder;
	hr = pFactory->CreateDecoderFromFilename(L"D:\\programming\\BoxShooting\\BoxShooting\\resources\\cursor.png", NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, pDecoder.put());
	
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

	Texture texture;
	pConverter->GetSize(&texture.width, &texture.height);
	texture.pixels.resize(texture.width * texture.height);
	
	pConverter->CopyPixels(NULL, sizeof(Color) * texture.width, sizeof(Color) * (unsigned int)texture.pixels.size(), (BYTE*)texture.pixels.data());

	resources.textures[0] = texture;

}

Application::Application()
	:graphics(),
	display(WINDOW_WIDTH, WINDOW_HEIGHT)
{
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);

	load_resources();

	game = std::make_unique<Game>(Devices{ graphics, input_handler }, resources);
}

int Application::run() {
	reset_timer();

	MSG msg;
	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) {
				return (int)msg.wParam;
			}
		}
		doFrame();
	}
}

void Application::doFrame() {

	graphics.clear(Color(0, 0, 0));

	game->tick(reset_timer());

	display.swap(graphics.panel.data());
}