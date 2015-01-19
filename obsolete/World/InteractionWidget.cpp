/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "InteractionWidget.h"
#include "SkinManager.h"

#include <OpenSR/ScrollArea.h>
#include <OpenSR/Engine.h>
#include <OpenSR/ColorLabel.h>
#include <OpenSR/WidgetNode.h>
#include <OpenSR/LabelWidget.h>
#include <OpenSR/ActionListener.h>
#include <OpenSR/Action.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Sprite.h>
#include <OpenSR/NinePatch.h>

namespace
{
const float SELECTION_INDENT = 5.0f;
}

//TODO: Style for InteractionWidget
namespace Rangers
{
namespace World
{
class InteractionWidget::InteractionWidgetListener: public ActionListener
{
public:
    InteractionWidgetListener(InteractionWidget *parent): ActionListener(), m_parent(parent)
    {
    }

    virtual void actionPerformed(const Action &action)
    {
        boost::shared_ptr<LabelWidget> w = boost::dynamic_pointer_cast<LabelWidget>(action.source());

        if (!w)
            return;


        if (action.type() == Action::MOUSE_ENTER)
        {
            boost::dynamic_pointer_cast<ColorLabel>(w->label())->setTextColor(m_parent->m_style->activeColor);
        }
        else if (action.type() == Action::MOUSE_LEAVE)
        {
            boost::dynamic_pointer_cast<ColorLabel>(w->label())->setTextColor(m_parent->m_style->color);
        }
        else if (action.type() == Action::MOUSE_CLICK)
        {
            int id = std::find(m_parent->m_selectionsWidget.begin(), m_parent->m_selectionsWidget.end(), w) - m_parent->m_selectionsWidget.begin();
            boost::shared_ptr<Widget> parent;

            if (m_parent->parent())
                parent = boost::dynamic_pointer_cast<Widget>(m_parent->parent()->getChild(m_parent));

            m_parent->action(Action(parent, Action::USER + id));
        }
    }

private:
    InteractionWidget *m_parent;
};

InteractionWidget::InteractionWidget(): Widget()
{
    m_actionListener = boost::shared_ptr<InteractionWidgetListener>(new InteractionWidgetListener(this));
}

InteractionWidget::InteractionWidget(const Rect& textRect, const Rect& selectionRect, boost::shared_ptr<InteractionWidgetStyle> style)
{
    m_actionListener = boost::shared_ptr<InteractionWidgetListener>(new InteractionWidgetListener(this));
    m_style = style;

    if (!m_style)
    {
        m_style = boost::shared_ptr<InteractionWidgetStyle>(new InteractionWidgetStyle);
        m_style->color = Color(0.0f, 0.0f, 0.0f);
        m_style->activeColor = Color(0.0f, 0.0f, 1.0f);
    }
    if (!m_style->scrollStyle)
        m_style->scrollStyle = Engine::instance().defaultSkin()->scrollStyle;

    boost::shared_ptr<TextureRegionDescriptor> texture;
    boost::shared_ptr<NinePatchDescriptor> ninepatch;
    if (m_style)
    {
        texture = boost::dynamic_pointer_cast<TextureRegionDescriptor>(m_style->background);
        ninepatch = boost::dynamic_pointer_cast<NinePatchDescriptor>(m_style->background);
    }
    if (ninepatch)
    {
        m_background = boost::shared_ptr<Sprite>(new NinePatch(ninepatch));
        addChild(m_background);
    }
    else if (texture)
    {
        m_background = boost::shared_ptr<Sprite>(new Sprite(texture));
        addChild(m_background);
    }

    m_textRect = textRect;
    m_selectionRect = selectionRect;
    m_normalBoundingRect = textRect + selectionRect;
    if (m_background)
    {
        Rect bgRect;
        bgRect.x = 0;
        bgRect.y = 0;
        bgRect.width = m_background->width();
        bgRect.height = m_background->height();
        m_normalBoundingRect = m_normalBoundingRect + bgRect;
    }
    setWidth(m_normalBoundingRect.x + m_normalBoundingRect.width);
    setHeight(m_normalBoundingRect.y + m_normalBoundingRect.height);

    boost::shared_ptr<WidgetNode> textNode = boost::shared_ptr<WidgetNode>(new WidgetNode());
    boost::shared_ptr<WidgetNode> selectionNode = boost::shared_ptr<WidgetNode>(new WidgetNode());

    m_textArea = boost::shared_ptr<ScrollArea>(new ScrollArea(m_style->scrollStyle, textNode));
    m_textArea->setGeometry(textRect.width, textRect.height);
    m_textArea->setPosition(textRect.x, textRect.y);

    m_selectionArea = boost::shared_ptr<ScrollArea>(new ScrollArea(m_style->scrollStyle, selectionNode));
    m_selectionArea->setGeometry(selectionRect.width, selectionRect.height);
    m_selectionArea->setPosition(selectionRect.x, selectionRect.y);

    boost::shared_ptr<Font> font;
    if ((m_style->font) && (m_style->font->path != ""))
        font = ResourceManager::instance().loadFont(m_style->font->path, m_style->font->size, m_style->font->antialiasing);
    else
        font = Engine::instance().coreFont();

    boost::shared_ptr<ColorLabel> textLabel = boost::shared_ptr<ColorLabel>(new ColorLabel("", font));
    textLabel->setWordWrap(true);
    textLabel->setFixedSize(m_textRect.width, -1);
    textLabel->setTextColor(m_style->color);
    textLabel->setSelectionColor(m_style->activeColor);

    m_textLabel = boost::shared_ptr<LabelWidget>(new LabelWidget(textLabel));
    textNode->addWidget(m_textLabel);

    addWidget(m_textArea);
    addWidget(m_selectionArea);
}

void InteractionWidget::draw() const
{
    if (!m_selectionArea || !m_textArea)
        return;

    if (!prepareDraw())
        return;

    if (m_background)
        m_background->draw();

    m_textArea->draw();
    m_selectionArea->draw();

    endDraw();
}

void InteractionWidget::processMain()
{
    Widget::processMain();

    float dx = width() - m_normalBoundingRect.x - m_normalBoundingRect.width;
    float dy = height() - m_normalBoundingRect.y - m_normalBoundingRect.height;

    m_textLabel->label()->setFixedWidth(m_textRect.width + dx / 2);
    m_textLabel->label()->processMain();

    m_textLabel->setGeometry(m_textRect.width + dx / 2, m_textLabel->label()->height());
    m_textArea->setGeometry(m_textRect.width + dx / 2, m_textRect.height + dy / 2);

    m_selectionArea->setGeometry(m_selectionRect.width + dx / 2, m_selectionRect.height + dy / 2);
    int selWidth = m_selectionRect.width + dx / 2;

    int y = 0;
    for (boost::shared_ptr<LabelWidget> w : m_selectionsWidget)
    {
        w->label()->setFixedWidth(selWidth);
        w->label()->processMain();
        w->setGeometry(w->label()->width(), w->label()->height() + SELECTION_INDENT);
        w->setPosition(0, y);
        y += w->label()->height() + SELECTION_INDENT;
    }

    // Resize rects
    if (m_selectionRect.x > m_textRect.x)
    {
        if (m_selectionRect.y > m_textRect.y)
            m_selectionArea->setPosition((int)(m_selectionRect.x + dx / 2), (int)(m_selectionRect.y + dy / 2));
        else
            m_selectionArea->setPosition((int)(m_selectionRect.x + dx / 2), (int)(m_selectionRect.y));
    }
    else
    {
        if (m_selectionRect.y > m_textRect.y)
            m_selectionArea->setPosition((int)(m_selectionRect.x), (int)(m_selectionRect.y + dy / 2));
        else
            m_selectionArea->setPosition((int)(m_selectionRect.x), (int)(m_selectionRect.y));
    }
    if (m_textRect.x > m_selectionRect.x)
    {
        if (m_textRect.y > m_selectionRect.y)
            m_textArea->setPosition((int)(m_textRect.x + dx / 2), (int)(m_textRect.y + dy / 2));
        else
            m_textArea->setPosition((int)(m_textRect.x + dx / 2), (int)(m_textRect.y));
    }
    else
    {
        if (m_textRect.y > m_selectionRect.y)
            m_textArea->setPosition((int)(m_textRect.x), (int)(m_textRect.y + dy / 2));
        else
            m_textArea->setPosition((int)(m_textRect.x), (int)(m_textRect.y));
    }
    if (m_background)
        m_background->setGeometry(width(), height());
}

std::vector< std::tuple< std::string, bool > > InteractionWidget::selections() const
{
    return m_selections;
}

void InteractionWidget::setSelections(const std::vector< std::tuple< std::string, bool > >& selections)
{
    m_selections = selections;

    for (boost::shared_ptr<LabelWidget> w : m_selectionsWidget)
    {
        m_selectionArea->node()->removeChild(w);
        w->removeListener(m_actionListener);
    }
    m_selectionsWidget.clear();

    for (const std::tuple<std::string, bool>& s : m_selections)
    {
        boost::shared_ptr<ColorLabel> l = boost::shared_ptr<ColorLabel>(new ColorLabel(std::get<0>(s)));
        boost::shared_ptr<LabelWidget> w = boost::shared_ptr<LabelWidget>(new LabelWidget(l));
        l->setWordWrap(true);
        l->setTextColor(m_style->color);
        l->setSelectionColor(m_style->activeColor);
        w->setVertAlign(LabelWidget::VALIGN_CENTER);
        if (std::get<1>(s))
            w->addListener(m_actionListener);
        else
            l->setTextColor(m_style->inactiveColor);
        m_selectionsWidget.push_back(w);
        m_selectionArea->node()->addWidget(w);
    }

    markToUpdate();
}

std::string InteractionWidget::text() const
{
    return m_text;
}

void InteractionWidget::setText(const std::string& text)
{
    m_text = text;
    m_textLabel->label()->setText(text);
    markToUpdate();
}

}
}
