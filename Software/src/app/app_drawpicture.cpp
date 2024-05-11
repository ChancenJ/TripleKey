#include "app_drawpicture.h"

PNG png;
File pngFile;
int16_t xOffset, yOffset;

void *myOpen(const char *filename, int32_t *size)
{

    pngFile = LittleFS.open(filename, "r");
    // pngFile = SPIFFS.open(filename, "r");

    if (!pngFile || pngFile.isDirectory())
    {
        // Serial.println(F("ERROR: Failed to open  file for reading"));
        // gfx2->println(F("ERROR: Failed to open  file for reading"));
    }
    else
    {
        *size = pngFile.size();
        // Serial.printf("Opened '%s', size: %d\n", filename, *size);
    }

    return &pngFile;
}

void myClose(void *handle)
{
    if (pngFile)
        pngFile.close();
}

int32_t myRead(PNGFILE *handle, uint8_t *buffer, int32_t length)
{
    if (!pngFile)
        return 0;
    return pngFile.read(buffer, length);
}

int32_t mySeek(PNGFILE *handle, int32_t position)
{
    if (!pngFile)
        return 0;
    return pngFile.seek(position);
}

// Function to draw pixels to the display
void PNGDraw1(PNGDRAW *pDraw)
{
    uint16_t usPixels[320];
    uint8_t usMask[320];

    // Serial.printf("Draw pos = 0,%d. size = %d x 1\n", pDraw->y, pDraw->iWidth);
    png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0x00000000);
    png.getAlphaMask(pDraw, usMask, 1);
    gfx1->draw16bitRGBBitmap(xOffset, yOffset + pDraw->y, usPixels, pDraw->iWidth, 1);
}

void PNGDraw2(PNGDRAW *pDraw)
{
    uint16_t usPixels[320];
    uint8_t usMask[320];

    // Serial.printf("Draw pos = 0,%d. size = %d x 1\n", pDraw->y, pDraw->iWidth);
    png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0x00000000);
    png.getAlphaMask(pDraw, usMask, 1);
    gfx2->draw16bitRGBBitmap(xOffset, yOffset + pDraw->y, usPixels, pDraw->iWidth, 1);
}

void PNGDraw3(PNGDRAW *pDraw)
{
    uint16_t usPixels[320];
    uint8_t usMask[320];

    // Serial.printf("Draw pos = 0,%d. size = %d x 1\n", pDraw->y, pDraw->iWidth);
    png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0x00000000);
    png.getAlphaMask(pDraw, usMask, 1);
    gfx3->draw16bitRGBBitmap(xOffset, yOffset + pDraw->y, usPixels, pDraw->iWidth, 1);
}

void myDrawPNG(int16_t x, int16_t y, const char *path, uint8_t oled_index)
{
    unsigned long start = millis();
    int rc;
    PNG_DRAW_CALLBACK *pfnDraw = NULL;
    switch (oled_index)
    {
    case 0:
        pfnDraw = PNGDraw1;
        break;
    case 1:
        pfnDraw = PNGDraw2;
        break;
    case 2:
        pfnDraw = PNGDraw3;
        break;
    default:
        break;
    }

    rc = png.open(path, myOpen, myClose, myRead, mySeek, pfnDraw);
    if (rc == PNG_SUCCESS)
    {
        // int16_t pw = png.getWidth();
        // int16_t ph = png.getHeight();

        xOffset = x; //+(OLED_WIDTH - pw) / 2;
        yOffset = y; //+(OLED_HEIGHT - ph) / 2;

        rc = png.decode(NULL, 0);

        // Serial.printf("Draw offset: (%d, %d), time used: %lu\r\n", xOffset, yOffset, millis() - start);
        // Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\r\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
        png.close();
        // delay(5000);
    }
    else
    {
        gfx[oled_index]->setFont(u8g_font_5x7);
        gfx[oled_index]->setCursor(x, y + 20);
        gfx[oled_index]->printf("Failed to open %s\n", path);
    }
}

void DrawPNGCentre(const char *path, uint8_t oled_index)
{
    unsigned long start = millis();
    int rc;
    PNG_DRAW_CALLBACK *pfnDraw = NULL;
    switch (oled_index)
    {
    case 0:
        pfnDraw = PNGDraw1;
        break;
    case 1:
        pfnDraw = PNGDraw2;
        break;
    case 2:
        pfnDraw = PNGDraw3;
        break;
    default:
        break;
    }
    rc = png.open(path, myOpen, myClose, myRead, mySeek, pfnDraw);
    if (rc == PNG_SUCCESS)
    {
        int16_t pw = png.getWidth();
        int16_t ph = png.getHeight();
        xOffset = (OLED_WIDTH - pw) / 2;
        yOffset = (OLED_HEIGHT - ph) / 2;
        rc = png.decode(NULL, 0);

        // Serial.printf("Draw offset: (%d, %d), time used: %lu\r\n", xOffset, yOffset, millis() - start);
        // Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\r\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
        png.close();
        // delay(5000);
    }
    else
    {
        gfx[oled_index]->setFont(u8g_font_5x7);
        gfx[oled_index]->setCursor(5, 10);
        gfx[oled_index]->printf("Failed to open %s\n", path);
    }
}

