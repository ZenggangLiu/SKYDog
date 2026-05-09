// #define CENTER_CAMERA_TO_OBJECT


using UnityEngine;


namespace SKYDog
{
    public class CameraControl : MonoBehaviour
    {
        /// 过渡时间(秒): 即, 从当前位置到达目标位置的时间
        [Tooltip("过渡时间(秒|当前位置到目标位置的时间)")]
        public float m_transition_time = 0.2f;
        public float m_ScreenEdgeBuffer = 4f;           // Space between the top/bottom most target and the screen edge.
        public float m_MinSize = 6.5f;                  // The smallest orthographic size the camera can be.
        public Transform[] m_Targets;                   // All the targets the camera needs to encompass.


        private Camera m_Camera;                        // Used for referencing the camera.
        private float m_ZoomSpeed;                      // Reference speed for the smooth damping of the orthographic size.
        /// 相机跟随的速度(SmoothDamp更新)
        private Vector3 m_MoveVelocity;
        /// 相机目标点(当观测者移动后, 必须保持的位置)
        private Vector3 m_DesiredPosition;

#if CENTER_CAMERA_TO_OBJECT
        private Vector3 m_AimToRig;                     // The offset to apply to the position so the child camera aim at the desired point 
#endif

        /// 在Script加载时候调用
        private void Awake ()
        {
            m_Camera = GetComponentInChildren<Camera> ();

#if CENTER_CAMERA_TO_OBJECT
            // plane in which the camera rig is in
            /// 使用悬挂物体的：Local位置 + Y方向创建一个平面
            Plane p = new Plane(Vector3.up, transform.position);

            /// 相机的Local位置 + Z方向生成一个Ray
            Ray r = new Ray(m_Camera.transform.position, m_Camera.transform.forward);
            /// 无Hit，hit_time为0
            p.Raycast(r, out float hit_time );

            // This is where the camera aim on the rig plane
            Vector3 aimTArget = r.GetPoint(hit_time);

            /// 要求相机以此物件的位置为观察中心
            /// 如果相机真正的观察中心不在此处，
            /// 将向反方向移动
            m_AimToRig = transform.position - aimTArget; /// 一个从相机真正的观察点，到此物件位置的偏移
#endif
        }


        private void FixedUpdate ()
        {
            // Move the camera towards a desired position.
            Move ();

            // Change the size of the camera based.
            Zoom ();
        }


        private void Move ()
        {
            /// 计算相机目标位置
            FindAveragePosition();

            /// 从当前位置平滑移动到目标位置
#if CENTER_CAMERA_TO_OBJECT
            transform.position =
                Vector3.SmoothDamp(transform.position,
                                   m_DesiredPosition + m_AimToRig,
                                   ref m_MoveVelocity,
                                   m_DampTime);
#else
            transform.position =
                Vector3.SmoothDamp(transform.position,
                                   m_DesiredPosition,
                                   ref m_MoveVelocity,
                                   m_transition_time);
#endif
        }

        /// 计算相机目标位置
        /// FixedUpdate()中调用
        private void FindAveragePosition ()
        {
            Vector3 averagePos = new Vector3 ();
            int numTargets = 0;

            /// 循环所有添加的Camera观测者
            for (int i = 0; i < m_Targets.Length; i++)
            {
                /// 如果当前观测者不Active
                if (m_Targets[i].gameObject.activeSelf == false)
                {
                    continue;
                }

                /// Sum所有观测者的位置:无物理依据
                averagePos += m_Targets[i].position;
                numTargets++;
            }

            /// 求平均
            if (numTargets > 0)
                averagePos /= numTargets;

            /// 避免相机在Y轴上变化(上下变化)
            averagePos.y = transform.position.y;
            
            m_DesiredPosition = averagePos;
        }


        private void Zoom ()
        {
            // Find the required size based on the desired position and smoothly transition to that size.
            float requiredSize = FindRequiredSize();
            m_Camera.orthographicSize = Mathf.SmoothDamp (m_Camera.orthographicSize, requiredSize, ref m_ZoomSpeed, m_transition_time);
        }


        private float FindRequiredSize ()
        {
            // Find the position the camera rig is moving towards in its local space.
            Vector3 desiredLocalPos = m_Camera.transform.InverseTransformPoint(m_DesiredPosition);

            // Start the camera's size calculation at zero.
            float size = 0f;

            // Go through all the targets...
            for (int i = 0; i < m_Targets.Length; i++)
            {
                // ... and if they aren't active continue on to the next target.
                if (!m_Targets[i].gameObject.activeSelf)
                    continue;

                // Otherwise, find the position of the target in the camera's local space.
                Vector3 targetLocalPos = m_Camera.transform.InverseTransformPoint(m_Targets[i].position);

                // Find the position of the target from the desired position of the camera's local space.
                Vector3 desiredPosToTarget = targetLocalPos - desiredLocalPos;

                // Choose the largest out of the current size and the distance of the tank 'up' or 'down' from the camera.
                size = Mathf.Max(size, Mathf.Abs(desiredPosToTarget.y));

                // Choose the largest out of the current size and the calculated size based on the tank being to the left or right of the camera.
                size = Mathf.Max(size, Mathf.Abs(desiredPosToTarget.x) / m_Camera.aspect);
            }

            // Add the edge buffer to the size.
            size += m_ScreenEdgeBuffer;

            // Make sure the camera's size isn't below the minimum.
            size = Mathf.Max (size, m_MinSize);

            return size;
        }


        public void SetStartPositionAndSize ()
        {
            // Find the desired position.
            FindAveragePosition ();

            // Set the camera's position to the desired position without damping.
            transform.position = m_DesiredPosition;

            // Find and set the required size of the camera.
            m_Camera.orthographicSize = FindRequiredSize ();
        }
    }
}
