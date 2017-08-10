/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 - 2017 Kosyak <ObKo@mail.ru>

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

#include <OpenSR/QM/QuestPlayer.h>

#include <OpenSR/QM/Parser.h>

#include <QStringList>
#include <QDebug>
#include <QRegularExpression>
#include <QFile>
#include <cmath>

namespace OpenSR
{
namespace QM
{
struct QuestPlayer::QuestPlayerPrivate
{
public:
    QuestPlayerPrivate(QuestPlayer *q);

    void applyModifier(const QM::Modifier &m);
    bool checkCondition(const QM::Transition::Condition &c) const;
    void setLocation(uint32_t location);
    void checkTransitions();
    void reduceTransitions();
    bool checkCriticalParameters();

    QString substituteValues(const QString &str) const;

    QM::Quest m_quest;
    bool m_questLoaded;

    QM::Location m_currentLocation;
    QString m_locationText;
    QM::Transition m_currentTransition;

    QMap<uint32_t, int32_t> m_parametersVisibility;
    QMap<uint32_t, float> m_parameters;
    QMap<uint32_t, float> m_oldParameters;
    QMap<uint32_t, int32_t> m_transitionCounts;
    QMap<uint32_t, int32_t> m_locationDescriptionsCount;
    QList<uint32_t> m_possibleTransitions;
    QList<uint32_t> m_alwaysVisibleTransitions;

    QuestPlayer *q;
};

QuestPlayer::QuestPlayer(QObject *parent): QObject(parent),
    d_osr_ptr(new QuestPlayer::QuestPlayerPrivate(this))
{
}

QuestPlayer::QuestPlayerPrivate::QuestPlayerPrivate(QuestPlayer *q_)
{
    q = q_;
    m_questLoaded = false;
}

QuestPlayer::~QuestPlayer()
{

}

QM::Location QuestPlayer::currentLocation() const
{
    Q_D(const QuestPlayer);
    return d->m_currentLocation;
}

QM::Transition QuestPlayer::currentTransition() const
{
    Q_D(const QuestPlayer);
    return d->m_currentTransition;
}

QString QuestPlayer::QuestPlayerPrivate::substituteValues(const QString &str) const
{
    //TODO: Better tags handling (<format>, <fix>, etc)
    if (str.isEmpty())
        return str;

    QString result = str;
    QRegularExpression valueExp("\\[p(\\d)+\\]");
    QRegularExpression expExp("\\{(.+?)\\}");
    int deltaPos = 0;

    QRegularExpressionMatchIterator i = expExp.globalMatch(str);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString value = "<font class=\"selected\">" + QString::number((int32_t)round(QM::eval(match.captured(1), m_parameters))) + "</font>";
        result.replace(match.capturedStart() + deltaPos, match.capturedLength(), value);
        deltaPos += value.length() - match.capturedLength();
    }
    deltaPos = 0;
    QString str2 = result;
    i = valueExp.globalMatch(str2);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString value = "<font class=\"selected\">" + QString::number((int32_t)round(QM::eval(match.captured(0), m_parameters))) + "</font>";
        result.replace(match.capturedStart() + deltaPos, match.capturedLength(), value);
        deltaPos += value.length() - match.capturedLength();
    }

    result.replace("<ToStar>", "<font class=\"selected\">" + m_quest.toStar + "</font>");
    result.replace("<ToPlanet>", "<font class=\"selected\">" + m_quest.toPlanet + "</font>");
    result.replace("<FromStar>", "<font class=\"selected\">" + m_quest.fromStar + "</font>");
    result.replace("<FromPlanet>", "<font class=\"selected\">" + m_quest.fromPlanet + "</font>");
    result.replace("<Date>", "<font class=\"selected\">" + m_quest.date + "</font>");
    result.replace("<Money>", "<font class=\"selected\">" + m_quest.money + "</font>");
    result.replace("<Ranger>", "<font class=\"selected\">" + m_quest.ranger + "</font>");

    result.replace("<clr>", "<font class=\"selected\">");
    result.replace("<clrEnd>", "</font>");

    return result;
}

