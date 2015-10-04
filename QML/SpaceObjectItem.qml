import QtQuick 2.3
import OpenSR 1.0
import OpenSR.World 1.0

Item {
    property WorldObject object

    signal entered(WorldManager obj)
    signal exited()

    AnimatedImage {
        id: image
        anchors.centerIn: parent

        SpaceMouseArea {
            id: mouse
            anchors.fill: parent
            radius: Math.min(width / 2, height / 2)
            onEntered: mouseEntered()
            onExited: mouseExited()
        }
    }

    Component.onCompleted: {
        if(WorldManager.typeName(object.typeId) === "OpenSR::World::PlanetarySystem")
            image.source = object.style.star
    }

    function mouseEntered() {
        if (object)
            entered(object);
    }

    function mouseExited() {
        exited(object);
    }
} 
