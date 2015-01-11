import QtQuick 2.3
import OpenSR 1.0

Item
{
    id: screen
    anchors.fill: parent
    
    
    function createObjectFromURL(url, parent, id, properties) {
        properties = (typeof properties === 'undefined') ? {} : properties;
        ScreenLoaderScript.createObjectFromURL(url, parent, id, properties);
    }
    
    function changeScreen(url, properties) {
        properties = (typeof properties === 'undefined') ? {} : properties;
        createObjectFromURL(url, screen, "screenRequest", properties);
    }
    
    function componentObjectCreated(){
    }
}