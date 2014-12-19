SET(ENGINE_PYTHON_HEADERS
    ObjectWrap.h
    SpriteWrap.h
    WidgetWrap.h
    Wrappers.h
)

INSTALL(FILES ${ENGINE_PYTHON_HEADERS} DESTINATION include/OpenSR/python)