SET(ENGINE_PRIVATE_HEADERS
    AnimatedSprite_p.h
    Button_p.h
    CheckBox_p.h
    ColorLabel_p.h
    ConsoleWidget_p.h
    Engine_p.h
    GAISprite_p.h
    Label_p.h
    LineEdit_p.h
    NinePatch_p.h
    Node_p.h
    Object_p.h
    RadioButtonGroup_p.h
    RadioButton_p.h
    ScriptWidget_p.h
    ScrollArea_p.h
    SpriteWidget_p.h
    Sprite_p.h
    TiledLine_p.h
    WidgetNode_p.h
    Widget_p.h
)

INSTALL(FILES ${ENGINE_PRIVATE_HEADERS} DESTINATION include/OpenSR/private)