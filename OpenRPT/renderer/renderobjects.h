/*
 * Copyright (c) 2002-2007 by OpenMFG, LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * If you do not wish to be bound by the terms of the GNU General Public
 * License, DO NOT USE THIS SOFTWARE.  Please contact OpenMFG, LLC at
 * info@openmfg.com for details on how to purchase a commercial license.
 */
#ifndef __RENDEROBJECTS_H__
#define __RENDEROBJECTS_H__

#include <QString>
#include <QList>
#include <QPointF>
#include <QSizeF>
#include <QFont>
#include <QImage>
#include <QPen>
#include <QBrush>

#include <reportpageoptions.h>

class ORODocument;
class OROPage;
class OROPrimitive;
class OROTextBox;
class OROLine;
class OROImage;

//
// ORODocument
// This object is a single document containing one or more OROPage elements
//
class ORODocument
{
  friend class OROPage;

  public:
    ORODocument(const QString & = QString());
    virtual ~ORODocument();

    QString title() const { return _title; };
    void setTitle(const QString &);

    int pages() const { return _pages.count(); };
    OROPage* page(int);
    void addPage(OROPage*);

    void setPageOptions(const ReportPageOptions &);
    ReportPageOptions pageOptions() const { return _pageOptions; };

  protected:
    QString _title;
    QList<OROPage*> _pages;
    ReportPageOptions _pageOptions;
};

//
// OROPage
// This object is a single page in a document and may contain zero or more
// OROPrimitive objects all of which represent some form of mark to made on
// a page.
//
class OROPage
{
  friend class ORODocument;
  friend class OROPrimitive;

  public:
    OROPage(ORODocument * = 0);
    virtual ~OROPage();

    ORODocument* document() const { return _document; };
    int page() const; // returns this pages current page number

    int primitives() const { return _primitives.count(); };
    OROPrimitive* primitive(int);
    void addPrimitive(OROPrimitive*);

    void setWatermarkText(const QString &);
    void setWatermarkFont(const QFont &);
    void setWatermarkOpacity(unsigned char); // 0..255 : default 25

    QString watermarkText() const { return _wmText; };
    QFont watermarkFont() const { return _wmFont; };
    unsigned char watermarkOpacity() const { return _wmOpacity; };

    void setBackgroundImage(const QImage &);
    void setBackgroundPosition(const QPointF &);
    void setBackgroundSize(const QSizeF &);
    void setBackgroundScale(bool);
    void setBackgroundScaleMode(Qt::AspectRatioMode);
    void setBackgroundAlign(int);
    void setBackgroundOpacity(unsigned char);

    QImage backgroundImage() const { return _bgImage; };
    QPointF backgroundPosition() const { return _bgPos; };
    QSizeF backgroundSize() const { return _bgSize; };
    bool backgroundScale() const { return _bgScale; };
    Qt::AspectRatioMode backgroundScaleMode() const { return _bgScaleMode; };
    int backgroundAlign() const { return _bgAlign; };
    unsigned char backgroundOpacity() const { return _bgOpacity; };

  protected:
    ORODocument * _document;
    QList<OROPrimitive*> _primitives;

    QString _wmText;
    QFont _wmFont;
    unsigned char _wmOpacity;

    QImage _bgImage;
    QPointF _bgPos;
    QSizeF _bgSize;
    bool _bgScale;
    Qt::AspectRatioMode _bgScaleMode;
    int _bgAlign;
    unsigned char _bgOpacity;
};

//
// OROPrimitive
// This object represents the basic primitive with a position and type.
// Other primitives are subclasses with a defined type and any additional
// information they require to define that primitive.
//
class OROPrimitive
{
  friend class OROPage;

  public:
    virtual ~OROPrimitive();

    // Returns the type of the primitive which should be
    // set by the subclass
    int type() const { return _type; };
    OROPage * page() const { return _page; };

    QPointF position() const { return _position; };
    void setPosition(const QPointF &);

  protected:
    OROPrimitive(int);

    OROPage * _page;
    int _type;
    QPointF _position;
};

//
// OROTextBox
// This is a text box primitive it defines a box region and text that will
// be rendered inside that region. It also contains information for font
// and positioning of the text.
//
class OROTextBox : public OROPrimitive
{
  public:
    OROTextBox();
    virtual ~OROTextBox();

    QSizeF size() const { return _size; };
    void setSize(const QSizeF &);

    QString text() const { return _text; };
    void setText(const QString &);

    QFont font() const { return _font; };
    void setFont(const QFont &);

    int flags() const { return _flags; };
    void setFlags(int);

    static const int TextBox;

  protected:
    QSizeF _size;
    QString _text;
    QFont _font;
    int _flags; // Qt::AlignmentFlag and Qt::TextFlag OR'd
};

//
// OROLine
// This primitive defines a line with a width/weight.
//
class OROLine : public OROPrimitive
{
  public:
    OROLine();
    virtual ~OROLine();

    QPointF startPoint() const { return position(); };
    void setStartPoint(const QPointF &);

    QPointF endPoint() const { return _endPoint; };
    void setEndPoint(const QPointF &);

    qreal weight() const { return _weight; };
    void setWeight(qreal);

    static const int Line;

  protected:
    QPointF _endPoint;
    qreal _weight;
};

//
// OROImage
// This primitive defines an image
//
class OROImage: public OROPrimitive
{
  public:
    OROImage();
    virtual ~OROImage();

    QImage image() const { return _image; };
    void setImage(const QImage &);

    QSizeF size() const { return _size; };
    void setSize(const QSizeF &);

    bool scaled() const { return _scaled; };
    void setScaled(bool);

    int transformationMode() const { return _transformFlags; };
    void setTransformationMode(int);

    int aspectRatioMode() const { return _aspectFlags; };
    void setAspectRatioMode(int);

    static const int Image;

  protected:
    QImage _image;
    QSizeF _size;
    bool _scaled;
    int _transformFlags;
    int _aspectFlags;
};

//
// ORORect
// This primitive defines a drawn rectangle
//
class ORORect: public OROPrimitive
{
  public:
    ORORect();
    virtual ~ORORect();

    QSizeF size() const { return _size; }
    void setSize(const QSizeF &);

    QRectF rect() const { return QRectF(_position, _size); };
    void setRect(const QRectF &);

    QPen pen() const { return _pen; };
    void setPen(const QPen &);

    QBrush brush() const { return _brush; };
    void setBrush(const QBrush &);

    static const int Rect;

  protected:
    QSizeF _size;
    QPen _pen;
    QBrush _brush;
};


#endif // __RENDEROBJECTS_H__
