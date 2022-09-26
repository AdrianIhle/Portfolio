using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class TwoDGridNode<T> : IHeapItem<TwoDGridNode<T>>
{
    #region FIELDS
    [SerializeField] protected Vector2Int gridPosition;
    [SerializeField] protected Vector3 worldPosition;
    [SerializeField] protected float diameter;
    [SerializeField] protected float radius;
    [SerializeField] protected Vector3 bottomLeft;
    [SerializeField] protected Vector3 bottomRight;
    [SerializeField] protected Vector3 topRight;
    [SerializeField] protected Vector3 topLeft;
    [SerializeField] protected T containedObject;
    [SerializeField] protected Color debugColor = Color.gray;
    #endregion

    #region PROPERTIES
    public T ContainedObject { get => containedObject; set => containedObject =value; }
    public Vector2Int GridPosition { get => gridPosition; }
    public Vector3 WorldPosition { get => worldPosition; }
    public float Diameter { get => diameter; }
    public float Radius { get => radius; }
    public Vector3 BottomLeft { get => bottomLeft; }
    public Vector3 BottomRight { get => bottomRight; }
    public Vector3 TopRight { get => topRight; }
    public Vector3 TopLeft { get => topLeft; }
    public Color DebugColor { get => debugColor; }
    public int HeapIndex { get => throw new System.NotImplementedException(); set => throw new System.NotImplementedException(); }
    #endregion

    public TwoDGridNode(int _gridX, int _gridY, Vector3 _worldPosition, float _diameter, T _value = default)
    {
        gridPosition = new Vector2Int(_gridX, _gridY);
        worldPosition = _worldPosition;
        diameter = _diameter;
        radius = _diameter / 2;
        containedObject = _value;
        CalculateBounds();
    }

    protected void CalculateBounds()
    {
        bottomLeft = new Vector3(worldPosition.x - (diameter / 2), worldPosition.y, worldPosition.z - (diameter / 2));
        topRight = new Vector3(worldPosition.x + (diameter / 2), worldPosition.y, worldPosition.z + (diameter / 2));
        topLeft = new Vector3(bottomLeft.x, worldPosition.y, topRight.z);
        bottomRight = new Vector3(topRight.x, worldPosition.y, bottomLeft.z);
    }

    public T GetItem()
    {
        return containedObject;
    }

    //using sqrMagnitude for quicker computation as all we care about is which node is closer to origin
    public virtual int CompareTo(TwoDGridNode<T> other)
    {
        return worldPosition.sqrMagnitude.CompareTo(other.worldPosition.sqrMagnitude);
    }

    public override string ToString()
    {
        return ($"index: {gridPosition}, position: {worldPosition}");
    }

    public static implicit operator T(TwoDGridNode<T> n) => n.containedObject;
}
