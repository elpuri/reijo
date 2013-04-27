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
        renderedWidth: mini ? 40 : 480
        renderedHeight: mini ? 40 : 360
        maxRecursionDepth: 5
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

    VideoEncoder {
        renderer: whitted
        filename: "glassballs.mp4"
        bitrate: 2000000
    }

    Camera {
        id: cam
        position: Qt.vector3d(5.0, 8.0, -16.0)
        lookAt: Qt.vector3d(4.0, 0.5, -4.0)
        fov: 50
    }


    Room {
        id: whiteRoom
        roomWidth: 20.0
        roomHeight: 12.0
        roomDepth: 16.0
        rearWallVisible: false
        rightWallVisible: false

        NullShape {
            id: pivot
            position: Qt.vector3d(4.0, 0.0, -4.0)
            rotation: Qt.vector3d(0.0, time / duration * 360.0, 0.0)

            Sphere {
                radius: 3.0
                material: mirror
                position: Qt.vector3d(0.0, 3.0, 0.0)
            }

            ShapeFactory {
                id: factory
                property real ringRadius : 5.0
                property int ballCount : 10
                model: ballCount
                componentSelectorScript: { return glassSphereComponent; }
            }

            Component {
                id: glassSphereComponent
                Sphere {
                    id: sphere
                    radius: 1.5
                    material: Material {
                        color: Qt.hsla(sphere.ShapeFactory.index / factory.ballCount, 1.0, 0.5, 1.0)
                        onColorChanged: console.log(color + " " + ShapeFactory.index)
                        ambientReflectivity: 0.00
                        specularReflectivity: 1.0
                        diffuseReflectivity: 0.0
                        shininess: 600
                        reflectivity: 0.0125
                        transmittance: 0.9
                        indexOfRefraction: 1.6
                    }
                    position: Qt.vector3d(Math.sin(ShapeFactory.index / factory.ballCount * Math.PI * 2.0) * factory.ringRadius, 1.5,
                                          Math.cos(ShapeFactory.index / factory.ballCount * Math.PI * 2.0) * factory.ringRadius)
                    onPositionChanged: console.log(position + " " + ShapeFactory.index)
                    Component.onCompleted: console.log("pippeli" + ShapeFactory.index)
                }
            }

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
            id: mirror
            color: "white"
            ambientReflectivity: 0.00
            specularReflectivity: 1.0
            diffuseReflectivity: 0.0
            shininess: 50
            reflectivity: 1.0
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
            position: Qt.vector3d(5.0, 8.4, -12.0)
            color: "white"
            intensity: 8.0
        }


        PointLight {
            position: Qt.vector3d(-2.0, 8.4, 0.0)
            color: "white"
            intensity: 4.0
        }
    }
}
