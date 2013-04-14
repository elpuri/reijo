import Reijo 1.0
import QtQuick 2.0

Scene {
    camera: cam
    renderer: whitted

    property bool mini : false

    WhittedRenderer {
        id: whitted
        renderedWidth: mini ? 20 : 640
        renderedHeight: mini ? 20 : 360
        maxRecursionDepth: 5
        ambientLightColor: Qt.vector3d(1.0, 1.0, 1.0)
        antiAliasing: WhittedRenderer.JitteredSamples
        samplesPerPixel: 8
    }

    HdrViewer {
        id: viewer
        renderer: whitted
        gamma: 1.8
    }

    Material {
        id: redMaterial
        color: "#ff2222"
        ambientReflectivity: 0.08
        diffuseReflectivity: 1.0
        specularReflectivity: 0.8
        shininess: 100
        reflectivity: 0.0
    }

    Room {
        id: whiteRoom
        roomWidth: 15.0
        roomHeight: 12.0
        roomDepth: 16.0
        rearWallVisible: false
        rightWallVisible: false


        ShapeFactory {
            id: clump
            position: Qt.vector3d(0.0, 3.5, 0.0)
            model: 50
            componentSelectorScript: { Math.random() < 0.7 ? sphereComponent : mirrorSphereComponent; }
        }


        Component {
            id: sphereComponent
            Sphere {
                position: Qt.vector3d((Math.random() - 0.5) * 7.0, (Math.random() - 0.5) * 5.0, (Math.random() - 0.5) * 7.0)
                radius: Math.random() * 0.3 + 0.4
                material: Material {
                    color: Qt.hsla(Math.random(), 1.0, 0.5, 1.0)
                    ambientReflectivity: 0.05
                    diffuseReflectivity: 0.9
                    specularReflectivity: 1.0
                    shininess: 100
                    reflectivity: 0.0
                }
            }
        }

        Component {
            id: mirrorSphereComponent
            Sphere {
                position: Qt.vector3d((Math.random() - 0.5) * 7.0, (Math.random() - 0.5) * 5.0 + 2.0, (Math.random() - 0.5) * 7.0)
                radius: Math.random() * 0.5 + 0.4
                material: Material {
                    color: "black"
                    ambientReflectivity: 0.00
                    diffuseReflectivity: 0.0
                    specularReflectivity: 1.0
                    shininess: 400
                    reflectivity: 0.8
                }
            }
        }

        PointLight {
            position: Qt.vector3d(0.0, whiteRoom.roomHeight - 1.0, -10.0)
            color: "white"
            intensity: 8.0
        }

        PointLight {
            position: Qt.vector3d(5.0, 6.0, -6.0)
            color: "white"
            intensity: 3.0
        }

        Camera {
            id: cam
            position: Qt.vector3d(6.0, 3.0, -13.0)
            lookAt: clump
            fov: 30
        }
    }
}
