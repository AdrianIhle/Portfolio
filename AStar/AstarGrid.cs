using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.ExceptionServices;
using System.Threading.Tasks;
using UnityEngine;


//TO DO
// include a way to move the center of the grid
public class AstarGrid : TwoDGrid<AstarNode>
{
    [SerializeField]
    //a reference to all the layers that are not considered walkable
    public LayerMask unnwalkableLayers;

    public AstarGrid(Vector2 _worldSize, Vector2 _worldPosition, float _nodeRadius) : base(_worldSize, _worldPosition, _nodeRadius)
    {
        CreateNewGrid();
    }

    private void Awake()
    {
        //calculates values for grid generation
        nodeDiameter = nodeRadius * 2;
        worldPosition = this.transform.position;
        gridColumns = Mathf.RoundToInt(worldSize.x / nodeDiameter);
        gridRows = Mathf.RoundToInt(worldSize.y / nodeDiameter);
        nodeCount = GridSize;
        //triggers grid generation
        CreateNewGrid();
        

    }

    //returns a list of all nodes next to the input node, at right angles and diagonally
    public List<AstarNode> GetNeighbourNodes(AstarNode node)
    {
        List<AstarNode> neighbours = new List<AstarNode>();
        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y < 2; y++)
            {
                if (x == 0 && y == 0)
                    continue;

                int checkX = node.GridPosition.x +x;
                int checkY = node.GridPosition.y +y;

                //validates that the grid node coordinate is within the grid limits before attempting to add
                if(checkX >= 0 && checkX < gridColumns && checkY >= 0 && checkY < gridRows)
                {
                    neighbours.Add(grid[checkX, checkY]);
                }
            }
        }

        return neighbours;
    }

    //creates the grid by creating the 2D array, defining the world position to generate from, iterates in rows and columns, adding offsets from bottom left of grid according to iteration and node size
    // casts a generates a circle overlap checking for objects on an unwalkable layer. If the node contains such an object, it is designated as unwalkable
    protected override void CreateNewGrid()
    {
        Stopwatch sw = new Stopwatch();
        sw.Start();

        base.CreateNewGrid();
        FillGrid();
        //calling update walkablilty as physics can only be called on main thread
        UpdateWalkability();
        sw.Stop();
    }

    void FillGrid()
    {
        Parallel.For(0, gridColumns, column =>
        {
            for (int row = 0; row < GridRows; row++)
            {
                TwoDGridNode<AstarNode> node = base.grid[column, row];
                node.ContainedObject = new AstarNode(true, node.WorldPosition, node.GridPosition, node.Diameter / 2);
            }
        });
    }


    public override void RecreateGrid()
    {
        nodeDiameter = nodeRadius * 2;
        gridColumns = Mathf.RoundToInt(worldSize.x / nodeDiameter);
        gridRows = Mathf.RoundToInt(worldSize.y / nodeDiameter);
        nodeCount = GridSize;
        CreateNewGrid();
    }

    public void UpdateWalkability()
    {
        Stopwatch sw = new Stopwatch();
        sw.Start();

        for (int x = 0; x < gridColumns; x++)
        {
            for (int y = 0; y < gridRows; y++)
            {

                bool walkable = true;
                Collider[] hits = Physics.OverlapSphere(grid[x, y].WorldPosition, nodeRadius * 2.0f, unnwalkableLayers);
                if (hits.Length > 0)
                {
                    walkable = false;
                }
                grid[x, y].ContainedObject.walkable = walkable;
            }
        }

        sw.Stop();
        print($"walkablity update took {sw.ElapsedMilliseconds / 1000.0f} seconds");
    }

    public void InitializeGrid(Vector2 _gridSize, Vector2 _worldPosition, float _nodeRadius, LayerMask _unwalkable)
    {
        nodeDiameter = nodeRadius * 2;
        worldSize = _gridSize;
        worldPosition = _worldPosition;
        nodeRadius = _nodeRadius;
        unnwalkableLayers = _unwalkable;

        RecreateGrid();
    }

    //if in debug mode and in the editor and has a grid, draws a cube of a given color at each node according to their walkablilty
    protected override void OnDrawGizmos()
    {
#if UNITY_EDITOR

        if (grid != null && displayGizmos)
        {
            foreach (TwoDGridNode<AstarNode> node in grid)
            {
                AstarNode n = node.ContainedObject;
                Gizmos.color = (n.walkable) ? new Color(1, 1, 1, 0.3f) : new Color(1, 0, 0, 0.3f);
                Gizmos.DrawCube(node.WorldPosition, debugNodeSize * 0.95f /*Vector2.one * (nodeDiameter - 0.05f)*/);
            }
        }
#endif
    }

}

