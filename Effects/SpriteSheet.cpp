#include "pch.h"
#include "SpriteSheet.h"
#include "Common\DirectXHelper.h"

using namespace concurrency;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Platform;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Input;

SpriteSheet::SpriteSheet(std::shared_ptr<DX::DeviceResources> deviceResources, Platform::String^ fileName, int spritesCount, D2D1_SIZE_F spriteSize)
{
	m_deviceResources = deviceResources;
	m_fileName = fileName;
	m_spritesCount = spritesCount;
	m_spriteSize = spriteSize;
	m_currentSprite = 0;

	LoadResources();
}

SpriteSheet::~SpriteSheet()
{
	if (m_bitmap) m_bitmap->Release();
}

void SpriteSheet::LoadResources()
{
	auto d2dContext = m_deviceResources->GetD2DDeviceContext();

	StorageFolder^ packageFolder = Windows::ApplicationModel::Package::Current->InstalledLocation;

	create_task(packageFolder->GetFileAsync("Assets\\" + m_fileName)).then([=](StorageFile^ file)
	{
		return file->OpenAsync(FileAccessMode::Read);

	}).then([=](IRandomAccessStream^ stream)
	{
		ComPtr<IStream> iStream;
		DX::ThrowIfFailed(
			CreateStreamOverRandomAccessStream(
				stream,
				IID_PPV_ARGS(&iStream)
			)
		);

		DX::ThrowIfFailed(
			LoadBitmapFromFile(
				d2dContext,
				m_deviceResources->GetWicImagingFactory(),
				iStream.Get(),
				&m_bitmap
			)
		);
	});
}

void SpriteSheet::DrawNext(D2D1_POINT_2F position)
{
	Draw(m_currentSprite, position);

	m_currentSprite++;
	if (m_currentSprite >= m_spritesCount) m_currentSprite = 0;
}

void SpriteSheet::Draw(int index, D2D1_POINT_2F position)
{
	auto d2dContext = m_deviceResources->GetD2DDeviceContext();

	if (!m_bitmap | !d2dContext) return;

	if (index > m_spritesCount) index = 0;

	int framesAccross = m_bitmap->GetPixelSize().width / m_spriteSize.width;

	D2D_RECT_F src = D2D1::RectF(
		(float)((index % framesAccross) * m_spriteSize.width),
		(float)((index / framesAccross) * m_spriteSize.height),
		(float)((index % framesAccross) * m_spriteSize.width) + m_spriteSize.width,
		(float)((index / framesAccross) * m_spriteSize.height) + m_spriteSize.height
	);

	D2D_RECT_F dest = D2D1::RectF(
		position.x,
		position.y,
		position.x + m_spriteSize.width,
		position.y + m_spriteSize.height
	);

	d2dContext->DrawBitmap(
		m_bitmap,
		dest,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		src
	);

	m_currentSprite = index;
}

HRESULT SpriteSheet::LoadBitmapFromFile(ID2D1DeviceContext5 * pD2DContext, IWICImagingFactory2 * pIWICFactory, IStream * spIWICStream, ID2D1Bitmap ** ppBitmap)
{
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;

	HRESULT hr = pIWICFactory->CreateDecoderFromStream(
		spIWICStream,
		NULL,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}


	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = pD2DContext->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
		);
	}

	if (pDecoder) pDecoder->Release();
	if (pSource) pSource->Release();
	if (pStream) pStream->Release();
	if (pConverter) pConverter->Release();
	if (pScaler) pScaler->Release();

	return hr;
}
