using System;
using UnityEngine;


namespace SKYDog
{

    /// Topdown 2.5D相机控制器
    [DefaultExecutionOrder(-10)]
    public class TopdownCamera : MonoBehaviour
    {
        // === PUBLIC MEMBERS === //
        [Tooltip("追随者")]
        public GameObject FollowTarget;
        [Tooltip("上下旋转(X轴|度)")]
        public float PitchAngle = 50.0f;
        [Tooltip("自拍杆长度(米)")]
        public float SpringArmLength = 20.0f;
        [Tooltip("水平视角(度)")]
        public float FieldOfView = 45.0f;
        /// 位置过渡时间(秒): 即, 从当前位置到达目标位置的时间
        [Tooltip("位置过渡时间(秒)")]
        public float TransitionTime = 0.2f;


        // === PRIVATE MEMBERS === //
        /// 追随者的模型
        private Renderer m_player_mesh_render;
        /// 相机属性
        private Camera   m_main_camera;
        /// 相机目标位置
        private Vector3  m_camera_target_pos;
        /// 相机跟随的速度(SmoothDamp更新使用)
        private Vector3  m_follow_velocity;


        // === PRIVATE METHODS === //
        /// 在Script加载时候调用
        private void Awake()
        {
            /// 检测追随者
            if (FollowTarget == null)
            {
                Debug.LogError("<color=red><b>[ERROR(" + DateTime.Now + ")]: " +
                               "Please set the target of follow up with \"FollowTarget\"</b></color>");
            }

            /// 获取相机(查找此Script下任何位置是否有相机)
            m_main_camera = gameObject.GetComponentInChildren<Camera>();
            /// 无相机, 创建
            if (m_main_camera == null)
            {
                m_main_camera = gameObject.AddComponent<Camera>();
            }

            /// 配置Camera
            SetupCamera();
        }


        private void Update()
        {
#if UNITY_EDITOR
            SetupCamera();
#endif
            MoveCamera();
        }


        private void SetupCamera()
        {
            m_main_camera.tag = "MainCamera";
            m_main_camera.orthographic = false;

            // === 计算垂直Fov === //
            /// aspect = width / height
            float aspect = (float)Screen.width / Screen.height;

            /// -+------+------o-----------
            ///   \     |     /
            ///    \  Width  /
            /// ----+---+---o-----------^--
            ///      \ HFov/            n
            ///       \ | /             e
            ///        \|/              a
            ///         v               r
            ///         C---------------v--> x
            ///
            /// tangent(HFov * 0.5) =  0.5*width / near
            /// 0.5 * width = tangent(HFov * 0.5) * near
            /// tangent(VFov * 0.5) = 0.5*height / hear
            /// 0.5 * height = tangent(VFov * 0.5) * near
            ///
            ///           0.5*width      tangent(HFov * 0.5) * near     tangent(HFov * 0.5)
            /// aspect = ──────────── = ──────────────────────────── = ─────────────────────
            ///           0.5*height     tangent(VFov * 0.5) * near     tangent(VFov * 0.5)
            ///
            /// tangent(VFov * 0.5) = tangent(HFov * 0.5) / aspect
            /// VFov = 2 * actangent[tangent(HFov * 0.5) / aspect]
            ///
            /// tangent(HFov * 0.5)
            float tangent_half_hfov = Mathf.Tan(Mathf.Deg2Rad * FieldOfView * 0.5f);
            float vertical_fov = 2.0f * Mathf.Atan2(tangent_half_hfov, aspect);
            m_main_camera.fieldOfView = Mathf.Rad2Deg * vertical_fov;

            /// 确定自拍杆的绑定点
            m_player_mesh_render = FollowTarget.GetComponentInChildren<Renderer>();
            if (m_player_mesh_render == null)
            {
                Debug.LogError("<color=red><b>[ERROR(" + DateTime.Now + ")]: " +
                               "Under game object \"" + FollowTarget.name +
                               "\", there is not way to find any kind of mesh!</b></color>");
            }
            Vector3 mount_point = m_player_mesh_render.transform.position;

            /// 计算相机最终偏移
            m_main_camera.transform.position = CalcCameraPos(mount_point);
            /// 计算相机的最终旋转
            m_main_camera.transform.rotation = Quaternion.AngleAxis(PitchAngle, Vector3.right);
        }


        private void MoveCamera()
        {
            /// 获得自拍杆的绑定点
            Vector3 mount_point = m_player_mesh_render.transform.position;

            /// 计算相机最终偏移
            m_camera_target_pos = CalcCameraPos(mount_point);

            /// 平滑移动相机
            m_main_camera.transform.position =
                Vector3.SmoothDamp(m_main_camera.transform.position, m_camera_target_pos,
                                   ref m_follow_velocity, TransitionTime);
        }


        private Vector3 CalcCameraPos(
            Vector3 mount_point)
        {
            float xshift = mount_point.x;
            float yshift = mount_point.y + SpringArmLength * Mathf.Sin(PitchAngle * Mathf.Deg2Rad);
            float zshift = mount_point.z - SpringArmLength * Mathf.Cos(PitchAngle * Mathf.Deg2Rad);
            return new Vector3(xshift, yshift, zshift);
        }
    } /// class FollowUpCamera

} /// namespace SKYDog