static int jpegDrawCallback1(JPEGDRAW *pDraw)
{
    // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
    gfx1->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
    return 1;
}
static int jpegDrawCallback2(JPEGDRAW *pDraw)
{
    // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
    gfx2->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
    return 1;
}
static int jpegDrawCallback3(JPEGDRAW *pDraw)
{
    // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
    gfx3->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
    return 1;
}

void DrawJPEG(int16_t x, int16_t y, const char *path, uint8_t oled_index)
{
    JPEG_DRAW_CALLBACK *jpegDrawCallback = NULL;
    switch (oled_index)
    {
    case 0:
        jpegDrawCallback = jpegDrawCallback1;
        break;
    case 1:
        jpegDrawCallback = jpegDrawCallback2;
        break;
    case 2:
        jpegDrawCallback = jpegDrawCallback3;
        break;
    default:
        break;
    }
    // jpegDraw(path, jpegDrawCallback, true /* useBigEndian */,
    //          x /* x */, y /* y */, 128 /* widthLimit */, 128 /* heightLimit */);
    _x = x;
    _y = y;
    uint8_t widthLimit = OLED_WIDTH;
    uint8_t heightLimit = OLED_HEIGHT;
    _x_bound = _x + widthLimit - 1;
    _y_bound = _y + heightLimit - 1;
    int8_t rc;
    rc = _jpeg.open(path, jpegOpenFile, jpegCloseFile, jpegReadFile, jpegSeekFile, jpegDrawCallback);
    if (rc == 1)
    {
        // scale to fit height
        int _scale;
        int iMaxMCUs;
        float ratio = (float)_jpeg.getHeight() / heightLimit;
        if (ratio <= 1)
        {
            _scale = 0;
            iMaxMCUs = widthLimit / 16;
        }
        else if (ratio <= 2)
        {
            _scale = JPEG_SCALE_HALF;
            iMaxMCUs = widthLimit / 8;
        }
        else if (ratio <= 4)
        {
            _scale = JPEG_SCALE_QUARTER;
            iMaxMCUs = widthLimit / 4;
        }
        else
        {
            _scale = JPEG_SCALE_EIGHTH;
            iMaxMCUs = widthLimit / 2;
        }
        _jpeg.setMaxOutputSize(iMaxMCUs);

        _jpeg.setPixelType(RGB565_BIG_ENDIAN);

        _jpeg.decode(x, y, _scale);
        _jpeg.close();
    }
    else
    {
        gfx[oled_index]->setFont(u8g_font_5x7);
        gfx[oled_index]->setCursor(5, 10);
        gfx[oled_index]->printf("Failed to open %s\n", path);
    }
}

void DrawJPEGCentre(const char *path, uint8_t oled_index)
{
    JPEG_DRAW_CALLBACK *jpegDrawCallback = NULL;
    switch (oled_index)
    {
    case 0:
        jpegDrawCallback = jpegDrawCallback1;
        break;
    case 1:
        jpegDrawCallback = jpegDrawCallback2;
        break;
    case 2:
        jpegDrawCallback = jpegDrawCallback3;
        break;
    default:
        break;
    }
    uint8_t widthLimit = OLED_WIDTH;
    uint8_t heightLimit = OLED_HEIGHT;
    int8_t rc;
    rc = _jpeg.open(path, jpegOpenFile, jpegCloseFile, jpegReadFile, jpegSeekFile, jpegDrawCallback);
    if (rc == 1)
    {
        // scale to fit height
        int _scale;
        int iMaxMCUs;
        float ratio = (float)_jpeg.getHeight() / heightLimit;
        if (ratio <= 1)
        {
            _scale = 0;
            iMaxMCUs = widthLimit / 16;
        }
        else if (ratio <= 2)
        {
            _scale = JPEG_SCALE_HALF;
            iMaxMCUs = widthLimit / 8;
        }
        else if (ratio <= 4)
        {
            _scale = JPEG_SCALE_QUARTER;
            iMaxMCUs = widthLimit / 4;
        }
        else
        {
            _scale = JPEG_SCALE_EIGHTH;
            iMaxMCUs = widthLimit / 2;
        }
        _jpeg.setMaxOutputSize(iMaxMCUs);

        _jpeg.setPixelType(RGB565_BIG_ENDIAN);

        int pw = _jpeg.getWidth();
        int ph = _jpeg.getHeight();
        if (ph <= 128)
        {
            _x = (OLED_WIDTH - pw) / 2;
            _y = (OLED_HEIGHT - ph) / 2;
        }
        else
        {
            _x = (OLED_WIDTH - pw / _scale) / 2;
            _y = (OLED_HEIGHT - ph / _scale) / 2;
        }
        _x_bound = _x + widthLimit - 1;
        _y_bound = _y + heightLimit - 1;

        _jpeg.decode(_x, _y, _scale);
        _jpeg.close();
    }
    else
    {
        gfx[oled_index]->setFont(u8g_font_5x7);
        gfx[oled_index]->setCursor(5, 10);
        gfx[oled_index]->printf("Failed to open %s\n", path);
    }
}