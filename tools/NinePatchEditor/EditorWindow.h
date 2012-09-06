#ifndef QNPE_EDITORWINDOW_H
#define QNPE_EDITORWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui
{
class EditorWindow;
}

namespace QNPE
{
class PatchItem;
class NinePatchItem;
class BackgroundItem;
class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

    void restoreParemeters();
    void saveParameters();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::EditorWindow *m_ui;
    QGraphicsScene m_editorScene;
    QGraphicsScene m_previewScene;

    PatchItem *m_patch;
    NinePatchItem *m_previewItem;
    QGraphicsPixmapItem *m_pixmap;
    BackgroundItem *m_editorBackground;

private slots:
    void openFile();
    void saveFile();
    void updateScenes(const QList<QRectF> &region);
    void updatePreview(const QList<QRectF> &region);
};
}

#endif // QNPE_EDITORWINDOW_H
