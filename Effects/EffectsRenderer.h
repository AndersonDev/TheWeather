#pragma once

#include "Common\DeviceResources.h"
#include "SpriteSheet.h"

namespace Effects
{
	public ref class EffectsRenderer sealed
    {
    public:
		EffectsRenderer(Windows::UI::Xaml::Controls::SwapChainPanel^ swapChain);
       
        void Draw();
		void SetLogicalSize(Windows::Foundation::Size logicalSize);
		void SetPointerPoint(Windows::UI::Input::PointerPoint^ pointerPoint);

    private:
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();

	private:
		std::shared_ptr<DX::DeviceResources>        m_deviceResources;

		SpriteSheet*								m_exampleSpriteSheet;
		Windows::UI::Input::PointerPoint^			m_pointerPoint;
    };
}