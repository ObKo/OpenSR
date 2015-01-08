import QtQuick 2.3
import OpenSR 1.0

Item
{
    id: screen
    anchors.fill: parent
    
    function createObjectFromURL(url, parent, id) {
        ScreenLoaderScript.createObjectFromURL(url, parent, id);
    }
    
    function changeScreen(url) {
        createObjectFromURL(url, screen);
    }
    
    function componentObjectCreated(){
    }
}