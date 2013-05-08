/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_PYTHON_WRAPPERS_H 
#define RANGERS_PYTHON_WRAPPERS_H

#include <boost/python.hpp>

#define PYTHON_OVERRIDE_WRAP(func, ...) { \
        GILGuard g; \
        if (boost::python::override f = this->get_override(func)) \
        { \
            try \
            { \
                return f(__VA_ARGS__); \
            } \
            catch (const boost::python::error_already_set& e) \
            { \
                handlePythonError(); \
            } \
        } \
        }
        
#define PYTHON_OVERRIDE_WRAP_VOID(func, ...) { \
        GILGuard g; \
        if (boost::python::override f = this->get_override(func)) \
        { \
            try \
            { \
                f(__VA_ARGS__); \
                return; \
            } \
            catch (const boost::python::error_already_set& e) \
            { \
                handlePythonError(); \
            } \
        } \
        }

namespace Rangers
{
void handlePythonError();
class Object;
namespace Python
{
  
class GILGuard
{
public:
    inline GILGuard()
    {
        m_state = PyGILState_Ensure();
    }
    inline ~GILGuard()
    {
        PyGILState_Release(m_state);
    }
private:
    PyGILState_STATE m_state;
};

template<class D>
struct ObjectWrap_: D, boost::python::wrapper<D>
{   
    template<class T1>
    ObjectWrap_(T1 t1): D(t1)
    {
    }
    template<class T1, class T2>
    ObjectWrap_(T1 t1, T2 t2): D(t1, t2)
    {
    }
    template<class T1, class T2, class T3>
    ObjectWrap_(T1 t1, T2 t2, T3 t3): D(t1, t2, t3)
    {
    }
    template<class T1, class T2, class T3, class T4>
    ObjectWrap_(T1 t1, T2 t2, T3 t3, T4 t4): D(t1, t2, t3, t4)
    {
    }
    template<class T1, class T2, class T3, class T4, class T5>
    ObjectWrap_(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5): D(t1, t2, t3, t4, t5)
    {
    }
           
    void draw() const
    {
        PYTHON_OVERRIDE_WRAP_VOID("draw");
        D::draw();
    }
    void draw_() const
    {
        D::draw();
    }
    void processMain()
    {
        PYTHON_OVERRIDE_WRAP_VOID("processMain");
        D::processMain();
    }
    void processMain_()
    {
        D::processMain();
    }
    void processLogic(int dt)
    {
        PYTHON_OVERRIDE_WRAP_VOID("processLogic", dt);
        D::processLogic(dt);
    }
    void processLogic_(int dt)
    {
        D::processLogic(dt);
    }
    void setColor(uint32_t color)
    {
        PYTHON_OVERRIDE_WRAP_VOID("setColor");
        D::setColor(color);
    }
    void setColor_(uint32_t color)
    {
        D::setColor(color);
    }
    uint32_t color() const
    {
        PYTHON_OVERRIDE_WRAP("color")
        return D::color();
    }
    uint32_t color_() const
    {
        return D::color();
    }
    
    template <class T1>
    static T1& defWrapped(T1& c)
    {
        return c
            .def("draw", &D::draw, &ObjectWrap_<D>::draw_)
            .def("processMain", &D::processMain, &ObjectWrap_<D>::processMain_)
            .def("processLogic", &D::processLogic, &ObjectWrap_<D>::processLogic_)
            .def("setColor", (void (D::*)(uint32_t))&D::setColor, &ObjectWrap_<D>::setColor_);
    }
};

template<class D>
struct SpriteWrap_: ObjectWrap_<D>
{
    template<class T1>
    SpriteWrap_(T1 t1): ObjectWrap_<D>(t1)
    {
    }
    template<class T1, class T2>
    SpriteWrap_(T1 t1, T2 t2): ObjectWrap_<D>(t1, t2)
    {
    }
    template<class T1, class T2, class T3>
    SpriteWrap_(T1 t1, T2 t2, T3 t3): ObjectWrap_<D>(t1, t2, t3)
    {
    }
    template<class T1, class T2, class T3, class T4>
    SpriteWrap_(T1 t1, T2 t2, T3 t3, T4 t4): ObjectWrap_<D>(t1, t2, t3, t4)
    {
    }
    template<class T1, class T2, class T3, class T4, class T5>
    SpriteWrap_(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5): ObjectWrap_<D>(t1, t2, t3, t4, t5)
    {
    }
    
    float normalHeight() const
    {
        PYTHON_OVERRIDE_WRAP("normalHeight");
        return D::normalHeight();
    }
    float normalHeight_() const
    {
        return D::normalHeight();
    }
    float normalWidth() const
    {
        PYTHON_OVERRIDE_WRAP("normalWidth");
        return D::normalWidth();
    }
    float normalWidth_() const
    {
        return D::normalWidth();
    }
    
