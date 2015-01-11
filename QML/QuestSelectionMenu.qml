import QtQuick 2.3
import QtQuick.Controls 1.3
import QtGraphicalEffects 1.0
import OpenSR 1.0

Item {    
    signal questSelected(string url)
    
    id: questSelectionDialog
    implicitWidth: questSelectionBG.width
    implicitHeight: questSelectionBG.height
    
    ListModel {
        id: questListModel
    }
    
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
    } 
    
    Image {
        id: questImageBG
        source: "res:/DATA/FormLoadQuest/2ImageBG.gi"
        
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.rightMargin: 137
    }

    Image {
        id: questImage
        visible: false
    }

    OpacityMask {
        anchors.fill: questImageBG
        source: questImage
        maskSource: questImageBG
    }
    
    Image {
        id: questSelectionBG
        source: "res:/DATA/FormLoadQuest/2BG.gi"
    }
    Button {
        id: closeButton
        
        normalImage: "res:/DATA/FormSave2/2CloseN.gi"
        hoveredImage: "res:/DATA/FormSave2/2CloseA.gi"
        downImage: "res:/DATA/FormSave2/2CloseD.gi"
        
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: 30
        anchors.rightMargin: 23
        
        onClicked: {
            parent.destroy()
        }
    }
    Button {
        normalImage: "res:/DATA/FormSave2/2SaveN.gi"
        hoveredImage: ""
        downImage: "res:/DATA/FormSave2/2SaveD.gi"
        
        text: qsTr("Start")
        textColor: "#002842"
        textStyle: Text.Raised
        textStyleColor: "#21AEB5"
        textFont.bold: true
        
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: 79
        anchors.rightMargin: 224
        
        onClicked: {
            var questID = questListModel.get(questList.currentIndex).id;
            var url = "res:/" + engine.datValue("PlanetQuest.PlanetQuest")[questID].replace(/\\/g, "/");
            parent.questSelected(url);
            parent.destroy()
        }
    }
    
    Button {
        normalImage: "res:/DATA/FormSave2/2CancelN.gi"
        hoveredImage: ""
        downImage: "res:/DATA/FormSave2/2CancelD.gi"
        
        text: qsTr("Cancel")
        textColor: "#002842"
        textStyle: Text.Raised
        textStyleColor: "#21AEB5"
        textFont.bold: true
        
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: 38
        anchors.rightMargin: 225
        
        onClicked: closeButton.clicked()
    }
    
    ScrollView {    
        implicitWidth: 545
        implicitHeight: 460
        
        ListView {
            id: questList
            
            Component {
                id: questListDelegate
                Item {
                    width: questDelegateImage.width
                    height: questDelegateImage.height
                    Image {
                        id: questDelegateImage
                        source: questList.currentIndex == index ? 
                                ("res:/DATA/FormLoadQuest/2SlotActive.gi") :
                                questListDelegateMouse.pressed ?
                                ("res:/DATA/FormLoadQuest/2SlotOnPress.gi") :
                                questListDelegateMouse.containsMouse ?
                                ("res:/DATA/FormLoadQuest/2SlotOnMouse.gi") :
                                ("res:/DATA/FormLoadQuest/2SlotNormal.gi")
                    }
                    Text { 
                        text: name 
                        color: "#A5B68C"
                        
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                    }
                    MouseArea {
                        id: questListDelegateMouse
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: questList.currentIndex = index
                    }
                }
            }
            onCurrentIndexChanged: updateQuestInfo()
            
            model: questListModel
            delegate: questListDelegate
            spacing: 5
            
            anchors.fill: parent
        }
        
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 45
        anchors.topMargin: 150
    }
    
    ScrollView {    
        implicitWidth: 300
        implicitHeight: 260
        
        Text {
            id: questDescription
            width: 270
            
            wrapMode: Text.WordWrap
        }
        
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 150
        anchors.rightMargin: 40
    }
    
    Component.onCompleted: {
        var questList = engine.datValue("PlanetQuest.List");
        for(var key in questList)
        {
            var q = questList[key]
            questListModel.append({"name": q["Name"], "id": key});
        }
    }
    
    function updateQuestInfo()
    {
        var questID = questListModel.get(questList.currentIndex).id;
        var path = "res:/" + engine.datValue("PlanetQuest.PlanetQuest")[questID].replace(/\\/g, "/");
        var questInfo = QM.loadQuestInfo(path);
        var imageID = engine.datValue("PlanetQuest.List")[questID]["Image"]
        var imagePath = imageID.replace("Bm.", "").replace(".", "/");
        imagePath = "res:/DATA/" + imagePath + ".jpg";
        questImage.source = imagePath;
        questDescription.text = questInfo.descriptionText;
    }
}
