import QtQuick 2.3
import OpenSR 1.0

Item {
    property string text
    property string normalImage: "qrc:/DATA/FormPlanet2/QuestButN.sci"
    property string hoveredImage: "qrc:/DATA/FormPlanet2/QuestButA.sci"
    property string downImage: "qrc:/DATA/FormPlanet2/QuestButD.sci"
    property string enterSound: "res:/Sound/ButtonEnter.wav"
    property string leaveSound: "res:/Sound/ButtonLeave.WAV"
    property string clickSound: "res:/Sound/ButtonClick.wav"
    property bool sounded: true
    signal clicked

    implicitWidth: Math.max(bg.implicitWidth, (label.implicitWidth + 10))
    implicitHeight: Math.max(bg.implicitHeight, (label.implicitHeight + 10))

    BorderImage {
        id: bg
        anchors.fill: parent
        source: hoverArea.pressed ?
            parent.downImage : (hoverArea.containsMouse ?
            parent.hoveredImage :
            parent.normalImage)
    }
    Text {
        id: label
        anchors.centerIn: parent
        text: parent.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        renderType: Text.NativeRendering
    }
    Sound {
        id: cSnd
        source: clickSound
    }
    Sound {
        id: eSnd
        source: enterSound
    }
    Sound {
        id: lSnd
        source: leaveSound
    }
    MouseArea {
        id: hoverArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: { parent.clicked() }
        onPressed: { cSnd.play() }
        onEntered: {  eSnd.play() }
        onExited: { lSnd.play() }
    }
} 
