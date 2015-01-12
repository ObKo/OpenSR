import QtQuick 2.3
import QtQuick.Controls 1.3
import OpenSR 1.0

Item {
    property string questID
    
    id: questPlayerView
    
    anchors.fill: parent
    
    QuestPlayer {
        id: player
        
        onParametersChanged: {
            parametersView.text = "<style>.selected{color: #FFEF63}</style>" + parameters.join("<br>");
        }
        onCurrentTextChanged: {
            textView.text = "<style>.selected{color: " + d.skinSelectionColor + "} .win{color:green} .fail{color: red}</style>" + 
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
            textView.text = textView.text + "<p class=\"win\">" + qsTr("Quest completed!") + "</p>"
            transitonsModel.clear();
            transitonsModel.append({"id": -2, "title": qsTr("Exit"), "enabled": true});
        }        
        onQuestFailed: {
            textView.text = textView.text + "<p class=\"fail\">" + qsTr("Quest failed!") + "</p>"
            if(death)
                textView.text = textView.text + "<p class=\"fail\">" + qsTr("You're dead!") + "</p>"
            transitonsModel.clear();
            transitonsModel.append({"id": -2, "title": qsTr("Exit"), "enabled": true});
        }
    }    
    
    ListModel {
        id: transitonsModel
    }
    
    Image {
        source: player.image
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 30
        anchors.topMargin: 30
    }
    
    BorderImage {
        source: "res:/DATA/FormPQuest2/2S" + d.currentSkin + ".gi"
        width: implicitWidth + background.width - background.implicitWidth
        height: implicitHeight + background.height - background.implicitHeight
        
        border.left: 30
        border.top: 505
        border.bottom: 35
        border.right: 30
        
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 35
        anchors.topMargin: 5
    }
    
    BorderImage {
        id: background
        source: "res:/DATA/FormPQuest2/2BG.gi"
        
        border.top: 490
        border.bottom: 60
        border.right: 540
        border.left: 145

        anchors.fill: parent
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
        id: textScroll
        width: 550 + background.width - background.implicitWidth
        height: 365
        style: Qt.createComponent("res:/ORC/PlanetQuest/ScrollBarS1.qml")
        
        Text {
            id: textView
            color: d.skinTextColor
            textFormat: Text.RichText
            width: 512 + background.width - background.implicitWidth
            wrapMode: TextEdit.WordWrap            
        }
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 62
        anchors.topMargin: 49
    }
    
    ScrollView {    
        id: answersScroll
        width: 567 + background.width - background.implicitWidth
        height: 217 + background.height - background.implicitHeight
        style: Qt.createComponent("res:/ORC/PlanetQuest/ScrollBarS1.qml")
        
        ListView {
            id: transitonsList
                        
            model: transitonsModel
            delegate: Component {
                Item {
                    width: transitonsBG.width
                    height: transitonsBG.height
                    Rectangle {
                        id: transitonsBG
                        width: answerImg.width + 5 + transTitle.width + 20
                        height: transTitle.height + 5
                        color: "#80000000"
                        visible: transitonsListDelegateMouse.containsMouse
                    }
                    Image {
                        id: answerImg
                        source: model.enabled ? "res:/DATA/FormPQuest2/2S" + ((d.currentSkin == 4) ? "4" : "1") + "Answer.gi" : 
                            "res:/DATA/FormPQuest2/2S" + ((d.currentSkin == 4) ? "4" : "1") + "AnswerH.gi"
                        
                        anchors.left: transitonsBG.left
                        anchors.leftMargin: transitonsListDelegateMouse.pressed ? 13 : 10
                        anchors.verticalCenter: transitonsBG.verticalCenter
                    }
                    Text { 
                        id: transTitle
                        width: 500 + background.width - background.implicitWidth
                        text: title 
                        color: model.enabled ? d.skinTextColor : "#808080"
                        textFormat: Text.RichText
                        wrapMode: TextEdit.WordWrap  
                        
                        anchors.left: answerImg.right
                        anchors.leftMargin: 5
                        anchors.verticalCenter: transitonsBG.verticalCenter
                    }
                    MouseArea {
                        id: transitonsListDelegateMouse
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            if(!model.enabled)
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
        x: 45
        y: 486
    }
    
    Button {
        normalImage: "res:/DATA/FormPQuest2/2CloseN.gi"
        hoveredImage: "res:/DATA/FormPQuest2/2CloseA.gi"
        downImage: "res:/DATA/FormPQuest2/2CloseD.gi"
        
        onClicked: questExit()
        
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
    
    Item {
        width: childrenRect.width
        height: childrenRect.height
        
        Image {
            id: skinB1
            source: (d.currentSkin == 1) ? 
                "res:/DATA/FormPQuest2/2Skin1D.gi" :
                "res:/DATA/FormPQuest2/2Skin1N.gi"
                
            MouseArea {
                anchors.fill: parent
                onClicked: changeSkin(1)
            }
            anchors.left: parent.left
        }
        Image {
            id: skinB2
            source: (d.currentSkin == 2) ? 
                "res:/DATA/FormPQuest2/2Skin2D.gi" :
                "res:/DATA/FormPQuest2/2Skin2N.gi"
                
            MouseArea {
                anchors.fill: parent
                onClicked: changeSkin(2)
            }
            anchors.left: skinB1.right
            anchors.leftMargin: 3
        }
        Image {
            id: skinB3
            source: (d.currentSkin == 3) ? 
                "res:/DATA/FormPQuest2/2Skin3D.gi" :
                "res:/DATA/FormPQuest2/2Skin3N.gi"
                
            MouseArea {
                anchors.fill: parent
                onClicked: changeSkin(3)
            }
            anchors.left: skinB2.right
            anchors.leftMargin: 3
        }
        Image {
            id: skinB4
            source: (d.currentSkin == 4) ? 
                "res:/DATA/FormPQuest2/2Skin4D.gi" :
                "res:/DATA/FormPQuest2/2Skin4N.gi"
                
            MouseArea {
                anchors.fill: parent
                onClicked: changeSkin(4)
            }
            anchors.left: skinB3.right
            anchors.leftMargin: 3
        }
        
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: 11
        anchors.rightMargin: 425
    }
    
    Component.onCompleted: {
        player.questID = questID;
    }
    
    function changeSkin(number) {
        d.currentSkin = number
        if(number == 1 || number == 2) {
            d.skinTextColor = "#CECFAD";
            d.skinSelectionColor = "#FFEF63";
        }
        else if(number == 3 || number == 4) {
            d.skinTextColor = "#000000";
            d.skinSelectionColor = "#0000AD";
        }
        textScroll.style = Qt.createComponent("res:/ORC/PlanetQuest/ScrollBarS" + number + ".qml")
        answersScroll.style = Qt.createComponent("res:/ORC/PlanetQuest/ScrollBarS" + number + ".qml")
        player.onCurrentTextChanged()
    }
    
    function questExit() {
        questPlayerView.destroy()
        changeScreen("qrc:/OpenSR/MainMenu.qml");
    }
    
    QtObject {
         id: d
         property int currentSkin: 1
         property string skinTextColor: "#CECFAD"
         property string skinSelectionColor: "#FFEF63"
    }
}