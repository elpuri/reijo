import QtQuick 2.0
import Reijo 1.0

Sphere {
    id: root
    property int levels: 3
    property real radiusMultiplier : 0.5
    property real positionMultiplier : 1.5
    radius: 1.0

    Component {
        id: flakeLeaf
        Sphere {
            material: root.material
        }
    }

    function addLeaf(level, parentNode, d) {
        var leaf = flakeLeaf.createObject()
        leaf.radius = parentNode.radius * root.radiusMultiplier
        leaf.position = Qt.vector3d(d.x * positionMultiplier * parentNode.radius,
                                    d.y * positionMultiplier * parentNode.radius,
                                    d.z * positionMultiplier * parentNode.radius)
        parentNode.addNode(leaf)

        if (level + 1 > root.levels)
            return

        if (d !== -Qt.vector3d(1, 0, 0)) addLeaf(level + 1, leaf, Qt.vector3d(1, 0, 0));
        if (d !== -Qt.vector3d(-1, 0, 0)) addLeaf(level + 1, leaf, Qt.vector3d(-1, 0, 0));
        if (d !== -Qt.vector3d(0, 1, 0)) addLeaf(level + 1, leaf, Qt.vector3d(0, 1, 0));
        if (d !== -Qt.vector3d(0, -1, 0)) addLeaf(level + 1, leaf, Qt.vector3d(0, -1, 0));
        if (d !== -Qt.vector3d(0, 0, 1)) addLeaf(level + 1, leaf, Qt.vector3d(0, 0, 1));
        if (d !== -Qt.vector3d(0, 0, -1)) addLeaf(level + 1, leaf, Qt.vector3d(0, 0, -1));
    }

    Component.onCompleted: {
        addLeaf(1, root, Qt.vector3d(1, 0, 0));
        addLeaf(1, root, Qt.vector3d(-1, 0, 0));
        addLeaf(1, root, Qt.vector3d(0, 1, 0));
        addLeaf(1, root, Qt.vector3d(0, -1, 0));
        addLeaf(1, root, Qt.vector3d(0, 0, 1));
        addLeaf(1, root, Qt.vector3d(0, 0, -1));
    }
}
