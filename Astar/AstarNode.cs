using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AstarNode : TwoDGridNode<AstarNode>
{
    [SerializeField]
    public bool walkable;
    [SerializeField]
    public AstarNode parent;

    [SerializeField]
    public int gCost;
    [SerializeField]
    public int hCost;


    //any node requires at least a world position and a grid position and whether the node is walkable or not
    public AstarNode(bool _walkable, Vector2 _worldPos, Vector2Int _gridPosition, float _cellSize) :base(_gridPosition.x, _gridPosition.y, _worldPos, _cellSize)
    {
        this.walkable = _walkable;
        base.containedObject = this;
        base.CalculateBounds();
    }

    //returns the F cost aka the cost of getting to this node from the start node plus the cost of getting from this node to the destination node
    public int FCost()
    {
        return gCost + hCost;
    }

    //compares the F cost of a node to this node. It does so by comparing this nodes F value with the F value of the input node. (the compare returns 1 if the value is higher, 0 if equal, or -1 if lower)
    //if the values are equal it returns whatever the result is of comparing the heuristic value of this node to the heuristic value of the incoming node
    //finally it returns the inverse as it is being fed to the custom max heap algorithm where we want the lowest value to be on top (should be reworked to a min heap)
    public int CompareTo(AstarNode nodeToCompare)
    {
        int  compare= FCost().CompareTo(nodeToCompare.FCost());
        if(compare == 0)
        {
            compare = hCost.CompareTo(nodeToCompare.hCost);
        }

        return -compare;
    }
}
