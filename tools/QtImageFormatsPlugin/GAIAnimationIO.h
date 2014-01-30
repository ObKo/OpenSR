#ifndef GAIANIMATIONIO_H
#define GAIANIMATIONIO_H

#include <QImageIOHandler>
#include <QImage>
#include <QVector>

namespace Rangers
{
struct GAIHeader;

class GAIAnimationIO : public QImageIOHandler
{
public:
    GAIAnimationIO();
    ~GAIAnimationIO();

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
    void readTimes();

    mutable GAIHeader *m_header;
    QVector<int> m_times;
    int m_currentFrame;
    QImage m_prev;
};
}

#endif // GAIANIMATIONIO_H
