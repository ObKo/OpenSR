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
    GAIAnimatedImage {
        id: grid
        
        sources: ["qrc:/DATA/FormMain2/2AnimMain.gai"]
        speed: 0.75
        anchors.fill: parent
        
        onCurrentFrameChanged: {
            if(currentFrame == 0)
                gridPauseAnim.restart();
        }
        SequentialAnimation {
            id: gridPauseAnim
            
            PropertyAction { target: grid; property: "playing"; value: false }
            PauseAnimation { duration: 10000 }
            PropertyAction { target: grid; property: "playing"; value: true }
        }
    }
    Image {
        source: "qrc:/DATA/FormMain3/2Planet.gi"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }
    GAIAnimatedImage {
        id: animLine
        sources: ["qrc:/DATA/FormMain2/2AnimLine.gai"]
        speed: 0.5
        width: parent.width
        anchors.right: parent.right
        anchors.top: parent.top
    }
    Image {
        source: "qrc:/DATA/FormMain3/2caption.gi"
        anchors.top: parent.top
        anchors.horizontalCenter: animLine.right
        anchors.topMargin: 20;
        anchors.horizontalCenterOffset: -310 * animLine.width / animLine.implicitWidth
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
    GAIAnimatedImage {
        id: animation
        sources: ["qrc:/DATA/FormMain3/2Ship1.gai", 
                  "qrc:/DATA/FormMain3/2Ship2.gai", 
                  "qrc:/DATA/FormMain3/2Ship3.gai"]
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }
}
