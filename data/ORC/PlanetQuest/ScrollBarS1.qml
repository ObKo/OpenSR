import QtQuick 2.3
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

ScrollViewStyle {
    corner : Item {}
    frame: Item {}
    
    incrementControl: Image {
        source: styleData.pressed ? "res:/DATA/FormPQuest2/2S1BDownD.gi" :
               (styleData.hovered ? "res:/DATA/FormPQuest2/2S1BDownA.gi" :
                                    "res:/DATA/FormPQuest2/2S1BDownN.gi")
    }
    decrementControl: Image {
        source: styleData.pressed ? "res:/DATA/FormPQuest2/2S1BUpD.gi" :
               (styleData.hovered ? "res:/DATA/FormPQuest2/2S1BUpA.gi" :
                                    "res:/DATA/FormPQuest2/2S1BUpN.gi")
    }
    scrollBarBackground: Item {
        implicitWidth: 17
        implicitHeight: 20 + 20
    }
    handle: Item {
        implicitWidth: centerB.implicitWidth
        implicitHeight: topB.implicitHeight + centerB.implicitHeight + bottomB.implicitHeight
        Image {
            id: topB
            source: styleData.pressed ? "res:/DATA/FormPQuest2/2S1BTopD.gi" :
                   (styleData.hovered ? "res:/DATA/FormPQuest2/2S1BTopA.gi" :
                                        "res:/DATA/FormPQuest2/2S1BTopN.gi")
            anchors.top: parent.top
        }
        Image {
            id: centerB
            source: styleData.pressed ? "res:/DATA/FormPQuest2/2S1BCenterD.gi" :
                   (styleData.hovered ? "res:/DATA/FormPQuest2/2S1BCenterA.gi" :
                                        "res:/DATA/FormPQuest2/2S1BCenterN.gi")
            anchors.top: topB.bottom
            anchors.bottom: bottomB.top
        }
        Image {
            id: bottomB
            source: styleData.pressed ? "res:/DATA/FormPQuest2/2S1BBottomD.gi" :
                   (styleData.hovered ? "res:/DATA/FormPQuest2/2S1BBottomA.gi" :
                                        "res:/DATA/FormPQuest2/2S1BBottomN.gi")
            anchors.bottom: parent.bottom
        }
    }
    handleOverlap: 0
    minimumHandleLength: 10
}
