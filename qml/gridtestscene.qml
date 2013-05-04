import Reijo 1.0
import QtQuick 2.0

Scene {
    id: scene
    camera: cam
    renderer: whitted
    time: 4.0
    duration: 15
    frameTime: 1 / 30

    property bool mini : false

/*
    VideoEncoder {
        renderer: whitted
        filename: "gridtest2.mp4"
        bitrate: 2000000
    }
*/
    WhittedRenderer {
        id: whitted
        renderedWidth: mini ? 40 : 400
        renderedHeight: mini ? 20 : 300
        maxRecursionDepth: 3
        ambientLightColor: Qt.vector3d(1.0, 1.0, 1.0)
        antiAliasing: WhittedRenderer.JitteredSamples
        samplesPerPixel: 1
    }

    HdrViewer {
        id: viewer
        renderer: whitted
        gamma: 2.0
        exposure: 0.8
    }

    Material {
        id: redMaterial
        color: "#ff0000"
        ambientReflectivity: 0.1
        diffuseReflectivity: 1.0
        specularReflectivity: 1.0
        shininess: 150
        reflectivity: 0.0
    }
    Material {
        id: blueMaterial
        color: "#0000ff"
        ambientReflectivity: 0.1
        diffuseReflectivity: 1.0
        specularReflectivity: 1.0
        shininess: 150
        reflectivity: 0.0
    }

    Camera {
        id: cam
//        position: Qt.vector3d(10.0, 15.0, 2.0)
        position: Qt.vector3d(0.0, 9.0, -40.0)
        lookAt: gridFactory
//        up: Qt.vector3d(1.0, 0.0, 0.0)
        fov: 40
    }


    ShapeFactory {
        id: gridFactory
        property int gridDim : 4
        function totalElements() { return gridDim * gridDim * gridDim }
        position: Qt.vector3d(-3.0, 4.0, 0.0)
        rotation: Qt.vector3d(0.0, time * 360.0 / 10.0, time * 20.0)
//        rotation: Qt.vector3d(270.0, 0, 45.0)
        scale: Qt.vector3d(0.8, 0.8, 0.8)
        model: totalElements()
        componentSelectorScript: { return sphereComponent; }
    }

    Component {
        id: sphereComponent
        Sphere {
            id: sphere
            property int xIndex : ShapeFactory.index % gridFactory.gridDim
            property int yIndex : Math.floor(ShapeFactory.index / gridFactory.gridDim) % gridFactory.gridDim
            property int zIndex : Math.floor(ShapeFactory.index / (gridFactory.gridDim * gridFactory.gridDim))
            objectName: "gridsphere " + xIndex + " " + yIndex + " " + zIndex
            position: Qt.vector3d((xIndex - gridFactory.gridDim) * 2 - 5.0, yIndex * 2 - 5.0, zIndex * 2 - 5.0)
//            scale: Qt.vector3d(2.0, 1.0, 1.0)
            radius: 0.9
            material: Material {
                color: Qt.hsla(sphere.ShapeFactory.index / gridFactory.totalElements(), 1.0, 0.5, 1.0)
                diffuseReflectivity: 0.8
                ambientReflectivity: 0.05
                specularReflectivity: 10.0
                shininess: 250
                reflectivity: 0.0
            }
        }
    }
/*
    Room {
        id: whiteRoom
        roomWidth: 15.0
        roomHeight: 12.0
        roomDepth: 16.0
        rearWallVisible: false
        rightWallVisible: false
    }
*/
/*
    Sphere {
        objectName: "leftSphere"
        position: Qt.vector3d(0.0, 0.0, 0.0)

        radius: 2.0
        material: redMaterial
    }


    Sphere {
        objectName: "rightSphere"
        position: Qt.vector3d(4.0, 0.0, 0.0)
        radius: 2.0
        material: blueMaterial
    }
*/
    Material {
        id: glass
        color: "#aaeeff"
        ambientReflectivity: 0.0
        specularReflectivity: 1.0
        diffuseReflectivity: 0.4
        shininess: 250
        reflectivity: 0.0
        transmittance: 0.5
        indexOfRefraction: 1.0
    }

    PointLight {
        position: Qt.vector3d(-10.0, 3.0, -20.0)
        color: "white"
        intensity: 28.0
    }
}
