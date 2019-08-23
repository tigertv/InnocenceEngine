#version 450

layout(binding = 7, std140) uniform skyCBuffer
{
    layout(row_major) mat4 sky_p_inv;
    layout(row_major) mat4 sky_v_inv;
    vec4 sky_viewportSize;
    vec4 sky_posWSNormalizer;
    vec4 sky_padding[6];
} _410;

layout(binding = 11, std140) uniform GISkyCBuffer
{
    layout(row_major) mat4 GISky_p_inv;
    layout(row_major) mat4 GISky_v_inv[6];
    vec4 GISky_probeCount;
    vec4 GISky_probeInterval;
    vec4 GISky_workload;
    vec4 GISky_irradianceVolumeOffset;
} _415;

layout(binding = 3, std140) uniform sunCBuffer
{
    vec4 sun_dir;
    vec4 sun_luminance;
    layout(row_major) mat4 sun_r;
    vec4 sun_padding[2];
} _447;

layout(location = 0) out vec4 _entryPointOutput_skyPassRT0;

vec3 _1170;
vec2 _1171;
float _1175;
int _1176;
float _1177;

void main()
{
    vec4 _665 = _415.GISky_p_inv * vec4(((gl_FragCoord.xy / _410.sky_viewportSize.zw) - vec2(0.5)) * 2.0, 0.0, 1.0);
    vec3 _676 = normalize((_415.GISky_v_inv[uint(gl_Layer)] * (_665 / vec4(_665.w))).xyz);
    vec3 _1169;
    if (_676.y > (-0.100000001490116119384765625))
    {
        vec3 _1120;
        vec3 _1121;
        _1121 = _676;
        _1120 = -_447.sun_dir.xyz;
        vec3 _1124;
        vec3 _1133;
        vec3 _1168;
        vec3 _723;
        for (;;)
        {
            _723 = normalize(_1121);
            _1133 = normalize(_1120);
            _1124 = _723;
            vec2 _1122;
            for (;;)
            {
                float _901 = dot(_723, _723);
                float _904 = dot(_723, vec3(0.0, 6371000.0, 0.0));
                float _905 = 2.0 * _904;
                float _920 = (_905 * _905) - (_901 * (-5136797663232.0));
                if (_920 < 0.0)
                {
                    _1122 = vec2(100000.0, -100000.0);
                    break;
                }
                _1122 = vec2(((_904 * (-2.0)) - sqrt(_920)) / (2.0 * _901), ((_904 * (-2.0)) + sqrt(_920)) / (2.0 * _901));
                break;
            }
            if (_1122.x > _1122.y)
            {
                _1168 = vec3(0.0);
                break;
            }
            vec2 _1127;
            vec3 _1129;
            vec3 _1131;
            _1131 = _1133;
            _1129 = _1124;
            _1127 = _1122;
            vec2 _1125;
            for (;;)
            {
                float _956 = dot(_1124, _1124);
                float _959 = dot(_1124, vec3(0.0, 6371000.0, 0.0));
                float _960 = 2.0 * _959;
                float _970 = _960 * _960;
                if (_970 < 0.0)
                {
                    _1125 = vec2(100000.0, -100000.0);
                    break;
                }
                _1125 = vec2(((_959 * (-2.0)) - sqrt(_970)) / (2.0 * _956), _1177);
                break;
            }
            float _748 = min(_1122.y, _1125.x) - _1127.x;
            float _749 = _748 * 0.0625;
            float _752 = dot(_1129, _1131);
            float _1012 = 1.17489993572235107421875 - (0.550000011920928955078125 * pow(0.75800001621246337890625, 3.0));
            vec3 _1135;
            vec3 _1136;
            vec3 _1137;
            vec3 _1141;
            _1141 = _1131;
            _1137 = _1129;
            _1136 = vec3(0.0);
            _1135 = vec3(0.0);
            float _783;
            float _790;
            vec3 _858;
            float _1146;
            float _1147;
            float _1150;
            float _1151;
            vec3 _1154;
            vec3 _1157;
            float _1160;
            int _1163;
            vec3 _1166;
            vec3 _1174;
            int _1134 = 0;
            float _1138 = 0.0;
            float _1139 = 0.0;
            float _1140 = 0.0;
            for (; _1134 < 16; _858 = exp(-(vec3(2.0999999833293259143829345703125e-05 * (_1146 + _1147)) + (vec3(5.8000000535685103386640548706055e-06, 1.3500000022759195417165756225586e-05, 3.3100001019192859530448913574219e-05) * (_1150 + _1151)))), _1141 = _1166, _1140 = _1146, _1139 = _1150, _1138 = _1160 + _749, _1137 = _1174, _1136 = _1157 + (_858 * _790), _1135 = _1154 + (_858 * _783), _1134 = _1163 + 1)
            {
                float _770 = _1138 + (_748 * 0.03125);
                vec3 _771 = _1137 * _770;
                vec3 _772 = vec3(0.0, 6371000.0, 0.0) + _771;
                float _774 = length(_772);
                _783 = exp((6371000.0 - _774) * 0.00012500000593718141317367553710938) * _749;
                _790 = exp((6371000.0 - _774) * 0.00076923076994717121124267578125) * _749;
                _1174 = _1137;
                _1166 = _1141;
                _1163 = _1134;
                _1160 = _1138;
                _1157 = _1136;
                _1154 = _1135;
                _1150 = _1139 + _783;
                _1146 = _1140 + _790;
                vec2 _1142;
                for (;;)
                {
                    float _1039 = dot(_1141, _1141);
                    float _1042 = dot(_1141, _772);
                    float _1043 = 2.0 * _1042;
                    float _1058 = (_1043 * _1043) - ((4.0 * _1039) * (dot(_772, _772) - 41873842372608.0));
                    if (_1058 < 0.0)
                    {
                        _1142 = vec2(100000.0, -100000.0);
                        break;
                    }
                    _1142 = vec2(_1177, ((_1042 * (-2.0)) + sqrt(_1058)) / (2.0 * _1039));
                    break;
                }
                float _802 = _1142.y * 0.125;
                _1151 = 0.0;
                _1147 = 0.0;
                int _1143 = 0;
                float _1167 = 0.0;
                for (; _1143 < 8; )
                {
                    float _815 = _1167 + (_1142.y * 0.0625);
                    float _819 = length(_772 + (_1166 * _815));
                    _1167 += _802;
                    _1151 += (exp((6371000.0 - _819) * 0.00012500000593718141317367553710938) * _802);
                    _1147 += (exp((6371000.0 - _819) * 0.00076923076994717121124267578125) * _802);
                    _1143++;
                    continue;
                }
            }
            _1168 = (((vec3(5.8000000535685103386640548706055e-06, 1.3500000022759195417165756225586e-05, 3.3100001019192859530448913574219e-05) * (0.0596831031143665313720703125 * (1.0 + pow(_752, 2.0)))) * _1135) + (_1136 * (((1.0 - pow(_1012, 2.0)) / ((12.56637096405029296875 * pow(1.0 + (_1012 * _752), 2.0)) + 9.9999997473787516355514526367188e-05)) * 2.0999999833293259143829345703125e-05))) * 22.0;
            break;
        }
        _1169 = _1168;
    }
    else
    {
        _1169 = vec3(0.0);
    }
    _entryPointOutput_skyPassRT0 = vec4(_1169, 1.0);
}

