#include "pch.h"
#include "EffectsRenderer.h"
#include "Common\DirectXHelper.h"

using namespace Effects;

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

EffectsRenderer::EffectsRenderer(Windows::UI::Xaml::Controls::SwapChainPanel^ swapChain)
{
	m_deviceResources = std::make_shared<DX::DeviceResources>();
	m_deviceResources->SetSwapChainPanel(swapChain);

    CreateDeviceDependentResources();
}

void EffectsRenderer::SetLogicalSize(Windows::Foundation::Size logicalSize)
{
	m_deviceResources->SetLogicalSize(logicalSize);
}

void EffectsRenderer::CreateDeviceDependentResources()
{
	m_exampleSpriteSheet = new SpriteSheet(m_deviceResources, "SunAnimation.png", 13, D2D1::SizeF(200, 200));
}

void EffectsRenderer::ReleaseDeviceDependentResources()
{
}

void EffectsRenderer::Draw()
{
    ID2D1DeviceContext5* d2dContext = m_deviceResources->GetD2DDeviceContext();
    Size logicalSize = m_deviceResources->GetLogicalSize();
    
    d2dContext->BeginDraw();

    d2dContext->Clear(D2D1::ColorF(1, 1, 1, 0));

	if (m_exampleSpriteSheet)
	{
		auto spriteSize = m_exampleSpriteSheet->GetSpriteSize();

	/*	D2D1_POINT_2F dest = D2D1::Point2F(
			(logicalSize.Width - spriteSize.width) / 2.0f,
			(logicalSize.Height - spriteSize.height) / 2.0f
		);*/

		D2D1_POINT_2F dest = D2D1::Point2F(0, 0);

		if (m_pointerPoint)
		{
			dest.x = m_pointerPoint->Position.X - spriteSize.width / 2.0f;
			dest.y = m_pointerPoint->Position.Y - spriteSize.height / 2.0f;
		}

		m_exampleSpriteSheet->DrawNext(dest);
	}

    HRESULT hr = d2dContext->EndDraw();
    if (hr != D2DERR_RECREATE_TARGET)
    {
        DX::ThrowIfFailed(hr);
    }

    m_deviceResources->Present();
}

void EffectsRenderer::SetPointerPoint(Windows::UI::Input::PointerPoint^ pointerPoint)
{
	m_pointerPoint = pointerPoint;
}