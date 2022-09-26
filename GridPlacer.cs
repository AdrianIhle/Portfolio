using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[ExecuteInEditMode]
public class GridPlacer : MonoBehaviour
{

    [SerializeField] IntGrid grid;

    [Header("Grid Definition Options")]
    [SerializeField] private Vector3 gridCenter;
    [SerializeField] private int width, height;
    [Tooltip("size of each cell in world space, used to determine rows and columns")]
    [SerializeField] private float cellSize = 1;
    [Tooltip("should the grid assung random values when created?")]
    [SerializeField] private bool randomValues = false;
    [Space]
    [Tooltip("should the array use the following transforms to define the grid in world space?")]
    [SerializeField] private bool useObjectsForBounds = false;
    [SerializeField] private Transform lowerLeftBound, upperRightBound;



    [Header("Debug")]
    [SerializeField] private bool drawNodeBounds;
    [SerializeField] private bool drawNodeGrid;
    [SerializeField] private bool drawNodeTemp;
    [SerializeField] private bool drawNodeValues;
    [SerializeField] private bool remakeGrid;

    [Space]
    IntNode currentNode;
    TwoDGridNode<IntNode>[] currentNeighbours = new TwoDGridNode<IntNode>[0];
    [SerializeField] private string currentNodeOutput;

    [Header("Debug")]
    [Tooltip("should the mouse click action only select the nearest neighbours or propagate based on propagation factor and profile?")]
    [SerializeField] private bool propagateMouseClick;
    [Tooltip("use circular selection pattern? default is square")]
    [SerializeField] private bool circularSelection;
    [Tooltip("how many nodes should a click propagate out?")]
    [SerializeField] private int propogationFactor;
    [Tooltip("determines the fall off of value changes based on distance from selected node")]
    [SerializeField] private AnimationCurve propagationProfile;
    [SerializeField] private int alterationValue = 1;
    [Tooltip("a mouse click defaults to adding the alteration value, toggle this to subtract")]
    [SerializeField] private bool subtract;


    // Start is called before the first frame update
    void Start()
    {
        if(useObjectsForBounds && lowerLeftBound != null && upperRightBound != null)
        {
            width = Mathf.FloorToInt(Math.Abs(upperRightBound.position.x) - Math.Abs(lowerLeftBound.position.x));
            height = Mathf.FloorToInt( Math.Abs(upperRightBound.position.z) - Math.Abs(lowerLeftBound.position.z));
        }

        grid = GetComponent<IntGrid>();
        grid.ReInitializeGrid(new Vector2(width, height), this.transform.position, cellSize, randomValues);
    }

    // Update is called once per frame
    void Update()
    {
        if(remakeGrid)
        {
            remakeGrid = false;
            if (useObjectsForBounds && lowerLeftBound != null && upperRightBound != null)
            {
                width = Mathf.FloorToInt(Math.Abs(upperRightBound.position.x) + Math.Abs(lowerLeftBound.position.x));
                height = Mathf.FloorToInt(Math.Abs(upperRightBound.position.z) + Math.Abs(lowerLeftBound.position.z));
                gridCenter = (upperRightBound.position+lowerLeftBound.position)/2;
                grid.ReInitializeGrid(new Vector2(width, height), this.transform.position, cellSize, randomValues);
            }
            else
            {
                grid.ReInitializeGrid(new Vector2(width, height), this.transform.position, cellSize, randomValues);
                lowerLeftBound.position = grid.BottomLeftWorld;
                upperRightBound.position = grid.TopLeftWorld;
            }


        }

        //targets a node as the center or an alteration action
        if(Input.GetMouseButtonDown(0))
        {
            Vector3 mousePos = Input.mousePosition;
            Vector3 worldPos = Camera.main.ScreenToWorldPoint(new Vector3(mousePos.x, mousePos.y, 10.0f));
            //print("mouse pos in world " + worldPos);

            IntNode selectedNode = grid.NodeFromWorldPoint(worldPos).ContainedObject;
            currentNode = selectedNode;
            if (selectedNode != null)
            {
                ProcessMouseClick(selectedNode);

            }
        }
        
    }