void QuestPlayer::loadQuest(QIODevice *dev)
{
    Q_D(QuestPlayer);

    d->m_questLoaded = false;

    if (!dev || !dev->isOpen())
        return;

    d->m_quest = QM::readQuest(dev);
    d->m_questLoaded = true;

    resetQuest();
}

void QuestPlayer::resetQuest()
{
    Q_D(QuestPlayer);

    d->m_parametersVisibility.clear();
    d->m_parameters.clear();
    d->m_transitionCounts.clear();
    d->m_locationDescriptionsCount.clear();

    if (!d->m_questLoaded)
        return;

    auto end = d->m_quest.parameters.end();
    for (auto i = d->m_quest.parameters.begin(); i != end; ++i)
    {
        QString start = i.value().start;
        float value = QM::eval(start, QMap<uint32_t, float>());
        d->m_parameters[i.key()] = value;
        d->m_parametersVisibility[i.key()] = true;
    }

    d->setLocation(d->m_quest.startLocation);
}

void QuestPlayer::QuestPlayerPrivate::setLocation(uint32_t location)
{
    auto loc = m_quest.locations.find(location);
    if (loc == m_quest.locations.end())
    {
        qWarning() << "Invalid location: " << location;
        return;
    }
    m_currentLocation = *loc;

    m_oldParameters = m_parameters;

    for (const QM::Modifier & m : m_currentLocation.modifiers)
    {
        applyModifier(m);
    }

    checkTransitions();
    reduceTransitions();

    if (!m_currentLocation.empty || m_currentTransition.description.isEmpty())
    {
        if (m_currentLocation.descriptionExpression && !m_currentLocation.expression.isEmpty())
        {
            int32_t t = (int32_t)QM::eval(m_currentLocation.expression, m_parameters);
            //FIXME: Bug or feature?
            if (t == 0)
                t = 1;
            if ((t < 1) || (t > 10) || (m_currentLocation.descriptions[t - 1].isEmpty()))
            {
                qCritical() << "Invalid location description selection (" << t << ") in location " << m_currentLocation.id;
                m_locationText = "";
            }
            else
                m_locationText = substituteValues(m_currentLocation.descriptions[t - 1]);
        }
        else
        {
            uint32_t value;
            auto dit = m_locationDescriptionsCount.find(m_currentLocation.id);
            if (dit == m_locationDescriptionsCount.end())
            {
                dit = m_locationDescriptionsCount.insert(m_currentLocation.id, 0);
                value = 0;
            }
            else
            {
                value = *dit;
                value = (value + 1) % 10;
            }

            for (int i = 0; (i < 10) && (m_currentLocation.descriptions[value].isEmpty()); i++)
                value = (value + 1) % 10;

            *dit = value;

            m_locationText = substituteValues(m_currentLocation.descriptions[value]);
        }
    }

    qDebug() << "QuestPlayer: L" << m_currentLocation.id;

    if ((m_possibleTransitions.count() == 1) && m_currentLocation.transitions[m_possibleTransitions.front()].title.isEmpty() &&
            m_currentTransition.description.isEmpty())
        q->startTransition(m_possibleTransitions.front());
    else if (m_currentLocation.type == QM::Location::LOCATION_SUCCESS)
        emit(q->questCompleted(m_locationText));
    else if (m_currentLocation.type == QM::Location::LOCATION_FAIL)
        emit(q->questFailed(m_locationText, m_currentLocation.death));
    else if (checkCriticalParameters())
        emit(q->locationChanged());
}

bool QuestPlayer::QuestPlayerPrivate::checkCriticalParameters()
{
    auto end = m_parameters.end();
    for (auto i = m_parameters.begin(); i != end; ++i)
    {
        const QM::Parameter &pr = m_quest.parameters.value(i.key());

        if (pr.type == QM::Parameter::PARAMETER_NORMAL)
            continue;

        bool crit = false;

        if (pr.minCritical && i.value() <= pr.min)
            crit = true;
        else if (!pr.minCritical && i.value() >= pr.max)
            crit = true;

        if (crit)
        {
            switch (pr.type)
            {
            case QM::Parameter::PARAMETER_DEATH:
                emit(q->questFailed(substituteValues(pr.critText), true));
                break;
            case QM::Parameter::PARAMETER_FAIL:
                emit(q->questFailed(substituteValues(pr.critText), false));
                break;
            case QM::Parameter::PARAMETER_SUCCESS:
                emit(q->questCompleted(substituteValues(pr.critText)));
                break;
            }
            return false;
        }
    }
    return true;
}

