import Reijo 1.0
import QtQuick 2.0

Scene {
    id: scene
    camera: cam
    renderer: whitted
    duration: 5.0
    frameTime: 0.0 //1.0 / 30.0
    time: 0.0

    property bool mini : false

    WhittedRenderer {
        id: whitted
        renderedWidth: mini ? 40 : 640
        renderedHeight: mini ? 40 : 360
        maxRecursionDepth: 5
        ambientLightColor: Qt.vector3d(1.0, 1.0, 1.0)
        antiAliasing: WhittedRenderer.JitteredSamples
        samplesPerPixel: 1
    }

    HdrViewer {
        id: viewer
        renderer: whitted
        gamma: 1.3
        exposure: 1.0
    }

    VideoEncoder {
        renderer: whitted
        filename: "torus.mp4"
        bitrate: 2000000
    }

    Camera {
        id: cam
        position: Qt.vector3d(0.0, 3.0, -13.0)
        lookAt: Qt.vector3d(0.0, 0.0, 0.0)
        fov: 35
    }


    Room {
        id: whiteRoom
        roomWidth: 15.0
        roomHeight: 12.0
        roomDepth: 16.0
        rearWallVisible: false
        rightWallVisible: false

        Torus {
            id: torus
            innerRadius: 1.0
            outerRadius: 2.0
            position: Qt.vector3d(0.0, 4.0, -3.0)
            rotation: Qt.vector3d(time * 360.0 / scene.duration, 0.0, time * 360.0 / scene.duration)
            onRotationChanged: console.log(rotation)
            material: Material {
                color: "#ff0000"
                ambientReflectivity: 0.05
                diffuseReflectivity: 0.7
                specularReflectivity: 1.0
                shininess: 50
                reflectivity: 0.2
            }
        }


        PointLight {
            position: Qt.vector3d(0.0, 3.0, -10.0)
            color: "white"
            intensity: 3.0
        }

    }
}
