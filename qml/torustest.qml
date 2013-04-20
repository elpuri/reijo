import Reijo 1.0
import QtQuick 2.0

Scene {
    camera: cam
    renderer: whitted

    property bool mini : false

    WhittedRenderer {
        id: whitted
        renderedWidth: mini ? 40 : 640
        renderedHeight: mini ? 40 : 360
        maxRecursionDepth: 5
        ambientLightColor: Qt.vector3d(1.0, 1.0, 1.0)
        antiAliasing: WhittedRenderer.JitteredSamples
        samplesPerPixel: 15
    }

    HdrViewer {
        id: viewer
        renderer: whitted
        gamma: 2.2
        exposure: 0.6
    }



    Camera {
        id: cam
        position: Qt.vector3d(6.0, 3.0, -13.0)
        lookAt: torus
        fov: 35
    }


    Room {
        id: whiteRoom
        roomWidth: 15.0
        roomHeight: 12.0
        roomDepth: 16.0
        rearWallVisible: false
        rightWallVisible: false

        Sphere {
            radius: 2.0
            visible: false
            material: Material {
                color: "#ff2222"
                ambientReflectivity: 0.08
                diffuseReflectivity: 1.0
                specularReflectivity: 0.8
                shininess: 100
                reflectivity: 0.0
            }
        }

        Torus {
            id: torus
            innerRadius: 1.0
            outerRadius: 2.0
            position: Qt.vector3d(0.0, 4.0, -3.0)
            rotation: Qt.vector3d(45.0, 0.0, 45.0)
            material: Material {
                color: "#ff2222"
                ambientReflectivity: 0.00
                diffuseReflectivity: 1.0
                specularReflectivity: 1.0
                shininess: 50
                reflectivity: 0.0
            }
        }

        PointLight {
            position: Qt.vector3d(5.0, 6.0, -6.0)
            color: "white"
            intensity: 3.0
        }

    }
}
