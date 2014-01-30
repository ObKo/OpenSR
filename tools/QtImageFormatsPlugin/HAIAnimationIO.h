#ifndef HAIANIMATIONIO_H
#define HAIANIMATIONIO_H

#include <QImageIOHandler>

namespace Rangers
{
struct HAIHeader;
class HAIAnimationIO : public QImageIOHandler
{
public:
    HAIAnimationIO();
    ~HAIAnimationIO();

    virtual bool canRead() const;
    virtual bool read(QImage *image);

    virtual bool supportsOption(ImageOption option) const;
    virtual QVariant option(ImageOption option) const;

    int imageCount() const;
    int loopCount() const;
    int nextImageDelay() const;
    int currentImageNumber() const;

private:
    void peekHeader() const;

    mutable HAIHeader *m_header;
    int m_currentFrame;
};
}

#endif // HAIANIMATIONIO_H
