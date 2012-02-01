resources:addMapping(L"mapping.txt");
engine:setDefaultSkin(L"skin.json")
widget = Rangers.LuaWidget:new(L"test.lua")
engine:addWidget(widget)
engine:focusWidget(widget)
