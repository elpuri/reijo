import Reijo 1.0
import QtQuick 2.0

Scene {
    camera: cam
    renderer: whitted

    property bool mini : false


    Camera {
        id: cam
        position: Qt.vector3d(0.0, 2.0, -15.0)
        lookAt: red//Qt.vector3d(0.0, 0.0, 0.0)//red
        fov: 40
    }

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
        id: darkMirror
        reflectivity: 1.0
        diffuseReflectivity: 0.1
        specularReflectivity: 1.0
        shininess: 50
        color: "white"
    }

    Material {
        id: redDiffuse
        color: "#ff2222"
        ambientReflectivity: 0.00
        diffuseReflectivity: 1.0
        specularReflectivity: 0.8
        shininess: 100
        reflectivity: 1.0
    }

    Material {
        id: whiteDiffuse
        color: "white"
        ambientReflectivity: 0.05
        diffuseReflectivity: 0.95
        specularReflectivity: 1.0
    }


    Material {
        id: greyDiffuse
        color: "white"
        ambientReflectivity: 0.00
        diffuseReflectivity: 0.2
        specularReflectivity: 0.0
        reflectivity: 1.0
    }

/*
    Sphere {
        id: sphere
        objectName: "Parent sphere"
        position: Qt.vector3d(3.0, 2.0, 0.0)
        rotation: Qt.vector3d(0.0, 0.0, 30.0)
        radius: 2.0
        scale: Qt.vector3d(0.76, 1.0, 1.0)
        material: whiteDiffuse
    }
*/


    Sphere {
        id: red
        objectName: "Child sphere"
        position: Qt.vector3d(-2.5, radius, 3.5)
        radius: 4.0
        material: redDiffuse
        scale: Qt.vector3d(1.0, 1.0, 1.0)
    }

/*
    Sphere {
        position: Qt.vector3d(red.position.x + 2 * radius + 1.0, 2.0, -3.0)
        radius: 2.0
        material: darkMirror
        scale: Qt.vector3d(1.0, 1.0, 1.0)
    }
*/
    PointLight {
        position: Qt.vector3d(-4.0, 12.0, -12.0)
        color: "white"
        intensity: 1.0
    }
/*
    PointLight {
        position: Qt.vector3d(4.0, 5.0, -5.0)
        color: "red"
        intensity: 0.3
    }

    PointLight {
        position: Qt.vector3d(4.0, 10.0, 0.0)
        color: "blue"
        intensity: 0.3
    }
*/
    Plane {
        id: floor
        point: Qt.vector3d(0.0, 0.0, 0.0)
        normal: Qt.vector3d(0.0, 1.0, 0.0)
        material: greyDiffuse
    }

/*
    NullShape {
        id: clumpOfSpheres
        ShapeFactory {
            model: 2
            componentSelectorScript: { return compositeDelegate; }
        }

    }

    Component {
        id: compositeDelegate
        Sphere {
            position: Qt.vector3d(-5 + ShapeFactory.index * 10, 0.0, 0.0)
            radius: 1
            ShapeFactory {
                model: 2
                componentSelectorScript: { return sphereDelegate; }
            }
        }
    }

    Component {
        id: sphereDelegate
        Sphere {
            material: redDiffuse
            radius: ShapeFactory.index * (2 / (ShapeFactory.count - 1))
        }
    }
*/
}
