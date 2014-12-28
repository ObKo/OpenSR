import QtQuick 2.3
import OpenSR 1.0

Item {
    id: menu

    Connections {
        onHeightChanged: {
            background.height = menu.height
            bgToAnim.to = - background.width + menu.width
            bgFromAnim.from = - background.width + menu.width
            bgAnim.restart()
        }
    }

    Image {
        id: background
        source: "qrc:/DATA/FormMain3/2bg.gi"
        fillMode: Image.PreserveAspectCrop
        SequentialAnimation on x {
            id: bgAnim
            loops: Animation.Infinite
            PropertyAnimation { id: bgToAnim; duration: 50000; from: 0 }
            PropertyAnimation { id: bgFromAnim; duration: 50000; to: 0 }
        }
    }
//     AnimatedImage {
//         source: "qrc:/DATA/FormMain2/2AnimMain.gai"
//         fillMode: Image.Tile
//         anchors.fill: parent
//     }
    Image {
        source: "qrc:/DATA/FormMain3/2Planet.gi"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }
//     AnimatedImage {
//         source: "qrc:/DATA/FormMain2/2AnimLine.gai"
//         anchors.right: parent.right
//         anchors.top: parent.top
//     }
    Image {
        source: "qrc:/DATA/FormMain3/2caption.gi"
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 20;
    }
    Item {
        width: childrenRect.width
        height: childrenRect.height

        Button {
            id: newButton
            normalImage: "qrc:/DATA/FormMain2/2ButNewN.gi"
            hoveredImage: "qrc:/DATA/FormMain2/2ButNewA.gi"
            downImage: "qrc:/DATA/FormMain2/2ButNewD.gi"
        }
        Button {
            id: loadButton
            normalImage: "qrc:/DATA/FormMain2/2ButLoadN.gi"
            hoveredImage: "qrc:/DATA/FormMain2/2ButLoadA.gi"
            downImage: "qrc:/DATA/FormMain2/2ButLoadD.gi"
            anchors.top: newButton.bottom
            anchors.topMargin: 10
        }
        Button {
            id: settingsButton
            normalImage: "qrc:/DATA/FormMain2/2ButSettingsN.gi"
            hoveredImage: "qrc:/DATA/FormMain2/2ButSettingsA.gi"
            downImage: "qrc:/DATA/FormMain2/2ButSettingsD.gi"
            anchors.top: loadButton.bottom
            anchors.topMargin: 10
        }
        Button {
            id: recordsButton
            normalImage: "qrc:/DATA/FormMain2/2ButRecordsN.gi"
            hoveredImage: "qrc:/DATA/FormMain2/2ButRecordsA.gi"
            downImage: "qrc:/DATA/FormMain2/2ButRecordsD.gi"
            anchors.top: settingsButton.bottom
            anchors.topMargin: 10
        }
        Button {
            id: aboutButton
            normalImage: "qrc:/DATA/FormMain2/2ButAboutN.gi"
            hoveredImage: "qrc:/DATA/FormMain2/2ButAboutA.gi"
            downImage: "qrc:/DATA/FormMain2/2ButAboutD.gi"
            anchors.top: recordsButton.bottom
            anchors.topMargin: 10
        }
        Button {
            id: exitButton
            normalImage: "qrc:/DATA/FormMain2/2ButExitN.gi"
            hoveredImage: "qrc:/DATA/FormMain2/2ButExitA.gi"
            downImage: "qrc:/DATA/FormMain2/2ButExitD.gi"
            anchors.top: aboutButton.bottom
            anchors.topMargin: 10
        }
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 100
        anchors.right: parent.right
        anchors.rightMargin: 100
    }
//     AnimatedImage {
//         source: "qrc:/DATA/FormMain3/2Ship1.gai"
//         anchors.left: parent.left
//         anchors.bottom: parent.bottom
//     }
}