    private void ProcessMouseClick(IntNode selectedNode)
    {
        int baseValue = alterationValue;

        if (subtract)
            baseValue = -alterationValue;

        if (propagateMouseClick)
        {
            ProcessByPropagation(selectedNode, baseValue);
        }
        else
        {
            ProcessWithoutPropagation(selectedNode, baseValue);
        }
    }

    private void ProcessWithoutPropagation(IntNode selectedNode, int baseValue)
    {
        if (circularSelection)
        {
            int xDistanceTest = 0;

            if (selectedNode.WorldPosition.x > grid.WorldPositon.x)
                xDistanceTest = selectedNode.GridPosition.x - 1;
            else
                xDistanceTest = selectedNode.GridPosition.x + 1;

            IntNode testNode = grid.Grid[xDistanceTest, currentNode.GridPosition.y].ContainedObject;

            float maxDistance = Vector2.Distance(selectedNode.GridPosition, testNode.GridPosition);
            print($"selected node: {selectedNode.WorldPosition}, test node {testNode.WorldPosition}, distance {maxDistance}");

            currentNeighbours = grid.GetNeighbourNodesCircular(selectedNode, maxDistance).ToArray();

            foreach (IntNode n in currentNeighbours)
            {
                float distance = Vector3.Distance(selectedNode.WorldPosition, n.WorldPosition);
                float propagationIncrement = distance / maxDistance;
                float propagationMultiplier = propagationProfile.Evaluate(propagationIncrement);
                int value = Mathf.FloorToInt(baseValue * propagationMultiplier);

                n.AlterValueBy(value);
            }
        }
        else
        {
            selectedNode.AlterValueBy(baseValue);
        }
    }

    private void ProcessByPropagation(IntNode selectedNode, int baseValue)
    {
        selectedNode.AlterValueBy(baseValue);

        if (circularSelection)
        {
            int xDistanceTest = 0;

            if (selectedNode.WorldPosition.x > grid.WorldPositon.x)
                xDistanceTest = selectedNode.GridPosition.x - 1;
            else
                xDistanceTest = selectedNode.GridPosition.x + 1;

            IntNode testNode = grid.Grid[xDistanceTest, currentNode.GridPosition.y].ContainedObject;

            float maxDistance = Vector2.Distance(selectedNode.GridPosition, testNode.GridPosition);

            currentNeighbours = grid.GetPropogatedNeighbourNodesCircular(selectedNode, maxDistance, propogationFactor).ToArray();

            foreach (TwoDGridNode<IntNode> n in currentNeighbours)
            {
                IntNode node = n.ContainedObject;
                float distance = Vector3.Distance(selectedNode.WorldPosition, n.WorldPosition);
                float propagationIncrement = distance / maxDistance;
                float propagationMultiplier = propagationProfile.Evaluate(propagationIncrement);
                int value = Mathf.FloorToInt(baseValue * propagationMultiplier);

                node.AlterValueBy(value);
            }
        }
        else
        {
            currentNeighbours = grid.GetPropogatedNeighbours(selectedNode, propogationFactor).ToArray();
            float maxDistance = (float)Math.Sqrt(propogationFactor * propogationFactor * 2);
            foreach (IntNode n in currentNeighbours)
            {
                float distance = Vector3.Distance(selectedNode.WorldPosition, n.WorldPosition);
                float propagationIncrement = distance / maxDistance;
                float propagationMultiplier = propagationProfile.Evaluate(propagationIncrement);
                int value = Mathf.FloorToInt(baseValue * propagationMultiplier);

                n.AlterValueBy(value);
            }
        }
    }

    public int NodeCount()
    {
        return grid.NodeCount;
    }

    public Vector3[] GridBounds()
    {
        return grid.Bounds;
    }

    public IntGrid GetGrid()
    {
       return grid;
    }

