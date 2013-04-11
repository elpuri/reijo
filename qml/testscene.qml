import Reijo 1.0
import QtQuick 2.0

Scene {
    camera: cam
    renderer: whitted

    property bool mini : false



    WhittedRenderer {
        id: whitted
        renderedWidth: mini ? 20 : 400
        renderedHeight: mini ? 20 : 300
        maxRecursionDepth: 5
        ambientLightColor: Qt.vector3d(1.0, 1.0, 1.0)
    }

    HdrViewer {
        id: viewer
        renderer: whitted
    }

    Material {
        id: redMaterial
        color: "#ff2222"
        ambientReflectivity: 0.08
        diffuseReflectivity: 1.0
        specularReflectivity: 0.8
        shininess: 100
        reflectivity: 0.1
    }

    Room {
        id: whiteRoom
        roomWidth: 15.0
        roomHeight: 12.0
        roomDepth: 16.0

        Sphere {
            id: sphere
            position: Qt.vector3d(0, radius, 0.0)
            radius: 3.0
            material: redMaterial
            scale: Qt.vector3d(1.0, 1.0, 1.0)
        }

        PointLight {
            position: Qt.vector3d(0.0, whiteRoom.roomHeight - 1.0, -6.0)
            color: "white"
            intensity: 1.0
        }

        Camera {
            id: cam
            position: Qt.vector3d(4.0, 3.0, whiteRoom.roomDepth / -2.1)
            lookAt: sphere//Qt.vector3d(0.0, 3.0, 0.0)//red
           // up: Qt.vector3d(-0.2, 1.0, 0.0);
            fov: 60
        }
    }
}
