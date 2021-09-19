// Stereogram shader (messy port)

// See:
// https://www.shadertoy.com/view/ldGfDG
// https://www.shadertoy.com/view/4djSRW
// http://www.techmind.org/stereo/stech.html

#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

/*
SELECT + TRIANGLE: Off
SELECT + CROSS: Colors
SELECT + SQUARE: B&W
SELECT + CIRCLE: Depth

_C0 Stereo controls
_L 0xD0000000 0x10004001
_L 0xA2000003 0x40400000
_L 0xD0000000 0x10008001
_L 0xA2000003 0x00000000
_L 0xD0000000 0x10001001
_L 0xA2000003 0x3f800000
_L 0xD0000000 0x10002001
_L 0xA2000003 0x40000000

[Stereogram]
Name=Stereogram
Fragment=stereogram.fsh
Vertex=fxaa.vsh
UseDepthBuffer=True
SettingName1=Depth factor
SettingDefaultValue1=5.5
SettingMaxValue1=20.0
SettingMinValue1=1.0
SettingStep1=0.5
SettingName2=Invert depth
SettingDefaultValue2=0.0
SettingMaxValue2=1.0
SettingMinValue2=0.0
SettingStep2=1.0
SettingName3=Strength depth
SettingDefaultValue3=10.0
SettingMaxValue3=20.0
SettingMinValue3=0.05
SettingStep3=0.1
SettingName4=Mode
SettingDefaultValue4=0.0
SettingMaxValue4=3.0
SettingMinValue4=0.0
SettingStep4=1.0
*/

uniform sampler2D sampler0;
uniform sampler2D sampler3;
uniform vec2 u_texelDelta;
uniform vec4 u_time;
uniform vec4 u_setting;
varying vec2 v_texcoord0;

const float XDPI = 200.0;
const float EYE_SEP = XDPI*5.0;
const float OBS_DIST = XDPI*12.0;
const float BASE_DEPTH = 150.0;
const float BASE_SEP = EYE_SEP*BASE_DEPTH/(BASE_DEPTH + OBS_DIST);

void main() {
	if (abs(u_setting.w - 1.0) < 0.5) {
		gl_FragColor = texture2D(sampler0, v_texcoord0);
	} else if (abs(u_setting.w - 2.0) < 0.5) {
		gl_FragColor = vec4(vec3(pow(texture2D(sampler3, v_texcoord0).x, u_setting.z)), 1.0);
	} else {
		vec2 currCoord = v_texcoord0/u_texelDelta;
		for(int i = 0; i < 64; i++) {
			float d = pow(texture2D(sampler3, currCoord*u_texelDelta).x, u_setting.z);
			if (u_setting.y > 0.5)
				d = 1.0 - d;
			float depth = u_setting.x * d * 100.0;
			float sep = EYE_SEP*depth/(depth + OBS_DIST);
			sep = floor(sep);
			if(sep < 1.0 || currCoord.x < 0.0)
				break;
			currCoord.x -= sep;
			currCoord.x = floor(currCoord.x);
		}
		vec2 p = currCoord * 1024.0 + (abs(u_setting.w - 0.0) < 0.5 ? u_time.x : 0.0);
		vec3 p3  = fract(vec3(p.xyx) * 0.1031);
		p3 += dot(p3, p3.yzx + 19.19);
		float v = (p3.x + p3.y) * p3.z;

		gl_FragColor = vec4(fract(v), fract(v*16.0), fract(v*256.0), 1.0);
	}
}
