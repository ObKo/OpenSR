resources:addMapping(L"mapping.txt");
widget = Rangers.LuaWidget:new(L"test.lua")
engine:addWidget(widget)
engine:focusWidget(widget)
