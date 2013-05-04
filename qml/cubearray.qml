import Reijo 1.0
import QtQuick 2.0

Scene {
    id: scene
    camera: cam
    renderer: whitted
    duration: 15.0
    time: 0
    frameTime: 1 / 30.0     // 30fps

    property bool mini : true


    VideoEncoder {
        renderer: whitted
        filename: "cubismo_hd3.mp4"
        bitrate: 6000000
        fps: 30
        autoStartPlayer: true
    }


    WhittedRenderer {
        id: whitted
        renderedWidth: mini ? 320 : 800
        renderedHeight: mini ? 240 : 480
        maxRecursionDepth: 4
        ambientLightColor: Qt.vector3d(1.0, 1.0, 1.0)
        antiAliasing: WhittedRenderer.JitteredSamples
        samplesPerPixel: 4
        viewer: viewer
    }

    HdrViewer {
        id: viewer
        renderer: whitted
        gamma: 2.2
        exposure: 0.6
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
        roomWidth: 32.0
        roomHeight: 12.0
        roomDepth: 40.0
        rearWallVisible: false
        rightWallVisible: false
    }

    ShapeFactory {
        id: clump
        position: Qt.vector3d(-3.0, 5.0, 0.0)
        rotation: Qt.vector3d(0.0, time * 360.0 / 10.0, 45.0)
        scale: Qt.vector3d(0.8, 0.8, 0.8)
        model: 64
        componentSelectorScript: cubeLayerComponent;
    }


    Sphere {
        radius: 1.5
        position: Qt.vector3d(Math.cos(time / 15.0 * Math.PI * 5) * 2.0, 4.61 + Math.sin(time / 15.0 * Math.PI * 3) * 2.0 , -7.0 + Math.sin(time / 15.0 * Math.PI * 5) * 2.0)
        material: Material {
            reflectivity: 0.05
            transmittance: 1.0
            color: "white"
            indexOfRefraction: 1.5
        }
    }

    Camera {
        id: cam
        position: Qt.vector3d(4.5, 5.0, -18.0)
        lookAt: Qt.vector3d(-1.5, 5.0, 0.0)
        fov: 28
    }

    Component {
        id: cubeLayerComponent
        Box {
            id: box
            property int xIndex : ShapeFactory.index % 4
            property int yIndex : Math.floor(ShapeFactory.index / 4) % 4
            property int zIndex : Math.floor(ShapeFactory.index / 16)

            property real xPhase : xIndex * 20//Math.random() * 360.0
            property real yPhase : yIndex * 20//Math.random() * 360.0
            property real zPhase : zIndex * 20//Math.random() * 360.0

            position: Qt.vector3d(xIndex * 2 - 3.0, yIndex * 2 - 3.0, zIndex * 2 - 3.0)
            rotation: Qt.vector3d(time * 40.0 + xPhase, time * 40.0 + yPhase, time * 40.0 + zPhase)
            scale: Qt.vector3d(0.6, 0.6, 0.6)
            material: Material {
                color: Qt.hsla(box.ShapeFactory.index / 32.0 % 1.0 , 1.0, 0.5, 1.0)
                diffuseReflectivity: 0.8
                ambientReflectivity: 0.05
                specularReflectivity: 10.0
                shininess: 250
            }
        }
    }

    PointLight {
        position: Qt.vector3d(5.0, 11.0, -15.0)
        color: "white"
        intensity: 16.0
    }

    PointLight {
        position: Qt.vector3d(-3.0, 7.0, -10.0)
        color: "white"
        intensity: 18.0
    }
}
