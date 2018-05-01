#pragma once

#include "Common\DeviceResources.h"

class SpriteSheet
{
public:
	SpriteSheet(std::shared_ptr<DX::DeviceResources> deviceResources, Platform::String^ fileName, int spritesCount, D2D1_SIZE_F spriteSize);
	~SpriteSheet();

	void Draw(int index, D2D1_POINT_2F position);
	void DrawNext(D2D1_POINT_2F position);

	ID2D1Bitmap* GetBitmap() const { return m_bitmap; };
	D2D1_SIZE_F GetSpriteSize() { return m_spriteSize; }

private:
	void LoadResources();

	HRESULT LoadBitmapFromFile(ID2D1DeviceContext5 *pD2DContext, IWICImagingFactory2 *pIWICFactory, IStream *spIWICStream, ID2D1Bitmap **ppBitmap);

private:
	std::shared_ptr<DX::DeviceResources>		m_deviceResources;

	Platform::String^							m_fileName;
	ID2D1Bitmap*								m_bitmap;

	int											m_currentSprite;
	int											m_spritesCount;
	D2D1_SIZE_F									m_spriteSize;
};

