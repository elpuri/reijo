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
/*
    VideoEncoder {
        renderer: whitted
        filename: "torus.mp4"
        bitrate: 2000000
    }
*/
    Camera {
        id: cam
        position: Qt.vector3d(0.0, 5.0, -13.0)
        lookAt: box// Qt.vector3d(0.0, 0.0, 0.0)
        fov: 35
    }


    Room {
        id: whiteRoom
        roomWidth: 15.0
        roomHeight: 12.0
        roomDepth: 16.0
        rearWallVisible: false
        rightWallVisible: false
/*
        ShapeFactory {
            position: Qt.vector3d(0.0, 4.0, -3.0)
            model: 20
            componentSelectorScript:

        }
*/
        Box {
                       id: box
                       scale: Qt.vector3d(2.0, 4.0, 0.24)
                       position: Qt.vector3d(0.0, 2.0, -3.0)
                       rotation: Qt.vector3d(0.0, 33.0, 0.0)

                       material: Material {
                           color: "#ff0000"
                           ambientReflectivity: 0.00
                           diffuseReflectivity: 0.7
                           specularReflectivity: 1.0
                           shininess: 50
                           reflectivity: 0.0
                       }
                   }


        PointLight {
            position: Qt.vector3d(3.0, 8.4, -6.0)
            color: "white"
            intensity: 7.0
        }

    }
}
