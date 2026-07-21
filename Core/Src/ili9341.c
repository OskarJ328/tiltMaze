#include "ili9341.h"
#include "images.h"
#include "my_delay.h"
#include "stm32l4xx_hal_def.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_spi.h"
#include <stdint.h>
#include <stdbool.h>

#define MADCTL_MY   (1 << 7)
#define MADCTL_MX   (1 << 6)
#define MADCTL_MV   (1 << 5)
#define MADCTL_ML   (1 << 4)
#define MADCTL_BGR  (1 << 3)
#define MADCTL_MH   (1 << 2)

#define MSB(x)      ((uint8_t)(((x) >> 8) & 0xFF))
#define LSB(x)      ((uint8_t)((x) & 0xFF))      



typedef enum {
    softwareReset           = 0x01,
    sleepOut                = 0x11,
    powerControl1           = 0xC0,
    powerControl2           = 0xC1,
    VCOM_Control1           = 0xC5,
    VCOM_Control2           = 0xC7,
    memoryAccessControl     = 0x36,
    pixelFormatSet          = 0x3A,
    frameRateControl        = 0xB1,
    displayFunctionControl  = 0xB6,
    displayOFF              = 0x28,
    displayON               = 0x29,
    columnAddressSet        = 0x2A,
    pageAddressSet          = 0x2B,
    memoryWrite             = 0x2C,

    powerControlA           = 0xCB,
    powerControlB           = 0xCF,
    timingControlA          = 0xE8,
    timingControlB          = 0xEA,
    powerOnSequenceControl  = 0xED,
    pumpRatioControl        = 0xF7
}cmd_t;



static void ILI9341_select(ili9341_t *ili9341){
    HAL_GPIO_WritePin(ili9341->csPort, ili9341->csPin, GPIO_PIN_RESET);
}

static void ILI9341_unselect(ili9341_t *ili9341){
    HAL_GPIO_WritePin(ili9341->csPort, ili9341->csPin, GPIO_PIN_SET);
}

static void ILI9341_reset(ili9341_t *ili9341){
    HAL_GPIO_WritePin(ili9341->resPort, ili9341->resPin, GPIO_PIN_RESET);
    myDelay(5);
    HAL_GPIO_WritePin(ili9341->resPort, ili9341->resPin, GPIO_PIN_SET);
    myDelay(120);
}  

