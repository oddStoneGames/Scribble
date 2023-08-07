#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = nullptr);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return m_Modified; }
    QColor penColor() const { return m_PenColor; }
    int penWidth() const { return m_PenWidth; }

public slots:
    void clearImage();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    bool m_Modified;
    bool m_Scribbling;
    QColor m_PenColor;
    int m_PenWidth;
    QImage m_Image;
    QPoint m_LastPoint;
};

#endif // SCRIBBLEAREA_H
