resources:addMapping(L"mapping.txt");
engine:loadPlugin(L"./world")
engine:setDefaultSkin(L"skin.json")
widget = LuaWidget(L"ORC/startmenu.lua")
engine:addWidget(widget)
engine:focusWidget(widget)