static void ILI9341_writeCommand(ili9341_t *ili9341,  cmd_t cmd){
    uint8_t command = (uint8_t)cmd;
    HAL_GPIO_WritePin(ili9341->dcPort, ili9341->dcPin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(ili9341->spiHandle, &command, sizeof(command), HAL_MAX_DELAY);
}

static void ILI9341_writeData(ili9341_t *ili9341, uint8_t *dataBuff, uint32_t dataSize){
    HAL_GPIO_WritePin(ili9341->dcPort, ili9341->dcPin, GPIO_PIN_SET);
    while(dataSize != 0){
        uint16_t chunkSize;
        if(dataSize > UINT16_MAX){
            chunkSize = UINT16_MAX; 
        }else{
            chunkSize = dataSize;
        }

        HAL_SPI_Transmit(ili9341->spiHandle, dataBuff, chunkSize, HAL_MAX_DELAY);

        dataBuff += chunkSize;
        dataSize -= chunkSize;
    }
}

static void ILI9341_setAddressWindow(ili9341_t *ili9341, uint16_t x0, uint16_t y0, uint16_t xn, uint16_t yn){

    uint8_t dataColumnAddressSet[4] = {MSB(x0), LSB(x0), MSB(xn), LSB(xn)};
    ILI9341_writeCommand(ili9341, columnAddressSet);
    ILI9341_writeData(ili9341, dataColumnAddressSet, sizeof(dataColumnAddressSet));

    uint8_t dataPageAddressSet[4] = {MSB(y0), LSB(y0), MSB(yn), LSB(yn)};
    ILI9341_writeCommand(ili9341, pageAddressSet);
    ILI9341_writeData(ili9341, dataPageAddressSet, sizeof(dataPageAddressSet));

    ILI9341_writeCommand(ili9341, memoryWrite);
}

static void ILI9341_updateMADCTL(ili9341_t *ili9341){
    uint8_t data = 0;
    if(ili9341->x_isAxisInverted){
        data |= MADCTL_MX;
    }
    if(ili9341->y_isAxisInverted){
        data |= MADCTL_MY;
    }
    if(ili9341->areAxesSwapped){
        data |= MADCTL_MV;
    }
    if(ili9341->colorFormat == BGR){
        data |= MADCTL_BGR;
    }
    ILI9341_select(ili9341);
    ILI9341_writeCommand(ili9341, memoryAccessControl);
    ILI9341_writeData(ili9341, &data, sizeof(data));
    ILI9341_unselect(ili9341);
}

void ILI9341_init(ili9341_t *ili9341){
    ILI9341_select(ili9341);
    ILI9341_reset(ili9341);
    ili9341->x_isAxisInverted = false;
    ili9341->y_isAxisInverted = false;
    ili9341->areAxesSwapped = false;

    ILI9341_writeCommand(ili9341, softwareReset);
    myDelay(1000);
    
    ILI9341_writeCommand(ili9341, powerControlA);
    uint8_t dataPowerControlA[5] = {0x39, 0x2C, 0x00, 0x34, 0x02};
    ILI9341_writeData(ili9341, dataPowerControlA, sizeof(dataPowerControlA));

    ILI9341_writeCommand(ili9341, powerControlB);
    uint8_t dataPowerControlB[3] = {0x00, 0xC1, 0x30};
    ILI9341_writeData(ili9341, dataPowerControlB, sizeof(dataPowerControlB));

    ILI9341_writeCommand(ili9341, timingControlA);
    uint8_t dataTimingControlA[3] = {0x85, 0x00, 0x78};
    ILI9341_writeData(ili9341, dataTimingControlA, sizeof(dataTimingControlA));

    ILI9341_writeCommand(ili9341, timingControlB);
    uint8_t dataTimingControlB[2] = {0x00, 0x00};
    ILI9341_writeData(ili9341, dataTimingControlB, sizeof(dataTimingControlB));

    ILI9341_writeCommand(ili9341, powerOnSequenceControl);
    uint8_t dataPowerOnSequenceControl[4] = {0x64, 0x03, 0x12, 0x81};
    ILI9341_writeData(ili9341, dataPowerOnSequenceControl, sizeof(dataPowerOnSequenceControl));

    ILI9341_writeCommand(ili9341, pumpRatioControl);
    uint8_t dataPumpRatioControl = 0x20;
    ILI9341_writeData(ili9341, &dataPumpRatioControl, sizeof(dataPumpRatioControl));

    ILI9341_writeCommand(ili9341, powerControl1);    
    uint8_t dataPowerControl1 = 0x23;
    ILI9341_writeData(ili9341, &dataPowerControl1, sizeof(dataPowerControl1));
    
    ILI9341_writeCommand(ili9341, powerControl2);
    uint8_t dataPowerControl2 = 0x10;
    ILI9341_writeData(ili9341, &dataPowerControl2, sizeof(dataPowerControl2));
    
    ILI9341_writeCommand(ili9341, VCOM_Control1);
    uint8_t dataVCOM_Control1[2] = {0x3E, 0x28};
    ILI9341_writeData(ili9341, dataVCOM_Control1, sizeof(dataVCOM_Control1));

    ILI9341_writeCommand(ili9341, VCOM_Control2);
    uint8_t dataVCOM_Control2 = 0x86;
    ILI9341_writeData(ili9341, &dataVCOM_Control2, sizeof(dataVCOM_Control2));

    ILI9341_writeCommand(ili9341, memoryAccessControl);
    uint8_t dataMemoryAccessControl = MADCTL_BGR;
    ILI9341_writeData(ili9341, &dataMemoryAccessControl, sizeof(dataMemoryAccessControl));

    ILI9341_writeCommand(ili9341, pixelFormatSet);
    uint8_t dataPixelFormatSet = 0x55;
    ILI9341_writeData(ili9341, &dataPixelFormatSet, sizeof(dataPixelFormatSet));

    ILI9341_writeCommand(ili9341, frameRateControl);
    uint8_t dataFrameRateControl[2] = {0x00, 0x18};
    ILI9341_writeData(ili9341, dataFrameRateControl, sizeof(dataFrameRateControl));
    
    ILI9341_writeCommand(ili9341, sleepOut);
    myDelay(100);

    ILI9341_writeCommand(ili9341, displayON);
    ILI9341_unselect(ili9341);
    
}

void ILI9341_drawPixel(ili9341_t *ili9341, uint16_t x, uint16_t y, uint16_t color){
    if(x >= ili9341->width || y >= ili9341->height){
        return;
    }

    ILI9341_select(ili9341);
    
    ILI9341_setAddressWindow(ili9341, x, y, x + 1, y + 1);
    
    
    uint8_t data[2] = {MSB(color), LSB(color)};
    ILI9341_writeData(ili9341, data, sizeof(data));

    ILI9341_unselect(ili9341);

}

void ILI9341_drawRectangle(ili9341_t *ili9341, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
    if(x > ili9341->width || y > ili9341->height){
        return;
    }
    if(x + w > ili9341->width || y + h > ili9341->height){
        return;
    }

    uint8_t data[256];
    uint32_t dataRemaining = w * h * 2;
    for(uint16_t i = 0; i < sizeof(data); i += 2){
        data[i]     = MSB(color);
        data[i+1]   = LSB(color); 
    }

    ILI9341_select(ili9341);
    ILI9341_setAddressWindow(ili9341, x, y, x + w - 1, y + h - 1);
    while (dataRemaining != 0){
        uint16_t chunkSize;
        if(dataRemaining < 200){
            chunkSize = dataRemaining;
        }
        else{
            chunkSize = 200;
        }

        ILI9341_writeData(ili9341, data, chunkSize);
        dataRemaining -= chunkSize;
    }
    ILI9341_unselect(ili9341);
}

void ILI9341_drawImage(ili9341_t *ili9341, uint16_t x, uint16_t y, image_t *image){
    if(x + image->Width > ili9341->width || y + image->Height > ili9341->height){
        return;
    }
    const uint16_t *imageBuffer = image->Buffer;
    uint16_t pixelsToSend;
    uint8_t chunk[1024];
    uint32_t pixelsRemaining = image->Width * image->Height;
    ILI9341_select(ili9341);
    ILI9341_setAddressWindow(ili9341, x, y, x + image->Width - 1, y + image->Height -1);
    while(pixelsRemaining != 0){
        if(pixelsRemaining > sizeof(chunk) / 2){
            pixelsToSend = sizeof(chunk) / 2;
        }else{
            pixelsToSend = pixelsRemaining;
        }

        for(uint16_t i = 0; i < pixelsToSend; i++){
            chunk[2 * i]        = MSB(imageBuffer[i]);
            chunk[2 * i + 1]    = LSB(imageBuffer[i]);
        }
        ILI9341_writeData(ili9341, chunk, pixelsToSend * 2);
        pixelsRemaining -= pixelsToSend;
        if (pixelsRemaining != 0){
            imageBuffer += pixelsToSend;
        }
    }
    ILI9341_unselect(ili9341);
}

void ILI9341_fillScreen(ili9341_t *ili9341, uint16_t color){
    ILI9341_drawRectangle(ili9341, 0, 0, ili9341->width, ili9341->height, color);
}

void ILI9341_invertAxis(ili9341_t *ili9341, axisToInvert_t axis){
    switch (axis) {
    case invertX:
        if(ili9341->areAxesSwapped){
            ili9341->y_isAxisInverted = !ili9341->y_isAxisInverted;
        }
        else{
            ili9341->x_isAxisInverted = !ili9341->x_isAxisInverted;
        }
        
        break;
    case invertY:
        if(ili9341->areAxesSwapped){
            ili9341->x_isAxisInverted = !ili9341->x_isAxisInverted;
        }
        else{
            ili9341->y_isAxisInverted = !ili9341->y_isAxisInverted;
        }
        break; 
    case invertBoth:
        ili9341->x_isAxisInverted = !ili9341->x_isAxisInverted;
        ili9341->y_isAxisInverted = !ili9341->y_isAxisInverted;
        break;
    default:
        break;
    }
    ILI9341_updateMADCTL(ili9341);
}

void ILI9341_swapAxes(ili9341_t *ili9341){
    bool tmpX = ili9341->x_isAxisInverted;
    bool tmpY = ili9341->y_isAxisInverted;
    if(ili9341->areAxesSwapped){
        ili9341->areAxesSwapped = false;
        ili9341->width  = ILI9341_WIDTH;
        ili9341->height = ILI9341_HEIGHT;
    }
    else{
        ili9341->areAxesSwapped = true;
        ili9341->width  = ILI9341_HEIGHT;
        ili9341->height = ILI9341_WIDTH;   
    }
    ili9341->x_isAxisInverted = tmpY;
    ili9341->y_isAxisInverted = tmpX;
    ILI9341_updateMADCTL(ili9341);
}

void ILI9341_setRotation(ili9341_t *ili9341, ili9341_rotation_t rotation){
    uint8_t data;
    switch(ili9341->colorFormat){
    case RGB:
        data = 0;
        break;
    case BGR:
        data = MADCTL_BGR;
        break;
    default:
        data = 0;
        break;
    }

    switch(rotation){
    case defaultOrientation:
        ili9341->width = ILI9341_WIDTH;
        ili9341->height = ILI9341_HEIGHT;
        break;
    case ninetyDeg:
        data |= MADCTL_MX | MADCTL_MV;
        ili9341->width = ILI9341_HEIGHT;
        ili9341->height = ILI9341_WIDTH;
        break;
    case minusNinetyDeg:
        data |= MADCTL_MX | MADCTL_MY | MADCTL_MV;
        ili9341->width = ILI9341_HEIGHT;
        ili9341->height = ILI9341_WIDTH;
        break;
    case oneHundredEightyDeg:
        data |= MADCTL_MY | MADCTL_MX;
        ili9341->width = ILI9341_WIDTH;
        ili9341->height = ILI9341_HEIGHT;
        break;
    default:
        break;
    }
    ILI9341_select(ili9341);
    ILI9341_writeCommand(ili9341, memoryAccessControl);
    ILI9341_writeData(ili9341, &data, sizeof(data));
    ILI9341_unselect(ili9341);
}

void ILI9341_writeChar(ili9341_t *ili9341, uint16_t x0, uint16_t y0, font_t *font, char ch, uint16_t color, uint16_t bgColor){
    if(x0 + font->width >= ili9341->width || y0 + font->height >= ili9341->height){
        return;
    }
    if((ch != 10 && ch < 32) || ch > 126){
        return;
    }
    
    uint8_t data[font->width * 2];
    ILI9341_select(ili9341);
    ILI9341_setAddressWindow(ili9341, x0, y0, x0 + font->width - 1, y0 + font->height - 1);
    for(uint16_t y = 0; y < font->height; y++){
        uint32_t charRow = font->data[(ch - 32) * font->height + y]; 
        for(uint16_t x = 0; x < font->width; x++){
            if(1 << (16 - x) & charRow){
                data[2 * x]     = MSB(color);
                data[2 * x + 1] = LSB(color);
            }
            else{
                data[2 * x]     = MSB(bgColor);
                data[2 * x + 1] = LSB(bgColor);
            }
        }
        ILI9341_writeData(ili9341, data, sizeof(data));
    }
    ILI9341_unselect(ili9341);
}

void ILI9341_writeString(ili9341_t *ili9341, uint16_t x0, uint16_t y0, font_t *font, char *string, uint16_t stringSize, uint16_t color, uint16_t bgColor){
    uint16_t x_pos = x0;
    uint16_t y_pos = y0;
    for(uint16_t ch = 0; ch < stringSize; ch++){
        if(string[ch] == '\n'){
            y_pos += font->height + font->lineSpacing;
            x_pos = x0;
        }
        else{
            ILI9341_writeChar(ili9341, x_pos, y_pos, font, string[ch], color, bgColor);
            x_pos += font->width + font->charSpacing;
        }
    }
}