QString QuestPlayer::locationText() const
{
    Q_D(const QuestPlayer);
    return d->m_locationText;
}

QStringList QuestPlayer::visibleParameters() const
{
    Q_D(const QuestPlayer);
    QStringList params;
    auto end = d->m_parameters.end();
    for (auto i = d->m_parameters.begin(); i != end; ++i)
    {
        if ((i.value() != 0 || d->m_quest.parameters.value(i.key()).showOnZero) && d->m_parametersVisibility.value(i.key()))
        {
            QString value;
            for (const QM::Parameter::Range & r : d->m_quest.parameters.value(i.key()).ranges)
            {
                int32_t v = (int32_t)round(i.value());
                if ((v >= r.from) && (v <= r.to))
                {
                    value = r.text;
                    value.replace("<>", QString::number(v));
                    break;
                }
            }
            if (!value.isEmpty())
                params.append(d->substituteValues(value));
        }
    }
    return params;
}

bool QuestPlayer::QuestPlayerPrivate::checkCondition(const QM::Transition::Condition& c) const
{
    float param = m_parameters.value(c.param);

    if ((param < c.rangeFrom) || (param > c.rangeTo))
        return false;

    if (c.includeValues)
    {
        if ((c.values.count()) && (!c.values.contains(param)))
            return false;
    }
    else
    {
        if ((c.values.count()) && (c.values.contains(param)))
            return false;
    }

    if (c.includeMultiples)
    {
        for (int32_t m : c.multiples)
        {
            if ((fmod(param, m)) != 0)
                return false;
        }
    }
    else
    {
        for (int32_t m : c.multiples)
        {
            if ((fmod(param, m)) == 0)
                return false;
        }
    }
    return true;
}

void QuestPlayer::QuestPlayerPrivate::checkTransitions()
{
    m_possibleTransitions.clear();
    m_alwaysVisibleTransitions.clear();

    int i = 0;
    for (const QM::Transition & t : m_currentLocation.transitions)
    {
        bool cond = true;
        for (const QM::Transition::Condition & c : t.conditions)
        {
            cond = cond && checkCondition(c);
        }
        bool passed = (t.passCount == 0) || !((m_transitionCounts.find(t.id) != m_transitionCounts.end()) && (m_transitionCounts.value(t.id) >= t.passCount));
        if (passed && cond && (t.globalCondition.isEmpty() || QM::eval(t.globalCondition, m_parameters)))
        {
            m_possibleTransitions.append(i);
        }
        else if (t.alwaysVisible)
            m_alwaysVisibleTransitions.append(i);
        i++;
    }
}

QList<QuestPlayer::TransitionItem> QuestPlayer::visibleTransitions() const
{
    Q_D(const QuestPlayer);
    QList<TransitionItem> r;
    for (uint32_t t : d->m_possibleTransitions)
    {
        const QM::Transition &trans = d->m_currentLocation.transitions[t];

        TransitionItem item;
        item.id = t;
        item.enabled = true;
        item.title = d->substituteValues(trans.title);
        r.append(item);
    }

    for (uint32_t t : d->m_alwaysVisibleTransitions)
    {
        TransitionItem item;
        item.id = t;
        item.enabled = false;
        item.title = d->substituteValues(d->m_currentLocation.transitions[t].title);
        r.append(item);
    }

    qSort(r.begin(), r.end(),
          [&](const TransitionItem & a, const TransitionItem & b) -> bool
    {
        return d->m_currentLocation.transitions[a.id].position < d->m_currentLocation.transitions[b.id].position;
    });

    return r;
}

