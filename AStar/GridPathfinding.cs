using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEditor;
using UnityEngine;
using System.Diagnostics;

public class GridPathfinding : MonoBehaviour
{
    [SerializeField]
    AstarGrid agGrid;

    public bool debugMode;
    TwoDGridNode<AstarNode>[] drawThis;

    [SerializeField]
    LayerMask lmUnwalkableLayer;

    private void Awake()
    {
        //finds the  grid to pathfind on
        agGrid = GetComponent<AstarGrid>();
    }
    

    //uses the same algorithm as FindPath() to find a path, but discards potential nodes if it finds something unwalkable to collide with within 1.3 sizes of the tile.
    //currently this function is not in use, and uses a static 0.75f as unit size
    public void FindPathSized(PathRequest request, Action<PathResult> callback)
    {
        float unitSize = 0.75f;
        Stopwatch sw = new Stopwatch();
        sw.Start();

        Vector3[] waypoints = new Vector3[0];
        bool pathSuccess = false;

        List<AstarNode> alteredNodes = new List<AstarNode>();

        AstarNode startNode = agGrid.NodeFromWorldPoint(request.start);
        AstarNode targetNode = agGrid.NodeFromWorldPoint(request.end);


        if (startNode.walkable && targetNode.walkable)
        {
            Heap<TwoDGridNode<AstarNode>> openSet = new Heap<TwoDGridNode<AstarNode>>(agGrid.GridSize);
           // print("patfinding waypoint heap created " + sw.ElapsedMilliseconds + "ms");

            HashSet<AstarNode> closedSet = new HashSet<AstarNode>();
            //print("patfinding waypoint hash created " + sw.ElapsedMilliseconds + "ms");
            openSet.AddItem(startNode);
            //print("both start and end node are walkable");
            while (openSet.HeapSize() > 0)
            {
                AstarNode node = openSet.RemoveFirst();
                closedSet.Add(node);

                if (node == targetNode)
                {
                    pathSuccess = true;
                    sw.Stop();
                    break;
                }

                //print("patfinding waypoint before grid nav " + sw.ElapsedMilliseconds + "ms");


                foreach (AstarNode n in agGrid.GetNeighbourNodes(node))
                {
                    Collider[] hits = Physics.OverlapSphere(n.WorldPosition, unitSize * 1.3f, lmUnwalkableLayer);

                    foreach (Collider c in hits)
                    {
                        if(n.walkable)
                            alteredNodes.Add(n);
                        n.walkable = false;
                    }

                    if (!n.walkable || closedSet.Contains(n))
                    {
                        continue;
                    }

                    int newCostToNeighbour = node.gCost + GetDistance(node, n);
                    if (newCostToNeighbour < n.gCost || !openSet.Contains(n))
                    {
                        n.gCost = newCostToNeighbour;
                        n.hCost = GetDistance(n, targetNode);
                        n.parent = node;

                        if (!openSet.Contains(n))
                            openSet.AddItem(n);
                        else
                            openSet.UpdateItem(n);
                    }
/*<
                    if (sw.ElapsedMilliseconds % 30 == 0)
                        yield return null;*/
                }

                if (debugMode)
                {
                    //yield return new WaitForEndOfFrame();
                    drawThis = openSet.items;
                }
            }

            if (pathSuccess)
            {
                waypoints = RetracePath(startNode, targetNode);
                pathSuccess = waypoints.Length > 0;
            }
            //print("patfinding waypoint before node reset " + sw.ElapsedMilliseconds + "ms");
            foreach (AstarNode n in alteredNodes)
                n.walkable = true;
        }
        sw.Stop();
        //print("path found in " + sw.ElapsedMilliseconds + "ms");

        callback(new PathResult(waypoints, pathSuccess, request.callback));
    }

