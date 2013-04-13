import Reijo 1.0
import QtQuick 2.0

NullShape {
    property real roomWidth: 10.0
    property real roomDepth: 10.0
    property real roomHeight: 7.0
    property variant floorMaterial: defaultMaterial
    property variant wallMaterial: defaultMaterial
    property variant ceilingMaterial: defaultMaterial
    property alias leftWallVisible: leftWall.visible
    property alias rightWallVisible: rightWall.visible
    property alias frontWallVisible: frontWall.visible
    property alias rearWallVisible: rearWall.visible
    property alias ceilingVisible: ceiling.visible
    property alias floorVisible: floor.visible

    Material {
        id: defaultMaterial
        diffuseReflectivity: 0.8
        ambientReflectivity: 0.15
        color: "white"
    }

    // Left wall
    Plane {
        id: leftWall
        normal: Qt.vector3d(1.0, 0.0, 0.0)
        point: Qt.vector3d(-roomWidth / 2.0, 0.0, 0.0)
        material: wallMaterial
    }

    // Right wall
    Plane {
        id: rightWall
        normal: Qt.vector3d(-1.0, 0.0, 0.0)
        point: Qt.vector3d(roomWidth / 2.0, 0.0, 0.0)
        material: wallMaterial
    }

    // Front wall
    Plane {
        id: frontWall
        normal: Qt.vector3d(0.0, 0.0, -1.0)
        point: Qt.vector3d(0.0, 0.0, roomDepth / 2.0)
        material: wallMaterial
    }

    // Rear wall
    Plane {
        id: rearWall
        normal: Qt.vector3d(0.0, 0.0, -1.0)
        point: Qt.vector3d(0.0, 0.0, -roomDepth / 2.0)
        material: wallMaterial
    }

    // Floor
    Plane {
        id: floor
        normal: Qt.vector3d(0.0, 1.0, 0.0)
        point: Qt.vector3d(0.0, 0.0, 0.0)
        material: floorMaterial
    }

    // Ceiling
    Plane {
        id: ceiling
        normal: Qt.vector3d(0.0, -1.0, 0.0)
        point: Qt.vector3d(0.0, roomHeight, 0.0)
        material: ceilingMaterial
    }
}
