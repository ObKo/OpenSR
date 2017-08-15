import QtQuick 2.9
import QtQuick.Controls 2.2
import OpenSR 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    color: pallete.window

    SystemPalette {
        id: pallete
        colorGroup: SystemPalette.Active
    }

    ListModel {
        id: planetListModel
    }

    Component {
        id: simplePlanetDrawer
        Item {
            width: parent.width
            height: childrenRect.height

            PlanetDrawer {
                id: planetImage
                width: 64
                height: 64
                surface: image
                atmosphere: color
                cloud0: model.cloud0
                cloud1: model.cloud1
            }
            Text {
                id: planetName
                text: "<b>" + name + "</b><br/>" +
                      "Surface: " + planetImage.cloud0 + "<br/>" +
                      "Radius: " + radius
                color: parent.ListView.isCurrentItem ?
                           pallete.highlightedText : pallete.windowText

                verticalAlignment: Text.AlignVCenter

                anchors.left: planetImage.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
            }
        }
    }

    ScrollView {
        id: planetListScroll
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width * 0.33
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        ListView {
            id: planetListView
            anchors.fill: parent
            model: planetListModel
            delegate: simplePlanetDrawer
            highlight: Rectangle {
                color: pallete.highlight
            }
            highlightMoveDuration: 0
            highlightResizeDuration: 0

            onCurrentIndexChanged: {
                if (currentIndex != -1) {
                    var planet = planetListModel.get(currentIndex)
                    mainPlanetDrawer.surface = planet.image
                    mainPlanetDrawer.width = planet.radius * 2
                    mainPlanetDrawer.height = planet.radius * 2
                    mainPlanetDrawer.atmosphere = planet.color
                    mainPlanetDrawer.cloud0 = planet.cloud0
                    mainPlanetDrawer.cloud1 = planet.cloud1
                    cloud0Anim.duration = surfaceAnim.duration / 2.0 / planet.cloud0Speed
                    cloud1Anim.duration = surfaceAnim.duration / 2.0 / planet.cloud1Speed
                } else
                    mainPlanetDrawer.surface = ""
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                planetListView.currentIndex = planetListView.indexAt(mouseX, mouseY)
            }
        }
    }
    Rectangle {
        color: "#000000"

        anchors.left: planetListScroll.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        PlanetDrawer {
            id: mainPlanetDrawer
            anchors.centerIn: parent

            width: 256
            height: 256

            PropertyAnimation {
                id: surfaceAnim
                target: mainPlanetDrawer
                running: true
                loops: Animation.Infinite
                property: "phase"
                from: 0.0
                to: 1.0
                duration: 36000
            }
            PropertyAnimation {
                id: cloud0Anim
                target: mainPlanetDrawer
                running: true
                loops: Animation.Infinite
                property: "cloud0Phase"
                from: 0.0
                to: 1.0
                duration: 18000
            }
            PropertyAnimation {
                id: cloud1Anim
                target: mainPlanetDrawer
                running: true
                loops: Animation.Infinite
                property: "cloud1Phase"
                from: 0.0
                to: 1.0
                duration: 18000
            }
        }
        Rectangle {
            id: star
            color: "white"
            x: parent.width / 2 - width / 2 + 200
            y: parent.height / 2 - height / 2 + 0
            width: 32
            height: 32
            radius: width / 2
        }

        MouseArea {
            anchors.fill: parent
            onPositionChanged: {
                var angle = Math.atan2(height / 2 - mouse.y, mouse.x - width / 2)
                star.x = parent.width / 2 - star.width / 2 + 200 * Math.cos(angle)
                star.y = parent.height / 2 - star.height / 2 - 200 * Math.sin(angle)
                mainPlanetDrawer.lightAngle = angle
            }
        }
    }

    Component.onCompleted: {
        Engine.addDATFile("res:/main.dat")
        Engine.addDATFile("res:/rus.dat")
        Engine.addDATFile("res:/CacheData.dat", true)
        Engine.resources.addPKGArchive(Engine.dataDir + "/common.pkg")

        var planets = Engine.datValue("Data.SE.Planet")

        for (var n in planets) {
            var planet = planets[n]
            var color;
            if ("AtmColor" in planet) {
                var rgb = planet["AtmColor"].split(',')
                color = Qt.rgba(rgb[0] / 255.0, rgb[1] / 255.0, rgb[2] / 255.0, 1.0).toString()
            }
            else
                color = Qt.rgba(1.0, 1.0, 1.0, 1.0).toString()

            var splitted, cloud0 = "", cloud1 = "", cloud0Speed = 1.0, cloud1Speed = 1.0
            if ("Cloud0" in planet) {
                splitted = planet["Cloud0"].split(',')
                cloud0 = "dat:/" + splitted[1]
                cloud0Speed = parseFloat(splitted[0])
            }
            if ("Cloud1" in planet) {
                splitted = planet["Cloud1"].split(',')
                cloud1 = "dat:/" + splitted[1]
                cloud1Speed = parseFloat(splitted[0])
            }

            var repr = {
                "name": n,
                "image": "dat:/" + planet["Image"],
                "radius": planet["Radius"],
                "color": color,
                "cloud0": cloud0,
                "cloud1": cloud1,
                "cloud0Speed": cloud0Speed,
                "cloud1Speed": cloud1Speed
            };
            console.log("planet representation is ", JSON.stringify(repr) )
            planetListModel.append(repr);
        }
    }
}

