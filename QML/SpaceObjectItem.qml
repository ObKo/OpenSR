import QtQuick 2.3
import OpenSR 1.0
import OpenSR.World 1.0

Item {
    id: self
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
            image.visible = false;
            positioning = true;
            var drawer = Qt.createQmlObject("import OpenSR 1.0
                PlanetDrawer {}",
                self,
                "dynamicSnippet1");

            drawer.width = drawer.height = object.style.radius * 2;
            drawer.surface = object.style.image;
            drawer.cloud0  = object.style.cloud0;
            drawer.cloud1  = object.style.cloud1;

            var phaseAnim = Qt.createQmlObject("import QtQuick 2.3
                    PropertyAnimation {
                        loops: Animation.Infinite
                        property: 'phase'
                        from: 0.0; to: 1.0; duration: 36000
                    }",
                self);
            phaseAnim.target = drawer;
            phaseAnim.running = true;
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