void QuestPlayer::QuestPlayerPrivate::applyModifier(const QM::Modifier& m)
{
    switch (m.visibility)
    {
    case QM::Modifier::VISIBILITY_HIDE:
        m_parametersVisibility[m.param] = false;
        break;
    case QM::Modifier::VISIBILITY_SHOW:
        m_parametersVisibility[m.param] = true;
        break;
    case QM::Modifier::VISIBILITY_NO_CHANGE:
        break;
    }

    float value;

    switch (m.operation)
    {
    case QM::Modifier::OPERATION_ASSIGN:
        value = m.value;
        break;
    case QM::Modifier::OPERATION_CHANGE:
        value = m_oldParameters[m.param] + m.value;
        break;
    case QM::Modifier::OPERATION_PERCENT:
        value = m_oldParameters[m.param] + (m.value * m_oldParameters[m.param]) / 100.0f;
        break;
    case QM::Modifier::OPERATION_EXPRESSION:
        if (!m.expression.isEmpty())
            value = QM::eval(m.expression, m_oldParameters);
        else
            value = m_oldParameters[m.param];
        break;
    }

    m_parameters[m.param] = std::min(value, (float)m_quest.parameters.value(m.param).max);
    m_parameters[m.param] = std::max(m_parameters[m.param], (float)m_quest.parameters.value(m.param).min);
}

void QuestPlayer::startTransition(uint32_t num)
{
    Q_D(QuestPlayer);

    if (d->m_currentLocation.transitions.size() <= num)
        return;

    d->m_currentTransition = d->m_currentLocation.transitions[num];

    d->m_oldParameters = d->m_parameters;

    for (const QM::Modifier & m : d->m_currentTransition.modifiers)
    {
        d->applyModifier(m);
    }

    qDebug() << "QuestPlayer: P" << d->m_currentTransition.id;

    d->m_locationText = d->substituteValues(d->m_currentTransition.description);

    if (d->m_quest.locations.value(d->m_currentTransition.to).empty)
    {
        finishTransition();
    }
    else if (d->m_currentTransition.description.isEmpty())
    {
        finishTransition();
    }
    else
    {
        emit(transitionText(d->substituteValues(d->m_currentTransition.description)));
    }
}

void QuestPlayer::finishTransition()
{
    Q_D(QuestPlayer);
    auto it = d->m_transitionCounts.find(d->m_currentTransition.id);
    if (it == d->m_transitionCounts.end())
    {
        it = d->m_transitionCounts.insert(d->m_currentTransition.id, 1);
    }
    else
    {
        (*it)++;
    }
    d->setLocation(d->m_currentTransition.to);
}

//FIXME: Optimize
void QuestPlayer::QuestPlayerPrivate::reduceTransitions()
{
    QMap<QString, QList<uint32_t> > transitions;
    QMap<QString, uint32_t> alwaysVisibleTransitions;

    for (uint32_t t : m_possibleTransitions)
    {
        const QM::Transition& tr = m_currentLocation.transitions[t];
        transitions[tr.title].append(t);
    }

    for (uint32_t t : m_alwaysVisibleTransitions)
    {
        const QM::Transition& tr = m_currentLocation.transitions[t];
        if (transitions.find(tr.title) == transitions.end())
            alwaysVisibleTransitions[tr.title] = t;
    }

    m_possibleTransitions.clear();
    m_alwaysVisibleTransitions.clear();

    auto end = alwaysVisibleTransitions.end();
    for (auto i = alwaysVisibleTransitions.begin(); i != end; ++i)
    {
        m_alwaysVisibleTransitions.append(i.value());
    }

    auto tend = transitions.end();
    for (auto i = transitions.begin(); i != tend; ++i)
    {
        if (i.value().count() == 1)
        {
            double pr = m_currentLocation.transitions.at(i.value().front()).priority;
            if (pr < 1.0)
            {
                if ((rand() % 1000) < pr * 1000)
                    m_possibleTransitions.append(i.value().front());
            }
            else
                m_possibleTransitions.append(i.value().front());
            continue;
        }
        double range = 0;

        for (uint32_t t : i.value())
        {
            range += m_currentLocation.transitions[t].priority;
        }

        uint32_t randRange = range * 1000;
        uint32_t r = rand() % randRange;
        uint32_t counter = 0;
        uint32_t selected = 0;

        for (uint32_t t : i.value())
        {
            counter += m_currentLocation.transitions[t].priority * 1000;
            selected = t;
            if (counter >= r)
                break;
        }
        m_possibleTransitions.append(selected);
    }
}
}
}
