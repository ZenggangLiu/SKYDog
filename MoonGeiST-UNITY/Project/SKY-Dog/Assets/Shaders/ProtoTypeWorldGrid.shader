Shader "RedSoUL/ProtoTypeWorldGrid"
{
    Properties
    {
        /// 网格的底色
        _BackGroundColor("网格底色", Color) = (0.5, 0.5, 0.5, 1)

        /// 细网格线的颜色
        _SmallGridColor("细网格颜色", Color) = (0.4, 0.4, 0.4, 1)
        /// 细网格的间距
        _SmallGridSizeInUnits("细网格间距", Float) = 1.0
        /// 细网格线的宽度
        _SmallGridLineWidth("细网格线宽度", Range(0.001, 0.2)) = 0.02

        /// 大网格线的颜色: 例如: 每隔10格出现粗线
        _LargeGridColor("大网格颜色", Color) = (1, 0.6, 0, 1)
        /// 大网格的间距
        _LargeGridSizeInUnits("大网格间距", Float) = 4.0
        /// 大网格线的宽度
        _LargeGridLineWidth("大网格线宽度", Range(0.001, 0.5)) = 0.1

        /// 整体发光强度
        _Emission("发光强度", Range(0, 10)) = 1.5
    }

    SubShader
    {
        Tags
        {
            "RenderPipeline" = "UniversalPipeline"
            "RenderType" = "Opaque"
            "Queue" = "Geometry"
        }

        Pass
        {
            Name "ForwardLit"
            Tags { "LightMode" = "UniversalForward" }

            HLSLPROGRAM

            #pragma vertex vert
            #pragma fragment frag

            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl"

            struct Attributes
            {
                float4 positionOS : POSITION;
                float3 normalOS   : NORMAL;
            };

            struct Varyings
            {
                float4 positionCS : SV_POSITION;
                float3 positionWS : TEXCOORD0;
            };

            CBUFFER_START(UnityPerMaterial)
                half4 _BackGroundColor;
                half4 _SmallGridColor;
                half4 _LargeGridColor;

                float _SmallGridSizeInUnits;
                float _LargeGridSizeInUnits;

                float _SmallGridLineWidth;
                float _LargeGridLineWidth;

                float _Emission;
            CBUFFER_END

            Varyings vert(
                const Attributes IN)
            {
                Varyings OUT;

                VertexPositionInputs posInputs =
                    GetVertexPositionInputs(IN.positionOS.xyz);

                OUT.positionCS = posInputs.positionCS;
                OUT.positionWS = posInputs.positionWS;

                return OUT;
            }

            float GridLine(
                const float2 coord,
                const float  scale,
                const float  width)
            {
                const float2 scaled_coord = coord / scale;
                const float2 grid =
                    abs(frac(scaled_coord - 0.5) - 0.5) / fwidth(scaled_coord);
                const float lineDistance = min(grid.x, grid.y);

                return 1.0 - saturate(lineDistance  - width);
            }

            half4 frag(
                const Varyings IN) : SV_Target
            {
                const float2 coord = IN.positionWS.xz;

                const float minorGrid = GridLine(coord, _SmallGridSizeInUnits, _SmallGridLineWidth);
                const float majorGrid = GridLine(coord, _LargeGridSizeInUnits, _LargeGridLineWidth);

                float3 color = _BackGroundColor.rgb;
                color = lerp(color, _SmallGridColor.rgb, minorGrid);
                color = lerp(color, _LargeGridColor.rgb, majorGrid);
                color *= _Emission;

                return half4(color, 1.0);
            }

            ENDHLSL
        }
    }
}
