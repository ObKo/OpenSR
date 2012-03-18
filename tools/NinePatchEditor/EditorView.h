#ifndef QPE_EDITORVIEW_H
#define QPE_EDITORVIEW_H

#include <QGraphicsView>

namespace QNPE
{
class EditorView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit EditorView(QWidget *parent = 0);

    void wheelEvent(QWheelEvent *event);

    void setZoom(qreal zoom);
    qreal zoom() const;

protected:
    virtual void contextMenuEvent(QContextMenuEvent * event);

private:
    qreal m_zoom;
};
}

#endif // QPE_EDITORVIEW_H
