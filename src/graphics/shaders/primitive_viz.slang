struct ObjectUniforms {
    float4x4 model;
};

struct Vertex {
    float3 position;
    float uvX;
    float3 normal;
    float uvY;
    float4 color;
};

struct PushConstants {
    ObjectUniforms* data;
    Vertex* vertexbuffer;
    float time;
    uint textureId;
    uint samplerId;
};

[vk::push_constant] PushConstants push_consts;
[vk::binding(0, 0)] Texture2D textures[];
[vk::binding(1, 0)] SamplerState samplers[];

float3 rgb_to_hsv(float3 c)
{
    const float4 K = float4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    float4 p = lerp(float4(c.bg, K.wz), float4(c.gb, K.xy), step(c.b, c.g));
    float4 q = lerp(float4(p.xyw, c.r), float4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1e-10;

    return float3(
        abs(q.z + (q.w - q.y) / (6.0 * d + e)),
        d / (q.x + e),
        q.x
    );
}

float3 hsv_to_rgb(float3 c)
{
    const float4 K = float4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    float3 p = abs(frac(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * lerp(K.xxx, saturate(p - K.xxx), c.y);
}

float3 rainbow_fn(float3 initial_color, float time, float speed)
{
    float3 hsv = rgb_to_hsv(initial_color);

    hsv.x = frac(hsv.x + time * speed);

    return hsv_to_rgb(hsv);
}

struct VSOut {
    float4 positionCS : SV_Position;
    float3 color : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float3 normal : TEXCOORD2;
}

[shader("vertex")]
VSOut main(uint vertexID: SV_VulkanVertexID) {
    /// Pull the vertices from the vertex buffer based on SV_VulkanVertexID.
    float3 position = push_consts.vertexbuffer[vertexID].position;
    float2 uv = float2(push_consts.vertexbuffer[vertexID].uvX, push_consts.vertexbuffer[vertexID].uvY);


    VSOut output;
    output.positionCS = mul(push_consts.data.model, float4(position,1.0));
    output.color = float3(1.0);
    output.uv = uv;
    output.normal = push_consts.vertexbuffer[vertexID].normal;

    return output;
}

[shader("fragment")]
float4 main(VSOut vertex, uint h : SV_PrimitiveID) : SV_Target {
    h = (h ^ 61u) ^ (h >> 16u);
    h *= 9u;
    h = h ^ (h >> 4u);
    h *= 0x27d4eb2du;
    h = h ^ (h >> 15u);

    h ^= (h >> 13u);
    h *= 0x85ebca6bu;
    h ^= (h >> 16u);

    float hash = float(h) / 4294967295.0;

    float hsv_h = hash;
    float hsv_s = 0.85;
    float hsv_v = 0.85;

    float3 color = hsv_to_rgb(float3(hsv_h, hsv_s, hsv_v));

    return float4(color, 1.0);
}
