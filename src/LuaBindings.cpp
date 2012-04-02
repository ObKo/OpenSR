/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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

#include "LuaBindings.h"
#include "libRanger.h"
#include "ResourceManager.h"
#include "Engine.h"
#include "Log.h"
#include "Font.h"
#include "Texture.h"
#include "AnimatedTexture.h"
#include "GAISprite.h"
#include "Object.h"
#include "ActionListener.h"
#include "Action.h"
#include "Widget.h"
#include "Button.h"
#include "LuaWidget.h"
#include "WidgetNode.h"
#include "CheckBox.h"
#include "ScrollArea.h"
#include "NinePatch.h"
#include <libRanger.h>
#include <fstream>
#include <string>
#include <cstring>
#include <cerrno>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/operator.hpp>

namespace Rangers
{

bool operator==(Object const& lhs, Object const& rhs)
{
    return &lhs == &rhs;
}

int execLuaScript(const std::wstring& name)
{
    size_t size;
    char *luaData = ResourceManager::instance()->loadData(name, size);
    if (!luaData)
        return -1;
    int state = execLuaScript(luaData, size, toLocal(name));
    delete[] luaData;
    return state;
}

int execLuaScript(const char *data, size_t size, const std::string& name)
{
    lua_State *luaState = initLuaState();
    int state;
    if (state = (luaL_loadbuffer(luaState, data, size, name.c_str()) || lua_pcall(luaState, 0, LUA_MULTRET, 0)))
        Log::error() << "Cannot exec lua script: " << lua_tostring(luaState, -1);
    lua_close(luaState);
    return state;
}

lua_State *initLuaState()
{
    using namespace luabind;
    lua_State *luaState = luaL_newstate();
    luaL_openlibs(luaState);

    luabind::open(luaState);

    luabind::module(luaState) [
        luabind::class_<std::wstring>("wstring"),

        namespace_("libRanger")[
            def("fromCodec", &fromCodec),
            def("fromUTF8", &fromUTF8),
            def("fromASCII", &fromASCII),
            def("fromLocal", &fromLocal),
            def("toCodec", (std::string(*)(const char*, const std::wstring&))&toCodec),
            def("toUTF8", &toUTF8),
            def("toASCII", &toASCII),
            def("toLocal", &toLocal),
            def("split", (std::vector<std::wstring>(*)(const std::wstring&, wchar_t))&split),
            def("suffix", (std::wstring(*)(const std::wstring&))&suffix),
            def("trim", (std::wstring(*)(const std::wstring&))&trim),
            def("basename", (std::wstring(*)(const std::wstring&))&basename),
            def("directory", (std::wstring(*)(const std::wstring&))&directory)
        ],

        def("L", &fromLua),
        def("luaDebug", &luaDebug),
        def("luaWarning", &luaWarning),
        def("luaError", &luaError),
        def("execLuaScript", (int (*)(const std::wstring&))&execLuaScript),

        luabind::class_<Vector>("Vector")
        .def(luabind::constructor<>())
        .def(luabind::constructor<float, float>())
        .def_readwrite("x", &Vector::x)
        .def_readwrite("y", &Vector::y),

        luabind::class_<Rect>("Rect")
        .def(luabind::constructor<>())
        .def(luabind::constructor<float, float, float, float>())
        .def_readwrite("x", &Rect::x)
        .def_readwrite("y", &Rect::y)
        .def_readwrite("height", &Rect::height)
        .def_readwrite("width", &Rect::width),

        luabind::class_<Font>("Font")
        .def(luabind::constructor<const std::wstring&, int>())
        .def("renderText", (boost::shared_ptr<Texture> (Font::*)(const std::wstring&, int) const)&Font::renderText)
        .def("renderColoredText", (boost::shared_ptr<Texture> (Font::* const)(const std::wstring&, int, int) const)&Font::renderColoredText)
        .def("size", &Font::size)
        .def("calculateStringWidth", &Font::calculateStringWidth),

        /*luabind::class_<Label, Sprite>("Label")
        .def(luabind::constructor<const std::wstring&, Object*, boost::shared_ptr<Font> >())
        .def(luabind::constructor<const std::wstring&, Object*>())
        .def(luabind::constructor<const std::wstring&>())
        .def("setText", (void (Label::*)(const std::wstring&))&Label::setText)
        .def("setFont", &Label::setFont)
        .def("text", &Label::text)
        .def("font", &Label::font)
        .def("setWordWrap", &Label::setWordWrap)
        .def("setFixedSize", &Label::setFixedSize),*/

        //TODO: overloading
        luabind::class_<Object>("Object")
        .def(const_self == const_self)
        .def("draw", &Object::draw)
        .def("processMain", &Object::processMain)
        .def("processLogic", &Object::processLogic)
        .def("setPosition", (void (Object::*)(float, float))&Object::setPosition)
        .def("setRotation", &Object::setRotation)
        .def("setColor", (void (Object::*)(float, float, float, float))&Object::setColor)
        .def("setColor", (void (Object::*)(int))&Object::setColor)
        .def("addChild", &Object::addChild)
        .def("removeChild", &Object::removeChild)
        .def("setParent", &Object::setParent)
        .def("position", &Object::position)
        .def("rotation", &Object::rotation)
        .def("layer", &Object::layer)
        .def("parent", &Object::parent)
        .def("setLayer", &Object::processLogic),

        luabind::class_<Texture>("Texture")
        .def("width", &Texture::width)
        .def("height", &Texture::height),

        luabind::class_<AnimatedTexture, Texture>("AnimatedTexture")
        .def("waitSeek", &AnimatedTexture::waitSeek)
        .def("waitSize", &AnimatedTexture::waitSize)
        .def("frameCount", &AnimatedTexture::frameCount),

        luabind::class_<Sprite, Object>("Sprite")
        .enum_("TextureScaling")
        [
            value("TEXTURE_NO", 0),
            value("TEXTURE_NORMAL", 1),
            value("TEXTURE_KEEPASPECT", 2),
            value("TEXTURE_KEEPASPECT_EXPANDING", 3),
            value("TEXTURE_TILE_X", 4),
            value("TEXTURE_TILE_Y", 5),
            value("TEXTURE_TILE", 6)
        ]
        .enum_("SpriteXOrigin")
        [
            value("POSITION_X_LEFT", 0),
            value("POSITION_X_RIGHT", 1),
            value("POSITION_X_CENTER", 2)
        ]
        .enum_("SpriteYOrigin")
        [
            value("POSITION_Y_TOP", 0),
            value("POSITION_Y_BOTTOM", 1),
            value("POSITION_Y_CENTER", 2)
        ]
        .def(luabind::constructor<boost::shared_ptr<Texture>, Object*, TextureScaling, SpriteXOrigin, SpriteYOrigin>())
        .def(luabind::constructor<boost::shared_ptr<Texture>, Object*>())
        .def(luabind::constructor<boost::shared_ptr<Texture> >())
        .def(luabind::constructor<std::wstring, Object*, TextureScaling, SpriteXOrigin, SpriteYOrigin>())
        .def(luabind::constructor<std::wstring, Object*>())
        .def(luabind::constructor<std::wstring>())
        .def(luabind::constructor<const TextureRegion&, Object*>())
        .def(luabind::constructor<const TextureRegion&>())
        .def("setOrigin", &Sprite::setOrigin)
        .def("setGeometry", &Sprite::setGeometry)
        .def("setTextureScaling", &Sprite::setTextureScaling)
        .def("setTexture", &Sprite::setTexture)
        .def("setWidth", &Sprite::setWidth)
        .def("setHeight", &Sprite::setHeight)
        .def("width", &Sprite::width)
        .def("height", &Sprite::height),

        luabind::class_<AnimatedSprite, Sprite>("AnimatedSprite")
        .def(luabind::constructor<boost::shared_ptr<AnimatedTexture>, Object*>())
        .def(luabind::constructor<boost::shared_ptr<AnimatedTexture> >())
        .def(luabind::constructor<std::wstring, Object*>())
        .def(luabind::constructor<std::wstring>())
        .def("setSingleShot", &AnimatedSprite::setSingleShot)
        .def("start", &AnimatedSprite::start)
        .def("stop", &AnimatedSprite::stop)
        .def("reset", &AnimatedSprite::reset)
        .def("isStarted", &AnimatedSprite::isStarted)
        .def("isSingleShot", &AnimatedSprite::isSingleShot)
        .def("currentFrame", &AnimatedSprite::currentFrame)
        .def("frameRate", &AnimatedSprite::frameRate)
        .def("setFrame", &AnimatedSprite::setFrame)
        .def("setFrameRate", &AnimatedSprite::setFrameRate),

        luabind::class_<GAISprite, AnimatedSprite>("GAISprite")
        .def(luabind::constructor<std::wstring, Object*>())
        .def(luabind::constructor<std::wstring>()),

        luabind::class_<Action>("Action")
        .enum_("Type")
        [
            value("BUTTON_CLICKED", 0),
            value("KEY_PRESSED", 1),
            value("CHECKBOX_TOGGLED", 2)
        ]
        .def("source", &Action::source)
        .def("type", &Action::type)
        .def("getStringArgument", &Action::getStringArgument)
        .def("getRectArgument", &Action::getRectArgument)
        .def("getKeyArgument", &Action::getKeyArgument)
        .def("getBoolArgument", &Action::getBoolArgument),


        luabind::class_<ActionListener>("ActionListener")
        .def("actionPerformed", &ActionListener::actionPerformed),

        luabind::class_<Widget, Object>("Widget")
        .def("getBoundingRect", &Widget::getBoundingRect)
        .def("mouseEnter", &Widget::mouseEnter)
        .def("mouseLeave", &Widget::mouseLeave)
        .def("mouseMove", (void (Widget::*)(float, float))&Widget::mouseMove)
        .def("mouseDown", (void (Widget::*)(uint8_t key, float, float))&Widget::mouseDown)
        .def("mouseUp", (void (Widget::*)(uint8_t key, float, float))&Widget::mouseUp)
        .def("mouseClick", (void (Widget::*)(float, float))&Widget::mouseClick)
        .def("width", &Widget::width)
        .def("height", &Widget::height)
        .def("minWidth", &Widget::minWidth)
        .def("minHeight", &Widget::minHeight)
        .def("maxWidth", &Widget::maxWidth)
        .def("maxHeight", &Widget::maxHeight)
        .def("preferredWidth", &Widget::preferredWidth)
        .def("preferredHeight", &Widget::preferredHeight)
        .def("setWidth", &Widget::setWidth)
        .def("setHeight", &Widget::setHeight)
        .def("setGeometry", &Widget::setGeometry)
        .def("addWidget", &Widget::addWidget)
        .def("removeWidget", &Widget::removeWidget)
        .def("addListener", &Widget::addListener)
        .def("removeListener", &Widget::removeListener),


        luabind::class_<Node, Object>("Node")
        .def(luabind::constructor<Widget*>())
        .def(luabind::constructor<>()),
        luabind::class_<WidgetNode, Widget>("WidgetNode")
        .def(luabind::constructor<Widget*>())
        .def(luabind::constructor<>()),

        luabind::class_<LuaWidget, Widget>("LuaWidget")
        .def(luabind::constructor<const std::wstring&, Widget*>())
        .def(luabind::constructor<const std::wstring&>())
        .def("dispose", &LuaWidget::dispose)
        .scope
        [
            class_<LuaWidget::LuaActionListener, ActionListener>("LuaActionListener")
        ],

        luabind::class_<Button, Widget>("Button")
        .def(luabind::constructor<boost::shared_ptr<Texture>, Widget*>())
        .def(luabind::constructor<boost::shared_ptr<Texture> >())
        .def(luabind::constructor<boost::shared_ptr<Texture>, boost::shared_ptr<Texture>, Widget*>())
        .def(luabind::constructor<boost::shared_ptr<Texture>, boost::shared_ptr<Texture> >())
        .def(luabind::constructor<boost::shared_ptr<Texture>, boost::shared_ptr<Texture>, boost::shared_ptr<Texture>, Widget*>())
        .def(luabind::constructor<boost::shared_ptr<Texture>, boost::shared_ptr<Texture>, boost::shared_ptr<Texture> >())
        .def(luabind::constructor<const std::wstring&, Widget*>())
        .def(luabind::constructor<const std::wstring&>())
        .def(luabind::constructor<const std::wstring&, const std::wstring&, Widget*>())
        .def(luabind::constructor<const std::wstring&, const std::wstring&>())
        .def(luabind::constructor<const std::wstring&, const std::wstring&, const std::wstring&, Widget*>())
        .def(luabind::constructor<const std::wstring&, const std::wstring&, const std::wstring&>())
        .def(luabind::constructor<const ButtonStyle&, Widget*>())
        .def(luabind::constructor<const ButtonStyle&>())
        .def("color", &Button::color)
        .def("text", &Button::text)
        .def("font", &Button::font)
        .def("setColor", &Button::setColor)
        .def("setText", &Button::setText)
        .def("setFont", &Button::setFont)
        .def("setAutoResize", &Button::setAutoResize)
        .def("autoResize", &Button::autoResize),

        luabind::class_<CheckBox, Widget>("CheckBox")
        .def(luabind::constructor<const CheckBoxStyle&, const std::wstring&, Rangers::Widget*>())
        .def(luabind::constructor<const CheckBoxStyle&, const std::wstring&>())
        .def("setChecked", &CheckBox::setChecked)
        .def("setText", &CheckBox::setText)
        .def("checked", &CheckBox::checked)
        .def("text", &CheckBox::text),

        luabind::class_<NinePatch, Sprite>("NinePatch")
        .def(luabind::constructor<Object*>())
        .def(luabind::constructor<const std::wstring&, Object*>())
        .def(luabind::constructor<>())
        .def(luabind::constructor<const std::wstring&>()),

        luabind::class_<ScrollArea, Widget>("ScrollArea")
        .def(luabind::constructor<const ScrollBarStyle&, WidgetNode*, Widget*>())
        .def(luabind::constructor<const ScrollBarStyle&, WidgetNode*>())
        .def(luabind::constructor<const ScrollBarStyle&>())
        .def("setNode", &ScrollArea::setNode),

        luabind::class_<ResourceDescriptor>("ResourceDescriptor"),

        luabind::class_<ButtonStyle>("ButtonStyle")
        .def_readwrite("normal", &ButtonStyle::normal)
        .def_readwrite("hovered", &ButtonStyle::hovered)
        .def_readwrite("pressed", &ButtonStyle::pressed)
        .def_readwrite("font", &ButtonStyle::font)
        .def_readwrite("color", &ButtonStyle::color)
        .def_readwrite("contentRect", &ButtonStyle::contentRect),

        luabind::class_<LineEditStyle>("LineEditStyle")
        .def_readwrite("background", &LineEditStyle::background)
        .def_readwrite("font", &LineEditStyle::font)
        .def_readwrite("color", &LineEditStyle::color)
        .def_readwrite("contentRect", &LineEditStyle::contentRect),

        luabind::class_<ScrollBarStyle>("ScrollBarStyle")
        .def_readwrite("upButton", &ScrollBarStyle::upButton)
        .def_readwrite("scroll", &ScrollBarStyle::scroll)
        .def_readwrite("downButton", &ScrollBarStyle::downButton),

        luabind::class_<CheckBoxStyle>("CheckBoxStyle")
        .def_readwrite("normal", &CheckBoxStyle::normal)
        .def_readwrite("checkedNormal", &CheckBoxStyle::checkedNormal)
        .def_readwrite("hovered", &CheckBoxStyle::hovered)
        .def_readwrite("checkedHovered", &CheckBoxStyle::checkedHovered),

        luabind::class_<Skin>("Skin")
        .def_readwrite("buttonStyle", &Skin::buttonStyle)
        .def_readwrite("lineEditStyle", &Skin::lineEditStyle)
        .def_readwrite("checkBoxStyle", &Skin::checkBoxStyle)
        .def_readwrite("scrollStyle", &Skin::scrollStyle),

        luabind::class_<Engine>("Engine")
        .def("quit", &Engine::quit)
        .def("markToUpdate", &Engine::markToUpdate)
        .def("unmarkToUpdate", &Engine::unmarkToUpdate)
        .def("addWidget", &Engine::addWidget, adopt(_2))
        .def("focusWidget", &Engine::focusWidget, adopt(_2))
        .def("removeWidget", &Engine::removeWidget, adopt(_2))
        .def("markWidgetDeleting", &Engine::markWidgetDeleting, adopt(_2))
        .def("coreFont", &Engine::coreFont)
        .def("serviceFont", &Engine::serviceFont)
        .def("screenHeight", &Engine::screenHeight)
        .def("screenWidth", &Engine::screenWidth)
        .def("rootNode", &Engine::rootNode)
        .def("defaultSkin", &Engine::defaultSkin)
        .def("setDefaultSkin", (void (Engine::*)(const std::wstring&))&Engine::setDefaultSkin),

        luabind::class_<ResourceManager>("ResourceManager")
        .def("addDir", &ResourceManager::addDir)
        .def("addMapping", &ResourceManager::addMapping)
        .def("loadTexture", &ResourceManager::loadTexture)
        .def("loadAnimation", (boost::shared_ptr<Rangers::AnimatedTexture> (ResourceManager::*)(const std::wstring&, bool))&ResourceManager::loadAnimation)
        .def("loadAnimation", (boost::shared_ptr<Rangers::AnimatedTexture> (ResourceManager::*)(const std::wstring&))&ResourceManager::loadAnimation)
        .def("loadFont", &ResourceManager::loadFont)
    ];

    luabind::globals(luaState)["engine"] = Engine::instance();
    luabind::globals(luaState)["resources"] = ResourceManager::instance();
    return luaState;
}

std::wstring fromLua(const char *s)
{
    return Rangers::fromUTF8(s);
}

/*Font* getPointer(boost::shared_ptr<Font> sp)
{
    return sp.get();
}

Texture* getPointer(boost::shared_ptr<Texture> sp)
{
    return sp.get();
}

AnimatedTexture* getPointer(boost::shared_ptr<AnimatedTexture> sp)
{
    return sp.get();
}

GAISprite *getPointer(boost::shared_ptr<GAISprite> sp)
{
    return sp.get();
}*/

void luaDebug(std::wstring s)
{
    Log::debug() << L"Lua: " << s;
}

void luaWarning(std::wstring s)
{
    Log::warning() << L"Lua: " << s;
}

void luaError(std::wstring s)
{
    Log::error() << L"Lua: " << s;
}

/*void freePointer(boost::shared_ptr<Font> *sp)
{
    delete sp;
}

void freePointer(boost::shared_ptr<Texture> *sp)
{
    delete sp;
}

void freePointer(boost::shared_ptr<AnimatedTexture> *sp)
{
    delete sp;
}*/
}
