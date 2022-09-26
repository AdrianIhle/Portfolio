using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
using UnityEngine;


[System.Serializable]
public partial class IntGrid : TwoDGrid<IntNode>
{
    //fields for controlling the values of nodes
    [Tooltip("Fields for generating the values in the grid")]
    [Header("Value Limits")]
    [SerializeField] private int defaultValue = 0;
    [SerializeField] private int minValue = -256;
    [SerializeField] private int maxValue = 256;
    [SerializeField] private bool assignRandomValues;
    [SerializeField] private bool createGrid;

    public IntGrid(Vector3 _worldPosition, Vector2 _worldSize, float _nodeDiameter, bool _assignRandomValues = false) : base(_worldSize, _worldPosition, _nodeDiameter)
    {

        assignRandomValues = _assignRandomValues;
        CreateNewGrid();
    }

    private void Update()
    {
        if(createGrid)
        {
            createGrid = false;
            CreateNewGrid();

        }
    }

    protected override void OnDrawGizmos()
    {
        if (displayGizmos)
        {
            foreach (TwoDGridNode<IntNode> node in base.grid)
            {
               // UnityEditor.Handles.Label(node.WorldPosition, $"{node.ContainedObject.GetValue().ToString()}, {node.ContainedObject.GetNormalizedValue()}");
                //UnityEditor.Handles.Label(node.WorldPosition, $"{node.ContainedObject.GetValue().ToString()}");
                UnityEditor.Handles.Label(node.WorldPosition + Vector3.up, $"{node.ContainedObject.GetNormalizedValue().ToString("0.000")}");
            }

            base.OnDrawGizmos();
        }
    }
    protected override void CreateNewGrid()
    {
        base.CreateNewGrid();
        FillGrid();
    }

    public void ReInitializeGrid(Vector2 _worldSize, Vector2 _worldPosition, float _radius, bool _randomValues)
    {
        base.worldSize = _worldSize;
        base.worldPosition = _worldPosition;
        base.nodeRadius = _radius;
        assignRandomValues = _randomValues;
        RecreateGrid();
    }

    public override void RecreateGrid()
    {
        CreateNewGrid();
    }

    void FillGrid()
    {
        if (assignRandomValues)
        {
            System.Random rand = new System.Random();
            Parallel.For(0, gridColumns, column =>
            {
                for (int row = 0; row < GridRows; row++)
                {
                    TwoDGridNode<IntNode> node = base.grid[column, row];
                    node.ContainedObject = new IntNode(node.GridPosition.x, node.GridPosition.y, node.WorldPosition, node.Diameter, rand.Next(minValue, maxValue), minValue, maxValue);

                }

            });
        }
        else
        {
            Parallel.For(0, gridColumns, column =>
            {
                for (int row = 0; row < GridRows; row++)
                {
                    TwoDGridNode<IntNode> node = base.grid[column, row];
                    node.ContainedObject = new IntNode(node.GridPosition.x, node.GridPosition.y, node.WorldPosition, node.Diameter, defaultValue, minValue, maxValue);
                }

            });
        }
    }


    //Returns the array values as greyscale color
    public Color[] GetGridAsGreyScale()
    {
        List<Color> pixels = new List<Color>();

        for(int i = 0; i < grid.GetLength(0);i++)
        {
            for(int j = 0; j < grid.GetLength(1); j++)
            {
                pixels.Add(new Color(grid[i, j].ContainedObject.GetNormalizedValue(), grid[i, j].ContainedObject.GetNormalizedValue(), grid[i, j].ContainedObject.GetNormalizedValue()));
            }
        }
        return pixels.ToArray();
    }

    public Color[,] GetGridAsGreyScale2D()
    {
        Color[,] pixels = new Color[grid.GetLength(0), grid.GetLength(1)];

        for (int i = 0; i < grid.GetLength(0); i++)
        {
            for (int j = 0; j < grid.GetLength(1); j++)
            {
                pixels[i,j] = (new Color(grid[i, j].ContainedObject.GetNormalizedValue(), grid[i, j].ContainedObject.GetNormalizedValue(), grid[i, j].ContainedObject.GetNormalizedValue()));
            }
        }
        return pixels;
    }

}
