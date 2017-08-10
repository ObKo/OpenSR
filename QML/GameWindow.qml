import QtQuick 2.9
import QtQuick.Window 2.2
import OpenSR 1.0

Window
{
    width: 1024
    height: 768
    visible: true
    
    id: gameScreen
    
    function createObjectFromURL(url, parent, id, properties) {
        properties = (typeof properties === 'undefined') ? {} : properties;
        ScreenLoader.createObjectFromURL(url, parent, id, properties);
    }
    
    function changeScreen(url, properties) {
        var screens = gameScreen.children
        for(s in screens)
            screens[s].destroy();
        properties = (typeof properties === 'undefined') ? {} : properties;
        createObjectFromURL(url, gameScreen, "screenRequest", properties);
    }
    
    function componentObjectCreated(){
    }
}
