#include "Types.h"
#include <QIODevice>
#include <QImage>

namespace Rangers
{
extern void blitRGBAI(QImage *image, int x, int y, int w, int h, char *graphbuf, char *pal);

bool loadHAIFrame(QIODevice *device, QImage *image, HAIHeader *header, int num)
{
    if (!device || !header)
        return false;

    uint8_t *frameData = new uint8_t[header->frameSize];

    device->seek(sizeof(HAIHeader) + header->frameSize * num);
    device->read((char*)frameData, header->frameSize);

    *image = QImage(header->width, header->height, QImage::Format_ARGB32);
    blitRGBAI(image, 0, 0, header->width, header->height, (char*)frameData, (char*)(frameData + header->rowBytes * header->height));
    delete[] frameData;

    return true;
}
}
