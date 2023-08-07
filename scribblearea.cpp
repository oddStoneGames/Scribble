#include <QtWidgets>
#include "scribblearea.h"

ScribbleArea::ScribbleArea(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    m_Modified = false;
    m_Scribbling = false;
    m_PenWidth = 1;
    m_PenColor = QColor::fromRgb(255, 165, 0);
}

bool ScribbleArea::openImage(const QString &fileName)
{
    QImage loadedImage;
    if(!loadedImage.load(fileName)) return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    m_Image = loadedImage;
    m_Modified = false;
    update();
    return true;
}

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = m_Image;
    resizeImage(&visibleImage, size());
    if(visibleImage.save(fileName, fileFormat))
    {
        m_Modified = false;
        return true;
    }
    else
    {
        return false;
    }
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    m_PenColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    m_PenWidth = newWidth;
}

void ScribbleArea::clearImage()
{
    m_Image.fill(qRgb(255, 255, 255));
    m_Modified = true;
    update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_LastPoint = event->pos();
        m_Scribbling = true;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && m_Scribbling)
        drawLineTo(event->pos());
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && m_Scribbling)
    {
        drawLineTo(event->pos());
        m_Scribbling = false;
    }
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, m_Image, dirtyRect);
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if(width() > m_Image.width() || height() > m_Image.height())
    {
        int newWidth = qMax(width() + 128, m_Image.width());
        int newHeight = qMax(height() + 128, m_Image.height());
        resizeImage(&m_Image, QSize(newWidth, newHeight));
        update();
    }

    QWidget::resizeEvent(event);
}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&m_Image);
    painter.setPen(QPen(m_PenColor, m_PenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(m_LastPoint, endPoint);
    m_Modified = true;
    int rad = (m_PenWidth / 2) + 2;
    update(QRect(m_LastPoint, endPoint).normalized().adjusted(-rad, -rad, rad, rad));
    m_LastPoint = endPoint;
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if(image->size() == newSize) return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
