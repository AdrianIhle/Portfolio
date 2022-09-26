using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
using UnityEngine;

[System.Serializable]
public class TwoDGrid<T> : MonoBehaviour
{
    #region FIELDS
    [Tooltip("Size of the grid in world space")]
    [SerializeField] protected Vector2 worldSize;
    [Tooltip("Position of grid center in world space")]
    [SerializeField] protected Vector3 worldPosition;
    [Tooltip("how large a node is. Will be used to derive number of colums and rows")]
    [SerializeField] protected float nodeRadius;
    [Tooltip("total number of nodes in the grid")]
    [SerializeField] protected int nodeCount = 0;
    //quick reference to the total width of a node for generation purposes
    protected float nodeDiameter;
    //quick reference to number of rows and columns generated
    protected int gridRows, gridColumns;

    protected TwoDGridNode<T>[,] grid;

    //references to all the points that make up the 'geometry' of the grid
    protected Vector3 bottomLeftWorld;
    protected Vector3 topRightWorld;
    protected Vector3 bottomRightWorld;
    protected Vector3 topLeftWorld;
    protected Vector2 midpointWorld;
    //quick reference for the edges of the grid easy use in for loops
    protected Vector3[] bounds;

    [Header("Debugging")]
    [SerializeField] protected bool displayGizmos;
    [SerializeField] protected Vector3 debugNodeSize;
    #endregion

    #region PROPERTIES
    public Vector2 WorldSize { get => worldSize; }
    public Vector3 WorldPositon { get => worldPosition; }
    public Vector3 BottomLeftWorld { get => bottomLeftWorld; }
    public Vector3 TopRightWorld { get => topRightWorld; }
    public Vector3 BottomRightWorld { get => bottomRightWorld; }
    public Vector3 TopLeftWorld { get => topLeftWorld; }
    public Vector2 MidpointWorld { get => midpointWorld; }
    public Vector3[] Bounds { get => bounds; }
    public int GridSize { get => gridRows * gridColumns; }
    public TwoDGridNode<T>[,] Grid { get => grid; }
    public int NodeCount { get => nodeCount; }
    public int GridRows { get => gridRows; }
    public int GridColumns { get => gridColumns; }
    public float NodeRadius { get => nodeRadius; }
    #endregion

