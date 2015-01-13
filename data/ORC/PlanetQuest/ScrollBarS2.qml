import QtQuick 2.3
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

ScrollViewStyle {
    corner : Item {}
    frame: Item {}
    
    incrementControl: Image {
        source: styleData.pressed ? "res:/DATA/FormPQuest2/2S2BDownD.gi" :
               (styleData.hovered ? "res:/DATA/FormPQuest2/2S2BDownA.gi" :
                                    "res:/DATA/FormPQuest2/2S2BDownN.gi")
    }
    decrementControl: Image {
        source: styleData.pressed ? "res:/DATA/FormPQuest2/2S2BUpD.gi" :
               (styleData.hovered ? "res:/DATA/FormPQuest2/2S2BUpA.gi" :
                                    "res:/DATA/FormPQuest2/2S2BUpN.gi")
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
            source: styleData.pressed ? "res:/DATA/FormPQuest2/2S2BTopD.gi" :
                   (styleData.hovered ? "res:/DATA/FormPQuest2/2S2BTopA.gi" :
                                        "res:/DATA/FormPQuest2/2S2BTopN.gi")
            anchors.top: parent.top
        }
        Image {
            id: centerB
            source: styleData.pressed ? "res:/DATA/FormPQuest2/2S2BCenterD.gi" :
                   (styleData.hovered ? "res:/DATA/FormPQuest2/2S2BCenterA.gi" :
                                        "res:/DATA/FormPQuest2/2S2BCenterN.gi")
            anchors.top: topB.bottom
            anchors.bottom: bottomB.top
        }
        Image {
            id: bottomB
            source: styleData.pressed ? "res:/DATA/FormPQuest2/2S2BBottomD.gi" :
                   (styleData.hovered ? "res:/DATA/FormPQuest2/2S2BBottomA.gi" :
                                        "res:/DATA/FormPQuest2/2S2BBottomN.gi")
            anchors.bottom: parent.bottom
        }
    }
    handleOverlap: 0
    minimumHandleLength: 10
}
