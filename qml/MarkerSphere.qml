import QtQuick 2.0
import Reijo 1.0

Sphere {
    property alias color : material.color
    radius: 1.0
    material: Material {
        id: material
        color: "red"
        diffuseReflectivity: 0.8
        ambientReflectivity: 0.1
    }
}
