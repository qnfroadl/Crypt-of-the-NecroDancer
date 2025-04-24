#include "Image.h"

HRESULT Image::Init(int width, int height)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

    imageInfo->hTempDC = CreateCompatibleDC(hdc);
    imageInfo->hTempBit = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldTemp = (HBITMAP)SelectObject(imageInfo->hTempDC, imageInfo->hTempBit);

    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_TYPE::Empty;

    isTransparent = false;
    transColor = NULL;

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    return S_OK;   // S_OK, E_FAIL
}

HRESULT Image::Init(const wchar_t* filePath, int width, int height,
    bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = (HBITMAP)LoadImage(
        g_hInstance, filePath, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

    imageInfo->hTempDC = CreateCompatibleDC(hdc);
    imageInfo->hTempBit = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldTemp = (HBITMAP)SelectObject(imageInfo->hTempDC, imageInfo->hTempBit);

    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_TYPE::File;

    imageInfo->maxFrameX = 0;
    imageInfo->maxFrameY = 0;
    imageInfo->frameWidth = 0;
    imageInfo->frameHeight = 0;
    
    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    return S_OK;   // S_OK, E_FAIL
}

HRESULT Image::Init(const wchar_t* filePath, int width, int height, int maxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = (HBITMAP)LoadImage(
        g_hInstance, filePath, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_TYPE::File;

    imageInfo->maxFrameX = maxFrameX;
    imageInfo->maxFrameY = maxFrameY;
    imageInfo->frameWidth = width / maxFrameX;
    imageInfo->frameHeight = height / maxFrameY;

    imageInfo->hTempDC = CreateCompatibleDC(hdc);
    imageInfo->hTempBit = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldTemp = (HBITMAP)SelectObject(imageInfo->hTempDC, imageInfo->hTempBit);

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    return S_OK;   // S_OK, E_FAIL
}

HRESULT Image::Init(const wchar_t* filePath, int width, int height, int maxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor, bool gdiActive)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = (HBITMAP)LoadImage(
        g_hInstance, filePath, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_TYPE::File;

    imageInfo->maxFrameX = maxFrameX;
    imageInfo->maxFrameY = maxFrameY;
    imageInfo->frameWidth = width / maxFrameX;
    imageInfo->frameHeight = height / maxFrameY;

    imageInfo->hTempDC = CreateCompatibleDC(hdc);
    imageInfo->hTempBit = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldTemp = (HBITMAP)SelectObject(imageInfo->hTempDC, imageInfo->hTempBit);

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

	if (gdiActive)
	{
		imageInfo->gdiBitmap = new Gdiplus::Bitmap(filePath);
		if (imageInfo->gdiBitmap == nullptr)
		{
			Release();
			return E_FAIL;
		}
	}
	else
	{
		imageInfo->gdiBitmap = nullptr;
	}

    return S_OK;   // S_OK, E_FAIL
}

void Image::Render(HDC hdc, int destX, int destY)
{
    if (isTransparent)
    {
        GdiTransparentBlt(hdc,
            destX, destY,
            imageInfo->width, imageInfo->height,
            imageInfo->hMemDC,
            0, 0,
            imageInfo->width, imageInfo->height,
            transColor);
    }
    else
    {
        BitBlt(
            hdc,                // 복사 목적지 DC
            destX, destY,       // 복사 목적지 위치
            imageInfo->width,   // 원본에서 복사될 가로크기
            imageInfo->height,  // 원본에서 복사될 세로크기
            imageInfo->hMemDC,  // 원본 DC
            0, 0,               // 원본 복사 시작 위치
            SRCCOPY             // 복사 옵션
        );
    }
}

void Image::RenderCenter(HDC hdc, int destX, int destY)
{
    destX = destX - imageInfo->width / 2;
    destY = destY - imageInfo->height / 2;

    if (isTransparent)
    {
        GdiTransparentBlt(hdc,
            destX, destY,
            imageInfo->width, imageInfo->height,
            imageInfo->hMemDC,
            0, 0,
            imageInfo->width, imageInfo->height,
            transColor);
    }
    else
    {
        BitBlt(
            hdc,                // 복사 목적지 DC
            destX, destY,       // 복사 목적지 위치
            imageInfo->width,   // 원본에서 복사될 가로크기
            imageInfo->height,  // 원본에서 복사될 세로크기
            imageInfo->hMemDC,  // 원본 DC
            0, 0,               // 원본 복사 시작 위치
            SRCCOPY             // 복사 옵션
        );
    }
}


void Image::FrameRender(HDC hdc, int destX, int destY, int frameX, int frameY, bool isFlip, bool isCenter)
{
    if (isCenter)
    {
        destX = destX - imageInfo->frameWidth/2;
        destY = destY - imageInfo->frameHeight/2;
    }
    
    if (isFlip && isTransparent)
    {
        StretchBlt(imageInfo->hTempDC, 0, 0,
            imageInfo->frameWidth, imageInfo->frameHeight, // 여기부터
            imageInfo->hMemDC,
            (imageInfo->frameWidth * frameX) + (imageInfo->frameWidth + 1), // 여기까지?
            imageInfo->frameHeight * frameY, // 여기까지
            -imageInfo->frameWidth, imageInfo->frameHeight, // 이 크기로?
            SRCCOPY
        );

        GdiTransparentBlt(hdc,
            destX, destY, // 여기가 원래 destX 였음.
            imageInfo->frameWidth, imageInfo->frameHeight,

            imageInfo->hTempDC,
            0, 0,
            imageInfo->frameWidth, imageInfo->frameHeight,
            transColor);
    }
    else if (isTransparent)
    {
        GdiTransparentBlt(hdc,
            destX, destY,
            imageInfo->frameWidth, imageInfo->frameHeight,

            imageInfo->hMemDC,
            imageInfo->frameWidth * frameX,
            imageInfo->frameHeight * frameY,
            imageInfo->frameWidth, imageInfo->frameHeight,
            transColor);
    }
    else
    {
        BitBlt(
            hdc,
            destX, destY,
            imageInfo->frameWidth,
            imageInfo->frameHeight,
            imageInfo->hMemDC,
            imageInfo->frameWidth * frameX,
            imageInfo->frameHeight * frameY,
            SRCCOPY
        );
    }


}

void Image::FrameRender(HDC hdc, int destX, int destY, int frameX, int frameY, float sizeX, float sizeY, bool isFlip, bool isCenter)
{
    if (isCenter)
    {
        destX = destX - imageInfo->frameWidth * sizeX / 2.f;
        destY = destY - imageInfo->frameHeight * sizeY / 2.f;
    }
    
    if (isFlip && isTransparent)
    {
        StretchBlt(imageInfo->hTempDC, 0, 0,
            imageInfo->frameWidth, imageInfo->frameHeight,
            imageInfo->hMemDC,
            (imageInfo->frameWidth * frameX) + (imageInfo->frameWidth + 1),
            imageInfo->frameHeight * frameY,
            -imageInfo->frameWidth, imageInfo->frameHeight,
            SRCCOPY
        );

        GdiTransparentBlt(hdc,
            destX, destY,
            imageInfo->frameWidth * sizeX, imageInfo->frameHeight * sizeY,
            imageInfo->hTempDC,
            0, 0,
            imageInfo->frameWidth, imageInfo->frameHeight,
            transColor);
    }
    else if (isTransparent)
    {
        GdiTransparentBlt(hdc,
            destX, destY,
            imageInfo->frameWidth * sizeX, imageInfo->frameHeight * sizeY,
            imageInfo->hMemDC,
            imageInfo->frameWidth * frameX,
            imageInfo->frameHeight * frameY,
            imageInfo->frameWidth, imageInfo->frameHeight,
            transColor);
    }
    else
    {
        BitBlt(
            hdc,
            destX, destY,
            imageInfo->frameWidth * sizeX,
            imageInfo->frameHeight * sizeY,
            imageInfo->hMemDC,
            imageInfo->frameWidth * frameX,
            imageInfo->frameHeight * frameY,
            SRCCOPY
        );
    }
 }

void Image::FrameRender(HDC hdc, float destX, float destY, int frameX, int frameY, float sizeX, float sizeY, bool isFlip, bool isCenter, float alpha, float angle)
{
    if (imageInfo->gdiBitmap == nullptr) return;

    Gdiplus::Graphics graphics(hdc);
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeNone);
    graphics.SetCompositingQuality(Gdiplus::CompositingQualityHighSpeed);

    int width = imageInfo->gdiBitmap->GetWidth();
    int height = imageInfo->gdiBitmap->GetHeight();

	float frameWidth = width / imageInfo->maxFrameX;
	float frameHeight = height / imageInfo->maxFrameY;

	float destWidth = frameWidth * sizeX;
	float destHeight = frameHeight * sizeY;

    float destOffsetX = -(float)destWidth * 0.5f;
    float destOffsetY = -(float)destHeight * 0.5f;

    graphics.TranslateTransform(-frameWidth * 0.5f, -frameHeight * 0.5f, Gdiplus::MatrixOrderAppend);

    // 회전
    graphics.RotateTransform(angle, Gdiplus::MatrixOrderAppend);
    if (isFlip)
    {
        graphics.ScaleTransform(-1, 1, Gdiplus::MatrixOrderAppend);
    }
    graphics.ScaleTransform(sizeX, sizeY, Gdiplus::MatrixOrderAppend);

    graphics.TranslateTransform(destX, destY, Gdiplus::MatrixOrderAppend);

    // 알파 블렌딩
    Gdiplus::ColorMatrix colorMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, alpha, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    Gdiplus::ImageAttributes imgAttr;
    imgAttr.SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeDefault);
	imgAttr.SetColorKey(Gdiplus::Color(255, 0, 255), Gdiplus::Color(255, 0, 255));

    graphics.DrawImage(imageInfo->gdiBitmap,
        Gdiplus::Rect(0, 0, frameWidth, frameHeight),
        frameWidth * frameX, frameHeight * frameY,
        frameWidth, frameHeight,
        Gdiplus::UnitPixel, &imgAttr);
}


