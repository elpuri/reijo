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
        renderedWidth: mini ? 40 : 800
        renderedHeight: mini ? 40 : 480
        maxRecursionDepth: 17
        ambientLightColor: Qt.vector3d(1.0, 1.0, 1.0)
        antiAliasing: WhittedRenderer.JitteredSamples
        samplesPerPixel: 1
    }

    HdrViewer {
        id: viewer
        renderer: whitted
        gamma: 2.0
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
        position: Qt.vector3d(12.0, 8.0, -16.0)
        lookAt: Qt.vector3d(2.0, 0.0, 0.0)
        fov: 15
    }


    Room {
        id: whiteRoom
        roomWidth: 15.0
        roomHeight: 12.0
        roomDepth: 16.0
        rearWallVisible: false
        rightWallVisible: false

        Component.onCompleted: {
            var gridDim = 4
            var elementSize = 1.5
            var xOffset = -(gridDim - 1) * 0.5 * elementSize
            var zOffset = xOffset

            for (var i=0; i < gridDim; i++) {
                for (var j=0; j < gridDim; j++) {
                    var pilar = glassPilar.createObject();
                    var height = 1.0 + Math.random() * 2.0
                    pilar.scale = Qt.vector3d(0.45, height, 0.45)
                    pilar.position = Qt.vector3d(xOffset + i * elementSize, height * 0.5, zOffset + j * elementSize)
                    grid.addNode(pilar)
                }
            }
        }

        NullShape {
            id: grid
            position: Qt.vector3d(3.0, 0.0, -3.0)
            //rotation: Qt.vector3d(0.0, 30.0, 0.0)
        }

        Component {
            id: glassPilar
            Box {
                material: glass
            }
        }

        MirrorBall {
            position: Qt.vector3d(8.0, 1.0, 0.0)
        }


        Material {
            id: glass
            color: "#aaeeff"
            ambientReflectivity: 0.00
            specularReflectivity: 0.2
            diffuseReflectivity: 0.4
            shininess: 50
            reflectivity: 0.1
            transmittance: 0.8
            indexOfRefraction: 1.3
        }


        Material {
            id: yellowGlass
            color: "#ffff44"
            ambientReflectivity: 0.00
            specularReflectivity: 1.0
            diffuseReflectivity: 0.1
            shininess: 250
            reflectivity: 0.1
            transmittance: 0.8
            indexOfRefraction: 1.6
        }

        Material {
            id: mirror
            color: "white"
            ambientReflectivity: 0.00
            specularReflectivity: 1.0
            diffuseReflectivity: 0.0
            shininess: 50
            reflectivity: 1.0
        }


        PointLight {
            position: Qt.vector3d(5.0, 8.4, -12.0)
            color: "white"
            intensity: 12.0
        }

/*
        PointLight {
            position: Qt.vector3d(-2.0, 8.4, 0.0)
            color: "white"
            intensity: 4.0
        }*/
    }
}
