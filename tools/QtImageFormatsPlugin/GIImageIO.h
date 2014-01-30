#ifndef GIIMAGEIO_H
#define GIIMAGEIO_H

#include <QImageIOHandler>

namespace Rangers
{
struct GIFrameHeader;
class GIImageIO : public QImageIOHandler
{
public:
    GIImageIO();
    virtual ~GIImageIO();

    virtual bool canRead() const;
    virtual bool read(QImage *image);

    virtual bool supportsOption(ImageOption option) const;
    virtual QVariant option(ImageOption option) const;

private:
    void peekHeader() const;
    mutable GIFrameHeader *m_header;
};
}

#endif // GIIMAGEIO_H