void Image::RenderScaledImage(HDC hdc, int destX, int destY, int frameX, int frameY, float scaleX, float scaleY, bool isCenter)
{
   
   int frameW = imageInfo->frameWidth;
   int frameH = imageInfo->frameHeight;
   int scaledW = static_cast<int>(round(frameW * scaleX));
   int scaledH = static_cast<int>(round(frameH * scaleY));

   if (isCenter)
   {
       destX = destX - scaledW / 2;
       destY = destY - scaledH / 2;
   }

   HDC tempDC = CreateCompatibleDC(hdc);
   HBITMAP tempBitmap = CreateCompatibleBitmap(hdc, scaledW, scaledH);
   HBITMAP oldBitmap = (HBITMAP)SelectObject(tempDC, tempBitmap);

   SetStretchBltMode(tempDC, COLORONCOLOR);

   GdiTransparentBlt(
       tempDC, 0, 0, scaledW, scaledH,
       imageInfo->hMemDC,
       frameX * frameW,
       frameY * frameH,
       frameW,
       frameH,
       transColor
   );

   BitBlt(hdc, destX, destY, scaledW, scaledH, tempDC, 0, 0, SRCCOPY);

   SelectObject(tempDC, oldBitmap);
   DeleteObject(tempBitmap);
   DeleteDC(tempDC);
}

void Image::RenderHeight(HDC hdc, int destX, int destY, int frameIndex, int totalFrame)
{
    BitBlt(
        hdc,									// 복사 목적지 DC
        destX, destY,							// 복사 목적지 위치
        imageInfo->width, imageInfo->height / totalFrame,	// 원본에서 복사될 크기
        imageInfo->hMemDC,						// 원본 DC
        0, imageInfo->height / totalFrame * frameIndex,									// 원본 복사 시작 위치
        SRCCOPY									// 복사 옵션
    );
}

void Image::Release()
{
    if (imageInfo)
    {
        SelectObject(imageInfo->hTempDC, imageInfo->hOldTemp);
        DeleteObject(imageInfo->hTempBit);
        DeleteDC(imageInfo->hTempDC);

        SelectObject(imageInfo->hMemDC, imageInfo->hOldBit);
        DeleteObject(imageInfo->hBitmap);
        DeleteDC(imageInfo->hMemDC);

		if (imageInfo->gdiBitmap)
		{
			delete imageInfo->gdiBitmap;
			imageInfo->gdiBitmap = nullptr;
		}

        delete imageInfo;
        imageInfo = nullptr;
    }
}