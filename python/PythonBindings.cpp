/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#include <boost/python.hpp>
#include <OpenSR/Log.h>
#include <OpenSR/PythonBindings.h>
#include <libRanger.h>

namespace Rangers
{
namespace Python
{
void exportEngine();
void exportObject();
void exportTypes();
void exportNode();
void exportSprite();
void exportWidget();
void exportScriptWidget();
void exportAnimatedSprite();
void exportLabel();
void exportNinePatch();
void exportButton();
void exportCheckBox();
void exportLineEdit();
void exportScrollArea();
void exportWidgetNode();
void exportGAISprite();
void exportColorLabel();
void exportAction();
void exportActionListener();
void exportFont();
void exportResourceManager();
void exportShader();
void exportShaderProgram();
void exportSound();
void exportSoundManager();
void exportStyles();
void exportTexture();
void exportRadioButton();
void exportRadioButtonGroup();

void pythonError(const std::wstring& text)
{
    if (text.empty())
        return;
    std::vector<std::wstring> lines = split(text, L'\n');
    std::vector<std::wstring>::const_iterator end = lines.end();
    for (std::vector<std::wstring>::const_iterator i = lines.begin(); i != end; ++i)
    {
        if (i->empty())
            continue;
        Log::error() << "[Python] " << (*i);
    }
}

void pythonOut(const std::wstring& text)
{
    if (text.empty())
        return;
    std::vector<std::wstring> lines = split(text, L'\n');
    std::vector<std::wstring>::const_iterator end = lines.end();
    for (std::vector<std::wstring>::const_iterator i = lines.begin(); i != end; ++i)
    {
        if (i->empty())
            continue;
        Log::info() << "[Python] " << (*i);
    }
}

}
}

BOOST_PYTHON_MODULE(_Engine)
{
    Rangers::Python::exportTypes();
    Rangers::Python::exportEngine();
    Rangers::Python::exportObject();
    Rangers::Python::exportNode();
    Rangers::Python::exportSprite();
    Rangers::Python::exportWidget();
    Rangers::Python::exportScriptWidget();
    Rangers::Python::exportAnimatedSprite();
    Rangers::Python::exportLabel();
    Rangers::Python::exportNinePatch();
    Rangers::Python::exportButton();
    Rangers::Python::exportCheckBox();
    Rangers::Python::exportLineEdit();
    Rangers::Python::exportScrollArea();
    Rangers::Python::exportWidgetNode();
    Rangers::Python::exportGAISprite();
    Rangers::Python::exportColorLabel();
    Rangers::Python::exportAction();
    Rangers::Python::exportActionListener();
    Rangers::Python::exportFont();
    Rangers::Python::exportResourceManager();
    Rangers::Python::exportShader();
    Rangers::Python::exportShaderProgram();
    Rangers::Python::exportSound();
    Rangers::Python::exportSoundManager();
    Rangers::Python::exportStyles();
    Rangers::Python::exportTexture();
    Rangers::Python::exportRadioButton();
    Rangers::Python::exportRadioButtonGroup();
    boost::python::def("pythonError", &Rangers::Python::pythonError);
    boost::python::def("pythonOut", &Rangers::Python::pythonOut);
    boost::python::def("execPythonScript", (void (*)(const std::wstring&, const boost::python::object&))&Rangers::execPythonScript);
    boost::python::def("execPythonLine", &Rangers::execPythonLine);
    boost::python::def("execPythonModule", &Rangers::execPythonModule);
}
