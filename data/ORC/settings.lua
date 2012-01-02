background = Rangers.Sprite:new_local(L"DATA/FormOptions2/2Bg.gi", this)
background:setWidth(engine:screenWidth())
background:setHeight(engine:screenHeight()) 
menuNode = Rangers.WidgetNode:new_local()
w = Rangers.Button:new_local(L"DATA/FormOptions2/2Bg.gi", menuNode);
scroll = Rangers.ScrollArea:new_local(L"DATA/ScrollBar/2V2_UpN.gi", L"DATA/ScrollBar/2V2_DownN.gi",
                                      L"DATA/ScrollBar/2V2_TopN.gi", L"DATA/ScrollBar/2V2_BottomN.gi",
                                      L"DATA/ScrollBar/2V2_CenterN.gi", menuNode, this)
scroll:setWidth(500)
scroll:setHeight(500)