    //takes a path request with a a start and end, and a callback for results and finds a path if one is available
    public void FindPath(PathRequest request, Action<PathResult> callback)
    {
        //stopwatch funtionality to calculate path times
        Stopwatch sw = new Stopwatch();
        sw.Start();

        //initializes variables needed to return a callback
        Vector3[] waypoints = new Vector3[0];
        bool pathSuccess = false;

        //finds the relevant nodes from world position
        AstarNode startNode = agGrid.NodeFromWorldPoint(request.start);
        AstarNode targetNode = agGrid.NodeFromWorldPoint(request.end);

        //ensures that both start and end targets are walkable before calculating
        if (startNode.walkable && targetNode.walkable)
        {
            //generates a copy of the grid in a heap, open set is all the nodes currently under evaluation
            Heap<TwoDGridNode<AstarNode>> openSet = new Heap<TwoDGridNode<AstarNode>>(agGrid.GridSize);
            // print("patfinding waypoint heap created " + sw.ElapsedMilliseconds + "ms");

            //uses a built in hash set to contain the closed set of nodes. closed set is all nodes that have been evaluated
            HashSet<AstarNode> closedSet = new HashSet<AstarNode>();
            //print("patfinding waypoint hash created " + sw.ElapsedMilliseconds + "ms");

            //adds the start node to the open set as the first node to be evaluated
            openSet.AddItem(startNode);

            //now that open set has an item, removes said item, adds it to the closed set and uses the item to evaluate the next iteration of the path
            while (openSet.HeapSize() > 0)
            {
                AstarNode node = openSet.RemoveFirst();
                closedSet.Add(node);

                //if the destiantion is reached, end evaluation as successful
                if (node == targetNode)
                {
                    pathSuccess = true;
                    sw.Stop();
                    break;
                }

                //print("patfinding waypoint before grid nav " + sw.ElapsedMilliseconds + "ms");


                //gets all the neighbours of current node, if it is not walkable or already evaluated, skip.
                //if neighbour gets evaluated, checks if the neighbour is under evaluation or if the cost of getting to the neighbour is less than that of getting to the current node being evaluated. 
                //if either of those are true, assign the cost of getting to the neighbour as its new gCost (distance from start) and add the neighbour to the open set and set the neigbour as the parent of the current node
                    //if the neighbour is in the open set, update it instead
                foreach (AstarNode n in agGrid.GetNeighbourNodes(node))
                {

                    if (!n.walkable || closedSet.Contains(n))
                    {
                        continue;
                    }

                    int newCostToNeighbour = node.gCost + GetDistance(node, n);
                    if (newCostToNeighbour < n.gCost || !openSet.Contains(n))
                    {
                        n.gCost = newCostToNeighbour;
                        n.hCost = GetDistance(n, targetNode);
                        n.parent = node;

                        if (!openSet.Contains(n))
                            openSet.AddItem(n);
                        else
                            openSet.UpdateItem(n);
                    }
                    /*<
                                        if (sw.ElapsedMilliseconds % 30 == 0)
                                            yield return null;*/
                }

                //if in debug mode, assign all the currently evaluated nodes to be drawn
                if (debugMode)
                {
                    //yield return new WaitForEndOfFrame();
                    drawThis = openSet.items;
                }
            }

            //if the path has been found successfully, retraces the path as an array, and dboule checks that the path is successfully assigned by checking if waypoints is more than 0;
            if (pathSuccess)
            {
                waypoints = RetracePath(startNode, targetNode);
                pathSuccess = waypoints.Length > 0;
            }

            //print("patfinding waypoint before node reset " + sw.ElapsedMilliseconds + "ms");
        }
        sw.Stop();
        //print("path found in " + sw.ElapsedMilliseconds + "ms");

        //assigns the call back funtion as a path result with a refernece to the found path (even if null) the success of the pathfinding, and a reference to the callback to the requestor
        callback(new PathResult(waypoints, pathSuccess, request.callback));
    }


    //retraces the path assigned by the pathfinder by navigating the parent child relationship of all the nodes from the destination to the start node
    //as the path is in reverse, the array must be reversed before returned
    Vector3[] RetracePath(AstarNode startNode, AstarNode endNode)
    {
        List<AstarNode> path = new List<AstarNode>();
        AstarNode currentNode = endNode;

        while (currentNode != startNode)
        {
            path.Add(currentNode);
            currentNode = currentNode.parent;
        }

        if (currentNode == startNode)
            path.Add(currentNode);

        List<Vector3> waypoints = SimplyPath(path);
        waypoints.Reverse();

        // print("path contains " + path.Count + " nodes");

        return waypoints.ToArray();
    }

    //uses 'path smoothing' to return a shorter, more realistic path in world space cooridates, not node space
    //uses a psuedo ray to see if the path takes a turn or not. if it does, add the point to the path, if it doesnt, move on to the next one. 
    //specifically it uses vector direction comparison to see if a point is directly in the path between two other points, making it redundant
    List<Vector3> SimplyPath(List<AstarNode> path)
    {
        List<Vector3> waypoints = new List<Vector3>();

        Vector3 directionOld = Vector3.zero;

        for (int i = 1; i < path.Count; i++)
        {
            Vector3 directionNew = new Vector3(path[i - 1].GridPosition.x - path[i].GridPosition.y,
                path[i - 1].GridPosition.x - path[i].GridPosition.y);

            if (directionNew != directionOld)
            {
                waypoints.Add(path[i - 1].WorldPosition);
            }
            directionOld = directionNew;
        }

        return waypoints;
    }

    //get distance between two nodes 
    int GetDistance(AstarNode a, AstarNode b)
    {
        int distanceX = Mathf.Abs(a.GridPosition.x - b.GridPosition.x);
        int distanceY = Mathf.Abs(a.GridPosition.y - b.GridPosition.y);

        //depending on which distance is biggest, returns a value modified to roughly account for trignometry
        //only works for squares, nothing else, as the ratio of a hypotenuse for a square is 1.41
        if (distanceX > distanceY)
        {
            return 14 * distanceY + 10 * (distanceX - distanceY);
        }
        else
        {
            return 14 * distanceX + 10 * (distanceY - distanceX);
        }
    }


    // draws a found path with wire spheres
    private void OnDrawGizmos()
    {
        if (Application.isEditor)
        {
            if (drawThis != null && agGrid != null)
            {
                foreach (AstarNode a in drawThis)
                {
                    if (a != null)
                    {
                        Gizmos.DrawCube(a.WorldPosition, Vector2.one * (agGrid.NodeRadius*2 - 0.05f));
                        Gizmos.color = Color.red;
                        Gizmos.DrawWireSphere(a.WorldPosition, 0.6f * 1.05f);
                    }
                }
            }
        }
    }
}
