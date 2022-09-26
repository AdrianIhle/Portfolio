using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;

public class PathRequestManager : MonoBehaviour
{

    //Queue<PathRequest> pathRequestQueue = new Queue<PathRequest>();
    Queue<PathResult> results = new Queue<PathResult>();

    //PathRequest currentRequest;

    static PathRequestManager instance;
    //[SerializeField]
    GridPathfinding pathfinder;
    //[SerializeField]
    //bool isProcessingPath;

    //creates a static reference to itself for easy reference by other scripts 
    //find the pathfinder script
    private void Awake()
    {
        instance = this;
        pathfinder = GetComponent<GridPathfinding>();
    }

    //if there are results to process, locks the results list, processes all the results in turn, dequeuing them, and triggers the call back to the requestor. 
    private void Update()
    {
        if (results.Count > 0)
        {
            int itemsInQueue = results.Count;
            lock (results)
            {
                for (int i = 0; i < itemsInQueue; i++)
                {
                    PathResult result = results.Dequeue();
                    result.callback(result.path, result.success);
                }
            }
        }
    }

    //starts a thread to process a pathing attempt using the GridPathfinding class with a callback to quoue the result for processing
    public static void RequestPath(PathRequest request)
    {
        ThreadStart threadStart = delegate
        {
            instance.pathfinder.FindPath(request, instance.FinishProcessingPath);
        };

        threadStart.Invoke();
    }

    //function to queue the results of pathfinding requests
    public void FinishProcessingPath(PathResult result)
    {
        lock(results)
        {
            results.Enqueue(result);
        }
        /*
        currentRequest.callback(path, success);
        isProcessingPath = false;
        TryProcessNext();*/
    }

}

//struct to hold requests for paths, with a reference to a callback from the requestor to output the result to. 
public struct PathRequest
{
    public Vector3 start;
    public Vector3 end;
    public Action<Vector3[], bool> callback;

    public PathRequest(Vector2 _start, Vector3 _end, Action<Vector3[], bool> _callback)
    {
        start = _start;
        end = _end;
        callback = _callback;
    }
}

//contains the path and status of the path result. Fabricated by FinishProcessingPath. Also contains a callback for a function to trigger reactions from whoever called the path.
public struct PathResult
{
    public Vector3[] path;
    public bool success;
    public Action<Vector3[], bool> callback;

    public PathResult(Vector3[] _path, bool _success, Action<Vector3[], bool> _callback)
    {
        path = _path;
        success = _success;
        callback = _callback;
    }
}
