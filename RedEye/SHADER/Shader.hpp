#pragma once

#include "Header.hpp"

static const char * test_VERTEX = SHADER(
            layout (location = 0) in vec3 aPos;

            uniform mat4 projection;
            uniform mat4 view;
            uniform mat4 model;

            void main()
            {
                vec3 WorldPos = vec3(model * vec4(aPos, 1.0));
                gl_Position =  projection * view * vec4(WorldPos, 1.0);
            }

            );

static const char * test_FRAGMENT = SHADER(
            out vec4 color;

            void main()
            {
                color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
            );









static const char * s_VERTEX = SHADER(
            layout (location = 0) in vec3 position;
            layout (location = 1) in vec3 texCoords;

            out vec2 TexCoords;

            void main()
            {
                gl_Position = vec4(position * 1.0, 1.0f);
                TexCoords = texCoords.xy;
            }
            );

static const char * s_FRAGMENT = SHADER(
            in vec2 TexCoords;
            out vec4 color;

            uniform sampler2D screenTexture;

            void main()
            {
                //vec4 c = texture(screenTexture, TexCoords);


                vec2 m_outUV = TexCoords;

                float fstep=0.005;
                vec4 sample0=texture(screenTexture,vec2(m_outUV.x-fstep,m_outUV.y-fstep));
                vec4 sample1=texture(screenTexture,vec2(m_outUV.x+fstep,m_outUV.y-fstep));
                vec4 sample2=texture(screenTexture,vec2(m_outUV.x+fstep,m_outUV.y+fstep));
                vec4 sample3=texture(screenTexture,vec2(m_outUV.x-fstep,m_outUV.y+fstep));
                vec4 c = (sample0+sample1+sample2+sample3) / 4.0;


                color = c;
            }
            );


            /*
             *
             * PBR Shader
             *
             */

            static const char * PBR_VERTEX = SHADER(
                            layout (location = 0) in vec3 aPos;
                            layout (location = 1) in vec3 aNormal;
                            layout (location = 2) in vec3 aTexCoords;

                            out vec2 TexCoords;
                            out vec3 WorldPos;
                            out vec3 Normal;

                            uniform mat4 projection;
                            uniform mat4 view;
                            uniform mat4 model;

                            void main()
                            {
                                TexCoords = aTexCoords.xy;
                                WorldPos = vec3(model * vec4(aPos, 1.0));
                                Normal = mat3(model) * aNormal;

                                gl_Position =  projection * view * vec4(WorldPos, 1.0);
                            }
                        );


static const char * PBR_FRAGMENT = SHADER(
            out vec4 FragColor;
            in vec2 TexCoords;
            in vec3 WorldPos;
            in vec3 Normal;

            // material parameters
            uniform sampler2D albedoMap;
            uniform sampler2D normalMap;
            uniform sampler2D metallicMap;
            uniform sampler2D roughnessMap;
            uniform sampler2D aoMap;

            // IBL
            uniform samplerCube irradianceMap;
            uniform samplerCube prefilterMap;
            uniform sampler2D brdfLUT;

            // lights
            uniform vec3 lightPositions[4];
            uniform vec3 lightColors[4];

            uniform vec3 camPos;

            const float PI = 3.14159265359;

            vec3 getNormalFromMap()
            {
                vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

                vec3 Q1  = dFdx(WorldPos);
                vec3 Q2  = dFdy(WorldPos);
                vec2 st1 = dFdx(TexCoords);
                vec2 st2 = dFdy(TexCoords);

                vec3 N   = normalize(Normal);
                vec3 T  = -normalize(Q1*st2.t - Q2*st1.t);
                vec3 B  = -normalize(cross(N, T));
                mat3 TBN = mat3(T, B, N);

                return normalize(TBN * tangentNormal);
            }
            // ----------------------------------------------------------------------------
            float DistributionGGX(vec3 N, vec3 H, float roughness)
            {
                float a = roughness*roughness;
                float a2 = a*a;
                float NdotH = max(dot(N, H), 0.0);
                float NdotH2 = NdotH*NdotH;

                float nom   = a2;
                float denom = (NdotH2 * (a2 - 1.0) + 1.0);
                denom = PI * denom * denom;

                return nom / denom;
            }
            // ----------------------------------------------------------------------------
            float GeometrySchlickGGX(float NdotV, float roughness)
            {
                float r = (roughness + 1.0);
                float k = (r*r) / 8.0;

                float nom   = NdotV;
                float denom = NdotV * (1.0 - k) + k;

                return nom / denom;
            }
            // ----------------------------------------------------------------------------
            float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
            {
                float NdotV = max(dot(N, V), 0.0);
                float NdotL = max(dot(N, L), 0.0);
                float ggx2 = GeometrySchlickGGX(NdotV, roughness);
                float ggx1 = GeometrySchlickGGX(NdotL, roughness);

                return ggx1 * ggx2;
            }
            // ----------------------------------------------------------------------------
            vec3 fresnelSchlick(float cosTheta, vec3 F0)
            {
                return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
            }
            // ----------------------------------------------------------------------------
            vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
            {
                return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
            }
            // ----------------------------------------------------------------------------
            void main()
            {
                // material properties
                vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
                float metallic = texture(metallicMap, TexCoords).r * 1.5;
                float roughness = texture(roughnessMap, TexCoords).r * 0.5;
                float ao = texture(aoMap, TexCoords).r;

                // input lighting data
                vec3 N = getNormalFromMap();
                vec3 V = normalize(camPos - WorldPos);
                vec3 R = reflect(-V, N);

                // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
                // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
                vec3 F0 = vec3(0.04);
                F0 = mix(F0, albedo, metallic);

                // reflectance equation
                vec3 Lo = vec3(0.0);
                for(int i = 0; i < 4; ++i)
                {
                    // calculate per-light radiance
                    vec3 L = normalize(lightPositions[i] - WorldPos);
                    vec3 H = normalize(V + L);
                    float distance = length(lightPositions[i] - WorldPos);
                    float attenuation = 1.0 / (distance * distance);
                    vec3 radiance = lightColors[i] * attenuation;

                    // Cook-Torrance BRDF
                    float NDF = DistributionGGX(N, H, roughness);
                    float G   = GeometrySmith(N, V, L, roughness);
                    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

                    vec3 nominator    = NDF * G * F;
                    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
                    vec3 specular = nominator / denominator;

                     // kS is equal to Fresnel
                    vec3 kS = F;
                    // for energy conservation, the diffuse and specular light can't
                    // be above 1.0 (unless the surface emits light); to preserve this
                    // relationship the diffuse component (kD) should equal 1.0 - kS.
                    vec3 kD = vec3(1.0) - kS;
                    // multiply kD by the inverse metalness such that only non-metals
                    // have diffuse lighting, or a linear blend if partly metal (pure metals
                    // have no diffuse light).
                    kD *= 1.0 - metallic;

                    // scale light by NdotL
                    float NdotL = max(dot(N, L), 0.0);

                    // add to outgoing radiance Lo
                    Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
                }

                // ambient lighting (we now use IBL as the ambient term)
                vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

                vec3 kS = F;
                vec3 kD = 1.0 - kS;
                kD *= 1.0 - metallic;

                vec3 irradiance = texture(irradianceMap, N).rgb;
                vec3 diffuse      = irradiance * albedo;

                const float MAX_REFLECTION_LOD = 4.0;
                vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
                vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
                vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

                vec3 ambient = (kD * diffuse + specular) * ao;

                vec3 color = ambient + Lo;

                // HDR tonemapping
                color = color / (color + vec3(1.0));
                // gamma correct
                color = pow(color, vec3(1.0/2.2));

                FragColor = vec4(color , 1.0);




               //FragColor = vec4(texture(brdfLUT, TexCoords).xyz  , 1.0);
            }
            );



static const char * cubemap_vertex_shader = SHADER(
            layout (location = 0) in vec3 aPos;

            out vec3 WorldPos;

            uniform mat4 projection;
            uniform mat4 view;

            void main()
            {
                WorldPos = aPos;
                gl_Position =  projection * view * vec4(WorldPos, 1.0);
            }
            );

static const char * cubemap_fragment_shader = SHADER(
            out vec4 FragColor;
            in vec3 WorldPos;

            uniform sampler2D equirectangularMap;

            const vec2 invAtan = vec2(0.1591, 0.3183);
            vec2 SampleSphericalMap(vec3 v)
            {
                vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
                uv *= invAtan;
                uv += 0.5;
                return uv;
            }

            void main()
            {
                vec2 uv = SampleSphericalMap(normalize(WorldPos));
                vec3 color = texture(equirectangularMap, uv).rgb;

                FragColor = vec4(color, 1.0);
            }
            );

static const char * background_vertex_shader = SHADER(
            layout (location = 0) in vec3 aPos;

            uniform mat4 projection;
            uniform mat4 view;

            out vec3 WorldPos;

            void main()
            {
                WorldPos = aPos;

                mat4 rotView = mat4(mat3(view));
                vec4 clipPos = projection * rotView * vec4(WorldPos, 1.0);

                gl_Position = clipPos.xyww;
            }
            );

static const char * background_fragment_shader = SHADER(
            out vec4 FragColor;
            in vec3 WorldPos;

            uniform samplerCube environmentMap;

            void main()
            {
                vec3 envColor = texture(environmentMap, WorldPos).rgb;

                // HDR tonemap and gamma correct
                envColor = envColor / (envColor + vec3(1.0));
                envColor = pow(envColor, vec3(1.0/2.2));

                FragColor = vec4(envColor, 1.0);
                //FragColor = vec4(0.5);
            }
            );

static const char * irradiance_vertex_shader = SHADER(
            layout (location = 0) in vec3 aPos;

            out vec3 WorldPos;

            uniform mat4 projection;
            uniform mat4 view;

            void main()
            {
                WorldPos = aPos;
                gl_Position =  projection * view * vec4(WorldPos, 1.0);
            }
            );

static const char * irradiance_fragment_shader = SHADER(
            out vec4 FragColor;
            in vec3 WorldPos;

            uniform samplerCube environmentMap;

            const float PI = 3.14159265359;

            void main()
            {
                // The world vector acts as the normal of a tangent surface
                // from the origin, aligned to WorldPos. Given this normal, calculate all
                // incoming radiance of the environment. The result of this radiance
                // is the radiance of light coming from -Normal direction, which is what
                // we use in the PBR shader to sample irradiance.
                vec3 N = normalize(WorldPos);

                vec3 irradiance = vec3(0.0);

                // tangent space calculation from origin point
                vec3 up    = vec3(0.0, 1.0, 0.0);
                vec3 right = cross(up, N);
                up            = cross(N, right);

                float sampleDelta = 0.025;
                float nrSamples = 0.0;
                for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
                {
                    for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
                    {
                        // spherical to cartesian (in tangent space)
                        vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
                        // tangent space to world
                        vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

                        irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
                        nrSamples++;
                    }
                }
                irradiance = PI * irradiance * (1.0 / float(nrSamples));

                FragColor = vec4(irradiance, 1.0);
            }
            );



static const char * prefilter_vertex_shader = SHADER(
            layout (location = 0) in vec3 aPos;

            out vec3 WorldPos;

            uniform mat4 projection;
            uniform mat4 view;

            void main()
            {
                WorldPos = aPos;
                gl_Position =  projection * view * vec4(WorldPos, 1.0);
            }
            );


static const char * prefilter_fragment_shader = SHADER(
            out vec4 FragColor;
            in vec3 WorldPos;

            uniform samplerCube environmentMap;
            uniform float roughness;

            const float PI = 3.14159265359;
            // ----------------------------------------------------------------------------
            float DistributionGGX(vec3 N, vec3 H, float roughness)
            {
                float a = roughness*roughness;
                float a2 = a*a;
                float NdotH = max(dot(N, H), 0.0);
                float NdotH2 = NdotH*NdotH;

                float nom   = a2;
                float denom = (NdotH2 * (a2 - 1.0) + 1.0);
                denom = PI * denom * denom;

                return nom / denom;
            }
            // ----------------------------------------------------------------------------
            // http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
            // efficient VanDerCorpus calculation.
            float RadicalInverse_VdC(uint bits)
            {
                 bits = (bits << 16u) | (bits >> 16u);
                 bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
                 bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
                 bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
                 bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
                 return float(bits) * 2.3283064365386963e-10; // / 0x100000000
            }
            // ----------------------------------------------------------------------------
            vec2 Hammersley(uint i, uint N)
            {
                return vec2(float(i)/float(N), RadicalInverse_VdC(i));
            }
            // ----------------------------------------------------------------------------
            vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
            {
                float a = roughness*roughness;

                float phi = 2.0 * PI * Xi.x;
                float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
                float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

                // from spherical coordinates to cartesian coordinates - halfway vector
                vec3 H;
                H.x = cos(phi) * sinTheta;
                H.y = sin(phi) * sinTheta;
                H.z = cosTheta;

                // from tangent-space H vector to world-space sample vector
                vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
                vec3 tangent   = normalize(cross(up, N));
                vec3 bitangent = cross(N, tangent);

                vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
                return normalize(sampleVec);
            }
            // ----------------------------------------------------------------------------
            void main()
            {
                vec3 N = normalize(WorldPos);

                // make the simplyfying assumption that V equals R equals the normal
                vec3 R = N;
                vec3 V = R;

                const uint SAMPLE_COUNT = 1024u;
                vec3 prefilteredColor = vec3(0.0);
                float totalWeight = 0.0;

                for(uint i = 0u; i < SAMPLE_COUNT; ++i)
                {
                    // generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
                    vec2 Xi = Hammersley(i, SAMPLE_COUNT);
                    vec3 H = ImportanceSampleGGX(Xi, N, roughness);
                    vec3 L  = normalize(2.0 * dot(V, H) * H - V);

                    float NdotL = max(dot(N, L), 0.0);
                    if(NdotL > 0.0)
                    {
                        // sample from the environment's mip level based on roughness/pdf
                        float D   = DistributionGGX(N, H, roughness);
                        float NdotH = max(dot(N, H), 0.0);
                        float HdotV = max(dot(H, V), 0.0);
                        float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;

                        float resolution = 512.0; // resolution of source cubemap (per face)
                        float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
                        float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

                        float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

                        prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL;
                        totalWeight      += NdotL;
                    }
                }

                prefilteredColor = prefilteredColor / totalWeight;

                FragColor = vec4(prefilteredColor, 1.0);
            }
            );

static const char * brdf_vertex_shader = SHADER(
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoords;

            out vec2 TexCoords;

            void main()
            {
                TexCoords = aTexCoords;
                gl_Position = vec4(aPos, 1.0);
            }
            );

static const char * brdf_fragment_shader = SHADER(
            out vec2 FragColor;
            in vec2 TexCoords;

            const float PI = 3.14159265359;
            // ----------------------------------------------------------------------------
            // http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
            // efficient VanDerCorpus calculation.
            float RadicalInverse_VdC(uint bits)
            {
                 bits = (bits << 16u) | (bits >> 16u);
                 bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
                 bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
                 bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
                 bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
                 return float(bits) * 2.3283064365386963e-10; // / 0x100000000
            }
            // ----------------------------------------------------------------------------
            vec2 Hammersley(uint i, uint N)
            {
                return vec2(float(i)/float(N), RadicalInverse_VdC(i));
            }
            // ----------------------------------------------------------------------------
            vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
            {
                float a = roughness*roughness;

                float phi = 2.0 * PI * Xi.x;
                float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
                float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

                // from spherical coordinates to cartesian coordinates - halfway vector
                vec3 H;
                H.x = cos(phi) * sinTheta;
                H.y = sin(phi) * sinTheta;
                H.z = cosTheta;

                // from tangent-space H vector to world-space sample vector
                vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
                vec3 tangent   = normalize(cross(up, N));
                vec3 bitangent = cross(N, tangent);

                vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
                return normalize(sampleVec);
            }
            // ----------------------------------------------------------------------------
            float GeometrySchlickGGX(float NdotV, float roughness)
            {
                // note that we use a different k for IBL
                float a = roughness;
                float k = (a * a) / 2.0;

                float nom   = NdotV;
                float denom = NdotV * (1.0 - k) + k;

                return nom / denom;
            }
            // ----------------------------------------------------------------------------
            float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
            {
                float NdotV = max(dot(N, V), 0.0);
                float NdotL = max(dot(N, L), 0.0);
                float ggx2 = GeometrySchlickGGX(NdotV, roughness);
                float ggx1 = GeometrySchlickGGX(NdotL, roughness);

                return ggx1 * ggx2;
            }
            // ----------------------------------------------------------------------------
            vec2 IntegrateBRDF(float NdotV, float roughness)
            {
                vec3 V;
                V.x = sqrt(1.0 - NdotV*NdotV);
                V.y = 0.0;
                V.z = NdotV;

                float A = 0.0;
                float B = 0.0;

                vec3 N = vec3(0.0, 0.0, 1.0);

                const uint SAMPLE_COUNT = 1024u;
                for(uint i = 0u; i < SAMPLE_COUNT; ++i)
                {
                    // generates a sample vector that's biased towards the
                    // preferred alignment direction (importance sampling).
                    vec2 Xi = Hammersley(i, SAMPLE_COUNT);
                    vec3 H = ImportanceSampleGGX(Xi, N, roughness);
                    vec3 L = normalize(2.0 * dot(V, H) * H - V);

                    float NdotL = max(L.z, 0.0);
                    float NdotH = max(H.z, 0.0);
                    float VdotH = max(dot(V, H), 0.0);

                    if(NdotL > 0.0)
                    {
                        float G = GeometrySmith(N, V, L, roughness);
                        float G_Vis = (G * VdotH) / (NdotH * NdotV);
                        float Fc = pow(1.0 - VdotH, 5.0);

                        A += (1.0 - Fc) * G_Vis;
                        B += Fc * G_Vis;
                    }
                }
                A /= float(SAMPLE_COUNT);
                B /= float(SAMPLE_COUNT);
                return vec2(A, B);
            }
            // ----------------------------------------------------------------------------
            void main()
            {
                vec2 integratedBRDF = IntegrateBRDF(TexCoords.x, TexCoords.y);
                FragColor = integratedBRDF;
            }
            );