    private void OnDrawGizmos()
    {

        if (lowerLeftBound != null)
        {
            Handles.color = (Color.red + Color.cyan) / 2;
            Handles.DrawWireDisc(lowerLeftBound.position, Vector3.up, 0.2f);
        }

        if (upperRightBound != null)
        {
            Handles.color = (Color.red + Color.blue) / 2;
            Handles.DrawWireDisc(upperRightBound.position, Vector3.up, 0.2f);
        }

        Color[] colors = new Color[] { Color.red, Color.green, Color.blue, Color.cyan };
        if (Application.isEditor && grid != null && grid.Grid.Length > 0)
        {
            foreach (TwoDGridNode<IntNode> nodeContainer in grid.Grid)
            {
                IntNode node = nodeContainer.ContainedObject;
                if(drawNodeValues)
                 Handles.Label(node.WorldPosition, node.GetValue().ToString());

                Vector3[] nodebounds = new Vector3[] { node.BottomLeft, node.BottomRight, node.TopRight, node.TopLeft };

                if (drawNodeBounds)
                {
                    int indi = 0;
                    foreach (Vector3 b in nodebounds)
                    {
                        Vector3 normal = Quaternion.Euler((90 / 4) * indi, 0, 0) * Vector3.up;
                        Handles.color = colors[indi];
                        Handles.DrawWireDisc(b, normal, 0.05f, 0.01f);
                        if (node.GridPosition.x == 0 && node.GridPosition.y == 0)
                        {
                            string boundName = "";

                            switch (indi)
                            {
                                case 0:
                                    boundName = "bottom Left";
                                    break;
                                case 1:
                                    boundName = "bottom Right";
                                    break;
                                case 2:
                                    boundName = "top Right";
                                    break;
                                case 3:
                                    boundName = "top Left";
                                    break;
                                default:
                                    break;


                            }
                           // Handles.Label(b - Vector3.forward * 0.1f, $"{b.x}, {b.y}, {b.z}");
                            Handles.Label(b, boundName);
                        }
                        indi++;
                    }
                }

                if(drawNodeGrid)
                {
                    Handles.color = Color.white;
                    if (node.GridPosition.x == 0)
                    {
                        //print($"node at x 0 with y {node.nodeInGridPosition.y}");
                        Handles.DrawLine(node.BottomLeft, grid.Grid[grid.GridColumns - 1, node.GridPosition.y].BottomLeft);
                    }
                    if (node.GridPosition.y == 0)
                    {
                        //print($"node at y 0 with x {node.nodeInGridPosition.y}");
                        Handles.DrawLine(node.BottomLeft, grid.Grid[node.GridPosition.x, grid.GridRows - 1].TopLeft);
                    }
                }

                if(drawNodeTemp)
                {
                    Color c = new Color(node.GetNormalizedValue() - 0.45f, 0.0f, 0.0f);
                    if (node.GetValue() < 0)
                    {
                        c = new Color(0.0f, 0.0f, 1 - (node.GetNormalizedValue() + 0.45f));
                    }
                    else if (node.GetValue() == 0)
                        c = Color.black;

                    c.a = 0.5f;

                    Handles.color = c;
                    Handles.DrawSolidDisc(node.WorldPosition, Vector3.up, node.Diameter / 2);
                }

            }
            
            Vector3[] bounds = new Vector3[] { grid.BottomLeftWorld, grid.TopLeftWorld, grid.TopRightWorld, grid.BottomRightWorld };

            //draws outer bounds
            Handles.color = Color.black;
            for (int i = 0; i < bounds.Length; i++)
            {
                Vector3 offset = Vector3.zero;
                int forwardIndex = i + 1;

                if (forwardIndex > bounds.Length - 1)
                    forwardIndex = 0;

                string boundName = "";

                switch (i)
                {
                    case 0:
                        boundName = "bottom Left";
                        offset = -Vector3.forward * 0.2f;
                        break;
                    case 1:
                        boundName = "top left";
                        offset = Vector3.forward * 0.2f;
                        break;
                    case 2:
                        boundName = "top right";
                        offset = Vector3.forward * 0.2f;
                        break;
                    case 3:
                        boundName = "bot right";
                        offset = -Vector3.forward * 0.2f;
                        break;
                    default:
                        break;


                }
                Handles.Label(bounds[i]+offset, boundName);

                Handles.DrawLine(bounds[i], bounds[forwardIndex]);
            }

            Handles.color = Color.cyan;
            Handles.Label(grid.BottomLeftWorld - Vector3.forward/2, grid.BottomLeftWorld.ToString());
            Handles.Label(grid.TopRightWorld+ Vector3.forward/2, grid.TopRightWorld.ToString());
        }

    }
}
