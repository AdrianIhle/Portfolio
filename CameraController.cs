using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//[ExecuteInEditMode]
public class CameraController : MonoBehaviour
{
    [SerializeField]
    float cameraMoveThreshold = 0.01f;
    [SerializeField]
    Vector3 cameraTarget;
    //[SerializeField]
    bool updateCameraPosition;

    [Space]
    [Header("Targeting")]
    [SerializeField]
    Transform target;
    Vector3 oldTargetPosition;
    [SerializeField]
    Vector3 offsetDirection;
    [SerializeField]
    Transform offsetMax;
    [SerializeField]
    Transform offsetMin;

    [Space]
    [Header("Rotation")]
    [SerializeField]
    float rotationSpeed;
    [SerializeField]
    float rotationThreshold;
    [SerializeField]
    float rotationScale;
    [SerializeField]
    float rotation;
    bool cancelRotation;

    [Space]
    [Header("Zoom")]
    [SerializeField]
    float minZoomDistance;
    [SerializeField]
    float maxZoomDistance;
    [SerializeField]
    AnimationCurve zoomCurve;
    [SerializeField]
    float currentZoom = 0.5f;
    [SerializeField]
    float zoomScale;

    [Header("Look Effects")]
    public bool lookAtTarget;
    [SerializeField]
    Transform lookAtOffset;



    [Header("Debug")]
    public bool showDebugGizmos;

    // Start is called before the first frame update
    void Start()
    {
        //target = GameObject.FindGameObjectWithTag("Player").transform;

        transform.position = cameraTarget;

    }

    // Update is called once per frame
    void Update()
    {
#if UNITY_EDITOR
        if(!Application.isPlaying)
            transform.position = offsetMax.position;
#endif
        offsetDirection = (offsetMax.position - offsetMin.position).normalized;
        maxZoomDistance = Vector3.Distance(offsetMax.position, offsetMin.position);
    }

    private void LateUpdate()
    {
        if(oldTargetPosition != target.position)
        {
            updateCameraPosition = true;
            oldTargetPosition = target.position;
        }

        if(updateCameraPosition)
        {
            CreateCameraTarget();
            //print($"distance to target {Vector3.Distance(transform.position, cameraTarget)}");
        }

        if(Vector3.Distance(transform.position, cameraTarget) > cameraMoveThreshold)
        {
            ApplyTransformation();
        }

        if (target != null && lookAtTarget)
        {
            LookAtTarget();
        }


    }


    void LookAtTarget()
    {
        Vector3 relativePosition = lookAtOffset.transform.position - transform.position;
        Quaternion rotation = Quaternion.LookRotation(relativePosition, Vector3.up);
        transform.rotation = rotation;
    }

    public void Zoom(float _zoomValue)
    {
        float oldZoom = currentZoom;
        currentZoom += _zoomValue * zoomScale;

        //print($"zoom input: {zoomValue} | zoom value {currentZoom}");
        if (currentZoom < 0.05f)
            currentZoom = 0.05f;

        if (currentZoom > 0.90f)
            currentZoom = 0.90f;

        if (oldZoom != currentZoom)
            updateCameraPosition = true;

    }

    public void Rotate(float _rotationInput)
    {
        if(Mathf.Abs(_rotationInput) > rotationThreshold)
        {
            rotation += _rotationInput * rotationScale;

            updateCameraPosition = true;
        }

    }

    Vector3 ZoomCurvePositon(float _zoomAmount)
    {
        float heightDifferential = Mathf.Abs(offsetMax.position.y - offsetMin.position.y);
        Vector3 pos = offsetMin.position + (offsetDirection*maxZoomDistance * zoomCurve.Evaluate(_zoomAmount));
        pos.y = offsetMax.position.y - heightDifferential* zoomCurve.Evaluate(1.0f - _zoomAmount);
        return pos;
    }

    void CreateCameraTarget()
    {
        //cameraTarget.position = transform.position;
        cameraTarget = ZoomCurvePositon(currentZoom);
        Vector3 center = new Vector3(target.position.x, cameraTarget.y, target.position.z);
        float radius = Vector3.Distance(cameraTarget, center);

        if(cancelRotation)
        {
            //change so it stops applying rotation and uses current rotation
            cameraTarget = new Vector3(center.x + Mathf.Cos(rotation) * radius, cameraTarget.y, center.z + Mathf.Sin(rotation) * radius);
        }
        else
            cameraTarget =  new Vector3(center.x + Mathf.Cos(rotation)*radius, cameraTarget.y, center.z + Mathf.Sin(rotation)*radius);

    }

    void ApplyTransformation()
    {
        transform.position = Vector3.SlerpUnclamped(transform.position, cameraTarget, rotationSpeed * Time.deltaTime);
        updateCameraPosition = false;
    }

    public void CancelCameraRotation()
    {

    }

    private void OnDrawGizmos()
    {
#if UNITY_EDITOR
        if (showDebugGizmos)
        {
           /* Gizmos.color = Color.black;
            Gizmos.DrawLine(target.position, transform.position);
            Gizmos.color = Color.yellow;
            Gizmos.DrawLine(target.position, target.position + offsetDirection);
            Gizmos.color = Color.red;
            Gizmos.DrawLine(target.position, target.position + offsetDirection * maxZoomDistance);*/

            //create positions [check], create center position using player x,z [check], make circle [check], track on that circle



            Gizmos.color = Color.blue;
            List<Vector3> zoomPos = new List<Vector3>();

            for (float i = 0.0f; i <= 1.0f; i += zoomScale)
            {
                Vector3 v = ZoomCurvePositon(i);
                zoomPos.Add(v);
                Gizmos.DrawWireSphere(v, 0.3f);

                Vector3 v2 = ZoomCurvePositon(i);
                v2.x = target.position.x;
                v2.z = target.position.z;

                Gizmos.DrawWireSphere(v2, 0.1f);
                float radius = Vector3.Distance(v, v2);
                UnityEditor.Handles.color = Color.red;
                UnityEditor.Handles.DrawWireDisc(v2, Vector3.up, radius, 2);
                //Gizmos.DrawWireSphere(v2, radius);
            }

            Gizmos.color = Color.red;

            for (int i = 0; i < zoomPos.Count - 2; i++)
            {
                Gizmos.DrawLine(zoomPos[i], zoomPos[i + 1]);
                //Gizmos.DrawLine(pos[i], orbitPoint);
            }

            if (cameraTarget != null)
            {
                Gizmos.color = Color.cyan;
                Gizmos.DrawSphere(cameraTarget, 0.5f);
                //Gizmos.DrawWireSphere(new Vector3(target.position.x, cameraTarget.y, target.position.z), 0.5f);
                Gizmos.DrawLine(transform.position, cameraTarget);
            }
        }
#endif

    }

}
