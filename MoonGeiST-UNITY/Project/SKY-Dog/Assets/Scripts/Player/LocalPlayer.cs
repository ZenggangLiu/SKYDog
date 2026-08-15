/// 定义在: "Project Settings" --> "Script Compilation" --> "Scripting Define Symbols"
///#define DRAW_TARGET_MOVE_DIRECTION
using UnityEngine;
using UnityEngine.InputSystem;


namespace SKYDog
{

    /// 本地玩家控制器
    public class LocalPlayer : MonoBehaviour
    {
        // === PUBLIC MEMBERS === //
        [Tooltip("移动速度(米/秒)")]
        public float MoveSpeed = 4.0f; // 12.0f
        [Tooltip("旋转速度(度/秒)")]
        public float TurnSpeed = 420.0f;


        // === PRIVATE MEMBERS === //
        /// 各种Action的名称
        private const string MOVE_ACTION_NAME = "Move";
        private const string ATTACK_ACTION_NAME = "Attack";

        /// 各种Action的参考
        private InputAction  m_move_action;
        private InputAction  m_attack_action;

#if DRAW_TARGET_MOVE_DIRECTION
        /// 相机Frame上X/Z轴的宽度
        private const float  FRAME_LINE_WIDTH = 0.1f;
        /// 相机Frame上X/Z轴的长度
        private const float  FRAME_LINE_LENGTH = 1.5f;
        /// 目标移动方向线的宽度
        private const float  TARGET_DIRECTION_LINE_WIDTH = 0.05f;
        /// 目标移动方向线的长度
        private const float  TARGET_DIRECTION_LINE_LENGTH = 2.0f;

        /// 相机Frame(FORWARD, RIGHT)
        private LineRenderer m_camera_frame_render;
        /// 目标移动方向
        private LineRenderer  m_target_direction_render;
#endif


        // === PRIVATE METHODS === //
        /// 在Script加载时候调用
        private void Awake()
        {
            /// 获得所有Action的参考
            m_move_action = InputSystem.actions.FindAction(MOVE_ACTION_NAME);
            m_attack_action = InputSystem.actions.FindAction(ATTACK_ACTION_NAME);

#if DRAW_TARGET_MOVE_DIRECTION
            m_camera_frame_render =
                CreateLineRender("CameraFrame", Color.yellow, FRAME_LINE_WIDTH);
            m_camera_frame_render.positionCount = 4;

            m_target_direction_render =
                CreateLineRender("TargetDirection", Color.yellowGreen, TARGET_DIRECTION_LINE_WIDTH);
            m_target_direction_render.positionCount = 2;
#endif
        }


        private void Update()
        {
            // === INPUT处理 === //
            Vector2 user_move_input = m_move_action.ReadValue<Vector2>();
            if (user_move_input.sqrMagnitude > 0.5f)
            {
                // === 获取主相机数据 === //
                /// 获取相机的朝向
                Vector3 camera_forward = Camera.main.transform.forward;
                camera_forward.y = 0;
                camera_forward.Normalize();

                /// 计算相机的RIGHT
                Vector3 camera_right = Vector3.Cross(Vector3.up, camera_forward);

                /// 所有输入都相对于相机而言
                /// 以相机LOOK, RIGHT来合成运动方向
                /// NOTE: Action的输入数据分布
                /// 水平输入(X): 为左右旋转
                /// 垂直输入(Y): 为上下移动
                Vector3 target_direction = (camera_forward * user_move_input.y + camera_right * user_move_input.x);
                target_direction.Normalize();

#if DRAW_TARGET_MOVE_DIRECTION
                // === 绘制相机Frame === //
                {
                    /// Y轴上的偏移
                    const float UP_SHIFT = 0.01f;
                    Vector3 target_dir_line_origin =
                        transform.position + new Vector3(0.0f, UP_SHIFT, 0.0f);

                    /// 相机Z轴
                    m_camera_frame_render.SetPosition(0, target_dir_line_origin);
                    m_camera_frame_render.SetPosition(1,
                        target_dir_line_origin + camera_forward * FRAME_LINE_LENGTH);

                    /// 相机X轴
                    m_camera_frame_render.SetPosition(2, target_dir_line_origin);
                    m_camera_frame_render.SetPosition(3,
                        target_dir_line_origin + camera_right * FRAME_LINE_LENGTH);
                }

                // === 绘制目标移动方向 === //
                {
                    /// 获得模型的中心点
                    Renderer renderer = GetComponentInChildren<Renderer>();
                    float up_shift = renderer == null ? 0.0f : renderer.bounds.center.y;

                    Vector3 target_dir_line_origin =
                        transform.position + new Vector3(0.0f, up_shift, 0.0f);

                    m_target_direction_render.SetPosition(0, target_dir_line_origin);
                    m_target_direction_render.SetPosition(
                        1, target_dir_line_origin + target_direction * TARGET_DIRECTION_LINE_LENGTH);
                }
#endif

                /// 判断是进行旋转还是运动
                /// - 如果玩家当前朝向与目标移动方向夹角很小时 ==> 运动
                // === ROTATE === //
                if (Vector3.Dot(transform.forward, target_direction) < 0.996194698f) /// cos(5°) : 小于5度
                {
                    /// 计算正反旋转角: 当前朝向到目标方向, 目标方向在朝向右侧为正角
                    float angle_2_target =
                        Vector3.SignedAngle(transform.forward, target_direction, Vector3.up);

                    float rotate_angle = Mathf.Sign(angle_2_target)
                                       /// 判断DeltaTime的旋转是否太大
                                       * Mathf.Min(Mathf.Abs(angle_2_target), TurnSpeed * Time.deltaTime);
                    Quaternion input_rotate = Quaternion.AngleAxis(rotate_angle, Vector3.up);
                    transform.rotation *= input_rotate; 
                }
                // === MOVE === //
                else
                {
                    /// 按照Player当前朝向移动
                    Vector3 input_move = transform.forward * MoveSpeed * Time.deltaTime;
                    transform.localPosition += input_move;
                }
            }
        }


#if DRAW_TARGET_MOVE_DIRECTION
        /// 创建一个指定名称的LineRenderer
        private LineRenderer CreateLineRender(
            string render_name,
            Color  line_color,
            float  line_width)
        {
            /// 由于任何一个GO只可以有一个LineRenderer Component
            /// 创建一个子GO并添加LineRenderer
            GameObject render_go = new GameObject(render_name);
            render_go.transform.parent = transform;

            Material line_mat = new Material(Shader.Find("Universal Render Pipeline/Unlit"));
            line_mat.color = line_color;

            LineRenderer line_render = render_go.AddComponent<LineRenderer>();
            line_render.startWidth   = line_width;
            line_render.endWidth     = line_width;
            line_render.startColor   = line_color;
            line_render.endColor     = line_color;
            line_render.material     = line_mat;

            return line_render;
        }
#endif

    }

} /// namespace SKYDog
