import Reijo 1.0
import QtQuick 2.0

Scene {
    id: scene
    camera: cam
    renderer: whitted
    duration: 15.0
    frameTime: 1.0 / 30.0
    time: 0

    property bool mini : false

    WhittedRenderer {
        id: whitted
        renderedWidth: mini ? 40 : 500
        renderedHeight: mini ? 40 : 500
        maxRecursionDepth: 5
        ambientLightColor: Qt.vector3d(1.0, 1.0, 1.0)
        antiAliasing: WhittedRenderer.JitteredSamples
        samplesPerPixel:1
        viewer: viewer
    }
/*
    VideoEncoder {
        renderer: whitted
        filename: "sphereflake.mp4"
        bitrate: 2000000
        autoStartPlayer: true
    }
*/
    HdrViewer {
        id: viewer
        renderer: whitted
        gamma: 1.8
        exposure: 1.0
    }

    Camera {
        id: cam
        position: Qt.vector3d(0.0, -5.0, -20.0)
        lookAt: Qt.vector3d(0.0, 0.0, 0.0)
        fov: 35
    }

    PointLight {
        position: Qt.vector3d(0.0, 10.0, -16.0)
        color: "white"
        intensity: 25.0
    }

    Sphereflake {
        rotation: Qt.vector3d(time / 15 * 480.0, time / 15 * 240, time / 15 * 360.0)
        material: sphereMat
        radius: 2.7
        levels: 3
        radiusMultiplier: 0.4
        positionMultiplier: 1.20
    }

    Material {
        id: sphereMat
            specularReflectivity: 1.0
            diffuseReflectivity: 0.6
            ambientReflectivity: 0.02
            reflectivity: 0.5
            color: "red"
            shininess: 300
    }
}