    public TwoDGrid(Vector2 _worldSize, Vector2 _worldPosition, float _nodeRadius)
    {
        worldSize = _worldSize;
        worldPosition = _worldPosition;
        nodeRadius = _nodeRadius;
        nodeDiameter = nodeRadius * 2;
        gridColumns = Mathf.RoundToInt(worldSize.x / nodeDiameter);
        gridRows = Mathf.RoundToInt(worldSize.y / nodeDiameter);
        nodeCount = GridSize;
        CreateNewGrid();

    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    protected virtual void OnDrawGizmos()
    {
#if UNITY_EDITOR

        if (grid != null && displayGizmos)
        {
            foreach (TwoDGridNode<T> n in grid)
            {

                Gizmos.color = n.DebugColor;
                Gizmos.DrawCube(n.WorldPosition, debugNodeSize * 0.95f);
                Gizmos.color = new Color(0, 1, 0, 0.3f);
                Gizmos.DrawLine(n.WorldPosition, n.WorldPosition + new Vector3(0, debugNodeSize.magnitude / 2));
                Gizmos.color = new Color(0, 0, 1, 0.3f);
                Gizmos.DrawSphere(n.WorldPosition, 0.05f);
            }
        }
        if (displayGizmos)
        {
            Gizmos.color = Color.blue;
            Gizmos.DrawSphere(bottomLeftWorld, 0.2f);
            Gizmos.color = Color.cyan;
            Gizmos.DrawSphere(topRightWorld, 0.2f);
            Gizmos.color = Color.magenta;
            Gizmos.DrawSphere(midpointWorld, 0.2f);
            Gizmos.DrawWireCube(midpointWorld, new Vector3(worldSize.x,0, worldSize.y));
        }
#endif
    }

    protected virtual void CreateNewGrid()
    {
        nodeDiameter = nodeRadius * 2;
        worldPosition = this.transform.position;
        gridColumns = Mathf.RoundToInt(worldSize.x / nodeDiameter);
        gridRows = Mathf.RoundToInt(worldSize.y / nodeDiameter);
        nodeCount = GridSize;

        grid = null;
        grid = new TwoDGridNode<T>[gridColumns, gridRows];

        //defines grid limits
        bottomLeftWorld = new Vector3(worldPosition.x - (worldSize.x / 2), worldPosition.y, worldPosition.z - (worldSize.y) / 2);
        topRightWorld = bottomLeftWorld + new Vector3(worldSize.x, worldPosition.y, worldSize.y);
        bottomRightWorld = new Vector3(topRightWorld.x, worldPosition.y, bottomLeftWorld.z);
        topLeftWorld = new Vector3(bottomLeftWorld.x, worldPosition.y, topRightWorld.z);
        midpointWorld = (bottomLeftWorld + topRightWorld) / 2;

        //easy reference array to iterate over bounds
        bounds = new Vector3[] { bottomLeftWorld, topLeftWorld, topRightWorld, bottomRightWorld };

        //calculate size of node to allow non-uniform sizes
        float nodeColumnSize = worldSize.x / gridColumns;
        float nodeRowSize = worldSize.y / gridRows;
        Vector2 nodeSize = new Vector2(nodeColumnSize, nodeRowSize);

        //Fills the array
        int index = 0;
        for (int x = 0; x < gridColumns; x++)
        {
            for (int y = 0; y < gridRows; y++)
            {
                index++;

                Vector3 xOffset = Vector3.right * (x * nodeSize.x + (nodeSize.x / 2));
                Vector3 yOffset = Vector3.forward * (y * nodeSize.y + (nodeSize.y / 2));
                Vector3 nodePosition = bottomLeftWorld + xOffset + yOffset;
                grid[x, y] = new TwoDGridNode<T>(x, y, new Vector3(nodePosition.x, worldPosition.y, nodePosition.z), nodeRadius * 2);
            }
        }

        debugNodeSize = new Vector3(nodeDiameter,0.1f, nodeDiameter);
    }

    public virtual void RecreateGrid()
    {
        nodeDiameter = nodeRadius * 2;
        gridColumns = Mathf.RoundToInt(worldSize.x / nodeDiameter);
        gridRows = Mathf.RoundToInt(worldSize.y / nodeDiameter);
        nodeCount = GridSize;
        CreateNewGrid();
    }

    //returns a list of all nodes next to the input node, at right angles and diagonally
    public List<TwoDGridNode<T>> GetNeighbourNodes(TwoDGridNode<T> node)
    {
        List<TwoDGridNode<T>> neighbours = new List<TwoDGridNode<T>>();
        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y < 2; y++)
            {
                if (x == 0 && y == 0)
                    continue;

                int checkX = node.GridPosition.x + x;
                int checkY = node.GridPosition.y + y;

                //validates that the grid node coordinate is within the grid limits before attempting to add
                if (checkX >= 0 && checkX < gridColumns && checkY >= 0 && checkY < gridRows)
                {
                    neighbours.Add(grid[checkX, checkY]);
                }
            }
        }
        return neighbours;
    }

    //returns the neighbours within a certain level of propagation, i.e a number of nodes away
    public List<TwoDGridNode<T>> GetPropogatedNeighbours(TwoDGridNode<T> centerNode, int propagation)
    {
        List<TwoDGridNode<T>> results = new List<TwoDGridNode<T>>();

        if (propagation >= 1)
        {
            List<TwoDGridNode<T>> initialNeighbours = GetNeighbourNodes(centerNode);

            results.AddRange(initialNeighbours);

            foreach (TwoDGridNode<T> n in initialNeighbours)
            {
                List<TwoDGridNode<T>> resultantNeighbours = GetPropogatedNeighbours(n, propagation - 1);

                foreach (TwoDGridNode<T> m in resultantNeighbours)
                {
                    if (!results.Contains(m))
                    {
                        results.Add(m);
                    }
                }
            }
            return results;
        }
        else
            return GetNeighbourNodes(centerNode);
    }

    //returns a neighbours contained within a certain radius of the center of the input node
    public List<TwoDGridNode<T>> GetNeighbourNodesCircular(TwoDGridNode<T> node, float radius)
    {
        List<TwoDGridNode<T>> neighbours = new List<TwoDGridNode<T>>();

        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y < 2; y++)
            {
                if (x == 0 && y == 0)
                    continue;

                int checkX = node.GridPosition.x + x;
                int checkY = node.GridPosition.y + y;

                //validates that the grid node coordinate is within the grid limits before attempting to add
                if (checkX >= 0 && checkX < gridColumns && checkY >= 0 && checkY < gridRows)
                {
                    //checks the node is within the max radius
                    if (Vector2.Distance(node.GridPosition, grid[checkX, checkY].GridPosition) <= radius)
                    {
                        neighbours.Add(grid[checkX, checkY]);
                    }
                }
            }
        }
        return neighbours;
    }

    //Gets neighbours contained with in a certain radius of the nodes X nodes away (X being the propagation number)
    public List<TwoDGridNode<T>> GetPropogatedNeighbourNodesCircular(TwoDGridNode<T> node, float radius, int propagation)
    {
        List<TwoDGridNode<T>> results = new List<TwoDGridNode<T>>();

        if (propagation >= 1)
        {
            List<TwoDGridNode<T>> initialNeighbours = GetNeighbourNodesCircular(node, radius * propagation);
            results.AddRange(initialNeighbours);

            foreach (TwoDGridNode<T> n in initialNeighbours)
            {
                List<TwoDGridNode<T>> resultantNeighbours = GetPropogatedNeighbourNodesCircular(n, radius, propagation - 1);

                foreach (TwoDGridNode<T> m in resultantNeighbours)
                {
                    if (!results.Contains(m))
                    {
                        results.Add(m);
                    }
                }
            }
            return results;
        }
        else
            return GetNeighbourNodesCircular(node, radius);
    }

    //returns a node from a world point by converting the point into a percentage 
    //of width and height and converts that to rows and column coordinates 
    //(aka array indicies)
    public TwoDGridNode<T> NodeFromWorldPoint(Vector3 point)
    {
        float percentX = (point.x + worldSize.x / 2) / worldSize.x;
        float percentY = (point.z + worldSize.y / 2) / worldSize.y;
        if (percentX >= 0.0f && percentX <= 1.0f && percentY >= 0.0f && percentY <= 1.0f)
        {
            int x = Mathf.FloorToInt((gridColumns) * percentX);
            int y = Mathf.FloorToInt((gridRows) * percentY);

            return grid[x, y];
        }
        else
            return null;
    }
}
