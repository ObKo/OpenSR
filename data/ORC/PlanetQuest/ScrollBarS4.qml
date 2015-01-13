import QtQuick 2.3
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

ScrollViewStyle {
    corner : Item {}
    frame: Item {}
    
    incrementControl: Image {
        source: styleData.pressed ? "res:/DATA/FormPQuest2/2S4BDownD.gi" :
               (styleData.hovered ? "res:/DATA/FormPQuest2/2S4BDownA.gi" :
                                    "res:/DATA/FormPQuest2/2S4BDownN.gi")
    }
    decrementControl: Image {
        source: styleData.pressed ? "res:/DATA/FormPQuest2/2S4BUpD.gi" :
               (styleData.hovered ? "res:/DATA/FormPQuest2/2S4BUpA.gi" :
                                    "res:/DATA/FormPQuest2/2S4BUpN.gi")
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
            source: styleData.pressed ? "res:/DATA/FormPQuest2/2S4BTopD.gi" :
                   (styleData.hovered ? "res:/DATA/FormPQuest2/2S4BTopA.gi" :
                                        "res:/DATA/FormPQuest2/2S4BTopN.gi")
            anchors.top: parent.top
        }
        Image {
            id: centerB
            source: styleData.pressed ? "res:/DATA/FormPQuest2/2S4BCenterD.gi" :
                   (styleData.hovered ? "res:/DATA/FormPQuest2/2S4BCenterA.gi" :
                                        "res:/DATA/FormPQuest2/2S4BCenterN.gi")
            anchors.top: topB.bottom
            anchors.bottom: bottomB.top
        }
        Image {
            id: bottomB
            source: styleData.pressed ? "res:/DATA/FormPQuest2/2S4BBottomD.gi" :
                   (styleData.hovered ? "res:/DATA/FormPQuest2/2S4BBottomA.gi" :
                                        "res:/DATA/FormPQuest2/2S4BBottomN.gi")
            anchors.bottom: parent.bottom
        }
    }
    handleOverlap: 0
    minimumHandleLength: 10
}
