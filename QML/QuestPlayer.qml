import QtQuick 2.3
import QtQuick.Controls 1.3
import OpenSR 1.0

Item {
    property string source
    
    id: questPlayerView
    
    anchors.fill: parent
    
    QuestPlayer {
        id: player
        
        onParametersChanged: {
            parametersView.text = "<style>.selected{color: #FFEF63}</style>" + parameters.join("<br>");
        }
        onCurrentTextChanged: {
            textView.text = "<style>.selected{color: #FFEF63} .win{color:green} .fail{color: red}</style>" + 
                "<p>" + currentText.split("\r\n").join("</p><p>") + "</p>"
        }
        onQuestLocation: {
            transitonsModel.clear();
            
            var trans = player.transitions;
            for(var i = 0; i < trans.length; i++) {
                if(trans[i].title == "")
                    trans[i].title = qsTr("Next");
                transitonsModel.append(trans[i]);
            }
        }
        onQuestTransition: {
            transitonsModel.clear();
            transitonsModel.append({"id": -1, "title": qsTr("Next"), "enabled": true});
        }
        onQuestCompleted: {
            textView.text = textView.text + "<p color=\"win\">" + qsTr("Quest completed!") + "</p>"
            transitonsModel.clear();
            transitonsModel.append({"id": -2, "title": qsTr("Exit"), "enabled": true});
        }        
        onQuestFailed: {
            textView.text = textView.text + "<p class=\"fail\">" + qsTr("Quest failed!") + "</p>"
            if(death)
                textView.text = textView.text + "<p color=\"fail\">" + qsTr("You're dead!") + "</p>"
            transitonsModel.clear();
            transitonsModel.append({"id": -2, "title": qsTr("Exit"), "enabled": true});
        }
    }    
    
    ListModel {
        id: transitonsModel
    }
    
    Image {
        source: "res:/DATA/FormPQuest2/2S1.gi"
        
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 35
        anchors.topMargin: 5
    }
    
    Image {
        source: "res:/DATA/FormPQuest2/2BG.gi"
    }
    
    Item {
        implicitWidth: p1.implicitWidth + p2.implicitWidth + p3.implicitWidth
        implicitHeight: p1.implicitHeight + p4.implicitHeight + p7.implicitHeight
        width: Math.max(implicitWidth, parametersView.implicitWidth + 40)
        height: Math.max(implicitHeight, parametersView.implicitHeight + 80)
        
        Image {id: p1; source: "res:/DATA/FormPQuest2/2WinTopLeft.gi"; 
            anchors.top: parent.top; anchors.left: parent.left}
        Image {id: p2; source: "res:/DATA/FormPQuest2/2WinTop.gi"; 
            anchors.top: parent.top; anchors.left: p1.right; anchors.right: p3.left; fillMode: Image.TileHorizontally}
        Image {id: p3; source: "res:/DATA/FormPQuest2/2WinTopRight.gi"; 
            anchors.top: parent.top; anchors.right: parent.right}
        
        Image {id: p4; source: "res:/DATA/FormPQuest2/2WinLeft.gi"; 
            anchors.top: p1.bottom; anchors.left: parent.left; anchors.bottom: p7.top;
            fillMode: Image.TileVertically}
        Image {id: p5; source: "res:/DATA/FormPQuest2/2WinTexture.gi"; 
            anchors.top: p2.bottom; anchors.left: p4.right; anchors.bottom: p8.top; anchors.right: p6.left;
            fillMode: Image.Tile}
        Image {id: p6; source: "res:/DATA/FormPQuest2/2WinRight.gi"; 
            anchors.top: p3.bottom; anchors.right: parent.right; anchors.bottom: p9.top;
            fillMode: Image.TileVertically}
            
        Image {id: p7; source: "res:/DATA/FormPQuest2/2WinBottomLeft.gi"; 
            anchors.bottom: parent.bottom; anchors.left: parent.left}
        Image {id: p8; source: "res:/DATA/FormPQuest2/2WinBottom.gi"; 
            anchors.bottom: parent.bottom; anchors.left: p7.right; anchors.right: p9.left; fillMode: Image.TileHorizontally}
        Image {id: p9; source: "res:/DATA/FormPQuest2/2WinBottomRight.gi"; 
            anchors.bottom: parent.bottom; anchors.right: parent.right}
            
        Text {
            id: parametersView
            color: "#CECFAD"
            width: 250
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
            
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 30
            anchors.leftMargin: 30
        }
        
        x: parent.width - 380
        y: 460
    }
    ScrollView {    
        implicitWidth: 550
        implicitHeight: 375
        
        Text {
            id: textView
            color: "#CECFAD"
            textFormat: Text.RichText
            width: 512
            wrapMode: TextEdit.WordWrap            
        }
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 60
        anchors.topMargin: 50
    }
    
    ScrollView {    
        implicitWidth: 550
        implicitHeight: 375
        
        ListView {
            id: transitonsList
                        
            model: transitonsModel
            delegate: Component {
                Item {
                    width: childrenRect.width
                    height: childrenRect.height
                    Rectangle {
                        id: transitonsBG
                        width: transTitle.width + 20
                        height: transTitle.height + 5
                        color: "#80000000"
                        visible: transitonsListDelegateMouse.containsMouse
                    }
                    Text { 
                        id: transTitle
                        width: 490
                        text: title 
                        color: enabled ? "#CECFAD" : "#808080"
                        textFormat: Text.RichText
                        wrapMode: TextEdit.WordWrap  
                        
                        anchors.left: transitonsBG.left
                        anchors.leftMargin: 10
                        anchors.verticalCenter: transitonsBG.verticalCenter
                    }
                    MouseArea {
                        id: transitonsListDelegateMouse
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            if(!enabled)
                                return;
                            if(id == -2) {
                                questExit();
                            }
                            else if(id == -1) {
                                player.finishTransition()
                            }
                            else {
                                player.startTransition(id)
                            }
                        }                        
                    }
                }
            }
            
            anchors.fill: parent
        }
        x: 60
        y: 485
    }
    
    Component.onCompleted: {
        player.source = source;
    }
    
    function questExit()
    {
        questPlayerView.destroy()
        changeScreen("qrc:/OpenSR/MainMenu.qml");
    }
}