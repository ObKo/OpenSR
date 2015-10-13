import QtQuick 2.3
import OpenSR 1.0
import OpenSR.World 1.0

Item {
    property SpaceObject object

    property SpaceObject oldObject

    TexturedBezierCurve {
        id: curve
        source: "res:/DATA/PI/UnitPath1.gi"
        minStep: 20
        curve: object.trajectory[0]
    }

    onObjectChanged: {
        if (oldObject)
            oldObject.trajectoryChanged.disconnect(updateTrajectory)

        if (object)
            object.trajectoryChanged.connect(updateTrajectory)

        oldObject = object
    }

    function updateTrajectory() {
        console.log("Trajectory update!");
    }
} 
