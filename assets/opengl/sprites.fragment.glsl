#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sampler;
uniform vec4 iSpriteDef;		// sprite info: index,
uniform vec4 iSpriteFx;			// fx: tint_rgb, tintMode
uniform vec4 iSpriteSheet;		// spritesheet metrics: w,h,numx,numy
uniform vec4 iColorKey;			// chroma key

// cnvert YUV<>RGB
vec4 yuv(vec4 rgb) {
    float Y = 0.2989 * rgb.r + 0.5866 * rgb.g + 0.1145 * rgb.b;
    float Cr = 0.492 * (rgb.r - Y);
    float Cb = 0.877 * (rgb.b - Y);
    return vec4(Y,Cb,Cr,rgb.w);
}

void main()
{
	// coords in texture
	vec2 coords = TexCoords;

	// the base sprite size
	vec2 spriteSize =  iSpriteSheet.xy/iSpriteSheet.zw;

	// the sprite index in sheet to paint
    int spriteIndex = int(iSpriteDef.x);

	vec2 sprite = vec2(mod(float(spriteIndex), iSpriteSheet.z), spriteIndex/int(iSpriteSheet.z));
	vec2 spsheetoff = sprite*spriteSize;

	coords /= (iSpriteSheet.zw / iSpriteDef.zw);

	// add offset in sheet
	coords += (spsheetoff / iSpriteSheet.xy);

	// sample the sprite pixel
	vec4 fincolor = texture(sampler, coords);

	if (fincolor.w > 0.) {

		// non-transparent point in the sprite
		if (iSpriteFx.w == 1.) {
			// tinting is enabled in chroma mode
			vec4 yuvColorKey = yuv(iColorKey);
			vec2 ckey=vec2(yuvColorKey.y, yuvColorKey.z);
			vec4 co = yuv(fincolor);
			if (distance(ckey, vec2(co.y, co.z)) < iColorKey.w) {
				// tint the sprite to the new color computing the new shade
				fincolor = vec4( (fincolor.xyz - iColorKey.xyz) + iSpriteFx.xyz * 0.8, fincolor.w);
			}
		} else if (iSpriteFx.w == 2.) {
			// tinting is enabled, full mode
			fincolor = vec4( fincolor.xyz + iSpriteFx.xyz * 0.8, fincolor.w);
		} // else tinting is not enabled and the color read is the final color
		
	} else fincolor = vec4(0,0,0,0);

	color = fincolor;
}
