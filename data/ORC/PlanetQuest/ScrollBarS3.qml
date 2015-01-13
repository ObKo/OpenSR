import QtQuick 2.3
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

ScrollViewStyle {
    corner : Item {}
    frame: Item {}
    
    incrementControl: Image {
        source: styleData.pressed ? "res:/DATA/FormPQuest2/2S3BDownD.gi" :
               (styleData.hovered ? "res:/DATA/FormPQuest2/2S3BDownA.gi" :
                                    "res:/DATA/FormPQuest2/2S3BDownN.gi")
    }
    decrementControl: Image {
        source: styleData.pressed ? "res:/DATA/FormPQuest2/2S3BUpD.gi" :
               (styleData.hovered ? "res:/DATA/FormPQuest2/2S3BUpA.gi" :
                                    "res:/DATA/FormPQuest2/2S3BUpN.gi")
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
            source: styleData.pressed ? "res:/DATA/FormPQuest2/2S3BTopD.gi" :
                   (styleData.hovered ? "res:/DATA/FormPQuest2/2S3BTopA.gi" :
                                        "res:/DATA/FormPQuest2/2S3BTopN.gi")
            anchors.top: parent.top
        }
        Image {
            id: centerB
            source: styleData.pressed ? "res:/DATA/FormPQuest2/2S3BCenterD.gi" :
                   (styleData.hovered ? "res:/DATA/FormPQuest2/2S3BCenterA.gi" :
                                        "res:/DATA/FormPQuest2/2S3BCenterN.gi")
            anchors.top: topB.bottom
            anchors.bottom: bottomB.top
        }
        Image {
            id: bottomB
            source: styleData.pressed ? "res:/DATA/FormPQuest2/2S3BBottomD.gi" :
                   (styleData.hovered ? "res:/DATA/FormPQuest2/2S3BBottomA.gi" :
                                        "res:/DATA/FormPQuest2/2S3BBottomN.gi")
            anchors.bottom: parent.bottom
        }
    }
    handleOverlap: 0
    minimumHandleLength: 10
}
