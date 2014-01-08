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

#include <OpenSR/ScrollArea.h>
#include <OpenSR/Engine.h>
#include <OpenSR/ColorLabel.h>
#include <OpenSR/WidgetNode.h>
#include <OpenSR/LabelWidget.h>
#include <OpenSR/ActionListener.h>
#include <OpenSR/Action.h>

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
            w->label()->setColor(Color(0.0f, 0.7f, 0.7f));
        }
        else if (action.type() == Action::MOUSE_LEAVE)
        {
            w->label()->setColor(Color(1.0f, 1.0f, 1.0f));
        }
        else if (action.type() == Action::MOUSE_DOWN)
        {
            w->label()->setColor(Color(0.0f, 0.0f, 0.7f));
        }
        else if (action.type() == Action::MOUSE_UP)
        {
            w->label()->setColor(Color(0.0f, 0.7f, 0.7f));
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

InteractionWidget::InteractionWidget(const Rect& textRect, const Rect& selectionRect)
{
    m_actionListener = boost::shared_ptr<InteractionWidgetListener>(new InteractionWidgetListener(this));

    m_textRect = textRect;
    m_selectionRect = selectionRect;
    m_normalBoundingRect = textRect + selectionRect;
    setWidth(m_normalBoundingRect.x + m_normalBoundingRect.width);
    setHeight(m_normalBoundingRect.y + m_normalBoundingRect.height);

    boost::shared_ptr<WidgetNode> textNode = boost::shared_ptr<WidgetNode>(new WidgetNode());
    boost::shared_ptr<WidgetNode> selectionNode = boost::shared_ptr<WidgetNode>(new WidgetNode());

    m_textArea = boost::shared_ptr<ScrollArea>(new ScrollArea(Engine::instance().defaultSkin().scrollStyle, textNode));
    m_textArea->setGeometry(textRect.width, textRect.height);
    m_textArea->setPosition(textRect.x, textRect.y);

    m_selectionArea = boost::shared_ptr<ScrollArea>(new ScrollArea(Engine::instance().defaultSkin().scrollStyle, selectionNode));
    m_selectionArea->setGeometry(selectionRect.width, selectionRect.height);
    m_selectionArea->setPosition(selectionRect.x, selectionRect.y);

    boost::shared_ptr<ColorLabel> textLabel = boost::shared_ptr<ColorLabel>(new ColorLabel("", Engine::instance().coreFont()));
    textLabel->setWordWrap(true);
    textLabel->setFixedSize(m_textRect.width, -1);

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
}

std::vector< std::tuple< std::wstring, bool > > InteractionWidget::selections() const
{
    return m_selections;
}

void InteractionWidget::setSelections(const std::vector< std::tuple< std::wstring, bool > >& selections)
{
    m_selections = selections;

    for (boost::shared_ptr<LabelWidget> w : m_selectionsWidget)
    {
        m_selectionArea->node()->removeChild(w);
        w->removeListener(m_actionListener);
    }
    m_selectionsWidget.clear();

    for (const std::tuple<std::wstring, bool>& s : m_selections)
    {
        boost::shared_ptr<Label> l = boost::shared_ptr<Label>(new ColorLabel(std::get<0>(s)));
        boost::shared_ptr<LabelWidget> w = boost::shared_ptr<LabelWidget>(new LabelWidget(l));
        l->setWordWrap(true);
        w->setVertAlign(LabelWidget::VALIGN_CENTER);
        if (std::get<1>(s))
            w->addListener(m_actionListener);
        else
            w->label()->setColor(Color(0.5f, 0.5f, 0.5f));
        m_selectionsWidget.push_back(w);
        m_selectionArea->node()->addWidget(w);
    }

    markToUpdate();
}

std::wstring InteractionWidget::text() const
{
    return m_text;
}

void InteractionWidget::setText(const std::wstring& text)
{
    m_text = text;
    m_textLabel->label()->setText(text);
    markToUpdate();

}

}
}
