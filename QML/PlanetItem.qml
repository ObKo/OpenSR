import QtQuick 2.9
import OpenSR 1.0
import OpenSR.World 1.0

PlanetDrawer {
    property Planet planet
    id: drawer

    width: planet.style.radius * 2
    height: planet.style.radius * 2

    surface: planet.style.surface
    atmosphere: planet.style.atmosphere
    cloud0: planet.style.cloud0
    cloud1: planet.style.cloud1
    lightAngle: Math.atan2(planet.position.y, -planet.position.x)

    PropertyAnimation {
        id: surfaceAnim
        target: drawer
        running: true
        loops: Animation.Infinite
        property: "phase"
        from: 0.0
        to: 1.0
        duration: 36000
    }
    PropertyAnimation {
        id: cloud0Anim
        target: drawer
        running: true
        loops: Animation.Infinite
        property: "cloud0Phase"
        from: 0.0
        to: 1.0
        duration: 18000
    }
    PropertyAnimation {
        id: cloud1Anim
        target: drawer
        running: true
        loops: Animation.Infinite
        property: "cloud1Phase"
        from: 0.0
        to: 1.0
        duration: 18000
    }
}