    template <class T1>
    static T1& defWrapped(T1& c)
    {
        return ObjectWrap_<D>::defWrapped(c)
            .def("normalHeight", &D::normalHeight, &SpriteWrap_<D>::normalHeight_)
            .def("normalWidth", &D::normalWidth, &SpriteWrap_<D>::normalWidth_);
    }
};

template<class D>
struct WidgetWrap_: ObjectWrap_<D>
{
    template<class T1>
    WidgetWrap_(T1 t1): ObjectWrap_<D>(t1)
    {
    }
    template<class T1, class T2>
    WidgetWrap_(T1 t1, T2 t2): ObjectWrap_<D>(t1, t2)
    {
    }
    template<class T1, class T2, class T3>
    WidgetWrap_(T1 t1, T2 t2, T3 t3): ObjectWrap_<D>(t1, t2, t3)
    {
    }
    template<class T1, class T2, class T3, class T4>
    WidgetWrap_(T1 t1, T2 t2, T3 t3, T4 t4): ObjectWrap_<D>(t1, t2, t3, t4)
    {
    }
    template<class T1, class T2, class T3, class T4, class T5>
    WidgetWrap_(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5): ObjectWrap_<D>(t1, t2, t3, t4, t5)
    {
    }
    
    int minWidth() const
    {
        PYTHON_OVERRIDE_WRAP("minWidth");
        return D::minWidth();
    }
    int minWidth_() const
    {
        return D::minWidth();
    }
    int minHeight() const
    {
        PYTHON_OVERRIDE_WRAP("minHeight");
        return D::minHeight();
    }
    int minHeight_() const
    {
        return D::minHeight();
    }
    int maxWidth() const
    {
        PYTHON_OVERRIDE_WRAP("maxWidth");
        return D::maxWidth();
    }
    int maxWidth_() const
    {
        return D::maxWidth();
    }
    int maxHeight() const
    {
        PYTHON_OVERRIDE_WRAP("maxHeight");
        return D::maxHeight();
    }
    int maxHeight_() const
    {
        return D::maxHeight();
    }
    int preferredWidth() const
    {
        PYTHON_OVERRIDE_WRAP("preferredWidth");
        return D::preferredWidth();
    }
    int preferredWidth_() const
    {
        return D::preferredWidth();
    }
    int preferredHeight() const
    {
        PYTHON_OVERRIDE_WRAP("preferredHeight");
        return D::preferredHeight();
    }
    int preferredHeight_() const
    {
        return D::preferredHeight();
    }
    Rect getBoundingRect() const
    {
        PYTHON_OVERRIDE_WRAP("getBoundingRect");
        return D::getBoundingRect();
    }
    Rect getBoundingRect_() const
    {
        return D::getBoundingRect();
    }
    void mouseMove(const Vector &p) 
    {
        PYTHON_OVERRIDE_WRAP_VOID("mouseMove", p);
        D::mouseMove(p);
    }
    void mouseMove_(const Vector &p) 
    {
        D::mouseMove(p);
    }
    void focus()
    {
        PYTHON_OVERRIDE_WRAP_VOID("focus");
        D::focus();
    }
    void focus_()
    {
        D::focus();
    }
    void unFocus()
    {
        PYTHON_OVERRIDE_WRAP_VOID("unFocus");
        D::unFocus();
    }
    void unFocus_()
    {
        D::unFocus();
    }
        
    template <class T1>
    static T1& defWrapped(T1& c)
    {
        return ObjectWrap_<D>::defWrapped(c)
            .def("minHeight", &D::minHeight, &WidgetWrap_<D>::minHeight_)
            .def("minWidth", &D::minWidth, &WidgetWrap_<D>::minWidth_)
            .def("maxHeight", &D::maxHeight, &WidgetWrap_<D>::maxHeight_)
            .def("maxWidth", &D::maxWidth, &WidgetWrap_<D>::maxWidth_)
            .def("preferredHeight", &D::preferredHeight, &WidgetWrap_<D>::preferredHeight_)
            .def("preferredWidth", &D::preferredWidth, &WidgetWrap_<D>::preferredWidth_)
            .def("focus", &D::focus, &WidgetWrap_<D>::focus_)
            .def("unFocus", &D::unFocus, &WidgetWrap_<D>::unFocus_)
            .def("getBoundingRect", &D::getBoundingRect, &WidgetWrap_<D>::getBoundingRect_)
            .def("mouseMove", (void (D::*)(const Vector&))&D::mouseMove, &WidgetWrap_<D>::mouseMove_);
    }
};

}
}

#endif