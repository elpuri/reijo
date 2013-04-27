import Reijo 1.0
import QtQuick 2.0

Scene {
    id: scene
    camera: cam
    renderer: whitted
    duration: 10.0
    frameTime: 1.0 / 30.0
    time: 0.0

    property bool mini : false

    WhittedRenderer {
        id: whitted
        renderedWidth: mini ? 240 : 480
        renderedHeight: mini ? 180 : 360
        maxRecursionDepth: 25
        ambientLightColor: Qt.vector3d(1.0, 1.0, 1.0)
        antiAliasing: WhittedRenderer.JitteredSamples
        samplesPerPixel: 8
    }

    HdrViewer {
        id: viewer
        renderer: whitted
        gamma: 2.0
        exposure: 1.0
    }

    VideoEncoder {
        renderer: whitted
        filename: "glasstorus.mp4"
        bitrate: 2000000
    }

    Camera {
        id: cam
        position: Qt.vector3d(0.0, 3.0, -16.0)
        lookAt: Qt.vector3d(0.0, 2.0, 0.0)
        fov: 40
    }


    Room {
        id: whiteRoom
        roomWidth: 20.0
        roomHeight: 12.0
        roomDepth: 16.0
        rearWallVisible: false
        rightWallVisible: false


        Box {
            scale: Qt.vector3d(4.0, 2.0, 1.0)
            position: Qt.vector3d(0.0, 2.0, -3.0)
            material: red
        }

        Torus {
            position: Qt.vector3d(2.0, 3.0, -9.0)
            rotation: Qt.vector3d(time * 36.0 + 45.0, time * 18.0 + 20.0, time * 28.0 + 25.0)
            material: glass
            innerRadius: 0.7
            outerRadius: 1.3
        }


        Material {
            id: glass
            color: "#aaeeff"
            ambientReflectivity: 0.0
            specularReflectivity: 1.0
            diffuseReflectivity: 0.025
            shininess: 250
            reflectivity: 0.0
            transmittance: 0.75
            indexOfRefraction: 1.3
        }

        Material {
            id: red
            color: "red"
            ambientReflectivity: 0.1
            diffuseReflectivity: 0.9
            specularReflectivity: 1.0
            reflectivity: 0.0
            shininess: 250
        }

        PointLight {
            position: Qt.vector3d(1.0, 10.0, -15.0)
            color: "white"
            intensity: 12
        }

        PointLight {
            position: Qt.vector3d(-3.0, 8.4, -5.0)
            color: "white"
            intensity: 4.0
        }
    }
}
