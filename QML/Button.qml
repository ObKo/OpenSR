import QtQuick 2.3
import OpenSR 1.0

Item {
    property string text
    
    property color textColor: "black"
    property int textStyle: Text.Normal
    property color textStyleColor: "black"
    property font textFont
    
    property string normalImage: "res:/ORC/ButtonN.sci"
    property string hoveredImage: "res:/ORC/ButtonA.sci"
    property string downImage: "res:/ORC/ButtonD.sci"
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
        source: (hoverArea.pressed && downImage != "") ?
            parent.downImage : 
                ((hoverArea.containsMouse && hoveredImage != "") ?
                parent.hoveredImage : parent.normalImage)
    }
    Text {
        id: label
        anchors.centerIn: parent
        text: parent.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        renderType: Text.NativeRendering
        
        color: parent.textColor
        style: parent.textStyle
        styleColor: parent.textStyleColor
        font: parent.textFont
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
        onPressed: { if(sounded) cSnd.play() }
        onEntered: { if(sounded) eSnd.play() }
        onExited: { if(sounded) lSnd.play() }
    }
} 
