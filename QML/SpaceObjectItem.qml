import QtQuick 2.3
import OpenSR 1.0
import OpenSR.World 1.0

Item {
    property WorldObject object
    property bool positioning: false
    property int mouseDelta: 0

    signal entered(WorldObject obj)
    signal exited()

    x: positioning && object ? object.position.x : 0
    y: positioning && object ? object.position.y : 0

    AnimatedImage {
        id: image
        anchors.centerIn: parent
        cache: false

        SpaceMouseArea {
            id: mouse
            anchors.fill: parent
            radius: Math.min(width / 2, height / 2) - mouseDelta
            onEntered: mouseEntered(object)
            onExited: mouseExited()
        }
    }

    onObjectChanged: {
        if(!object) {
            image.source = "";
            positioning = false;
            return;
        }

        if(WorldManager.typeName(object.typeId) === "OpenSR::World::PlanetarySystem") {
            image.source = object.style.star
            positioning = false
        }
        else if(WorldManager.typeName(object.typeId) === "OpenSR::World::Asteroid") {
            image.source = object.style.texture
            positioning = true;
        } else if(WorldManager.typeName(object.typeId) === "OpenSR::World::InhabitedPlanet") {
            image.source = object.style.texture
            positioning = true;
        } else if(WorldManager.typeName(object.typeId) === "OpenSR::World::SpaceStation") {
            image.source = object.style.texture
            positioning = true;
        }
    }

    function mouseEntered() {
        if (object)
            entered(object);
    }

    function mouseExited() {
        exited(object);
    }
}
