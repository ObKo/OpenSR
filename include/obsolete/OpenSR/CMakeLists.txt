SET(ENGINE_HEADERS
    Action.h
    ActionListener.h
    AnimatedSprite.h
    AnimatedTexture.h
    Button.h
    CheckBox.h
    ColorLabel.h
    ConsoleWidget.h
    Engine.h
    FSAdapter.h
    Font.h
    FTFont.h
    GAISprite.h
    Label.h
    LabelWidget.h
    LineEdit.h
    Log.h
    NinePatch.h
    Node.h
    Object.h
    Plugin.h
    PluginInterface.h
    PythonBindings.h
    RPKGAdapter.h
    RadioButton.h
    RadioButtonGroup.h
    ResourceAdapter.h
    ResourceManager.h
    ResourceObjectManager.h
    ScriptWidget.h
    ScrollArea.h
    Shader.h
    ShaderProgram.h
    Sound.h
    SoundManager.h
    Sprite.h
    SpriteWidget.h
    Styles.h
    Texture.h
    TiledBezierCurve.h
    TiledLine.h
    TiledPolyline.h
    Types.h
    Utils.h
    Widget.h
    WidgetNode.h
    global.h
)

SET(RANGER_HEADERS
    libRanger.h
)

SET(ENGINE_CONFIG_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

CONFIGURE_FILE(version.h.in version.h @ONLY)
CONFIGURE_FILE(config.h.in config.h @ONLY)

INSTALL(FILES ${ENGINE_HEADERS} ${RANGER_HEADERS} ${ENGINE_CONFIG_HEADERS} DESTINATION include/OpenSR)

ADD_SUBDIRECTORY(private)
ADD_SUBDIRECTORY(python)
ADD_SUBDIRECTORY(World)
