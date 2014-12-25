import QtQuick 2.3
import QtMultimedia 5.4

Item {
    property string text
    property string normalImage: "qrc:/DATA/FormPlanet2/QuestButN.sci"
    property string hoveredImage: "qrc:/DATA/FormPlanet2/QuestButA.sci"
    property string downImage: "qrc:/DATA/FormPlanet2/QuestButD.sci"
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
//    SoundEffect {
//        id: clickSound
//        source: "qrc:/Sound/ButtonClick.wav"
//    }
//    SoundEffect {
//        id: enterSound
//        source: "qrc:/Sound/ButtonEnter.wav"
//    }
//    SoundEffect {
//        id: leaveSound
//        source: "qrc:/Sound/ButtonLeave.WAV"
//    }
    MouseArea {
        id: hoverArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: { parent.clicked() }
        onPressed: { /*clickSound.play()*/ }
        onEntered: { /*enterSound.play()*/ }
        onExited: { /*leaveSound.play()*/ }
    }
} 
