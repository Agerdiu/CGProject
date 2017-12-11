// WORKING SKY SHADER
// modify at ur own taste
// don't forget to change each "yourmapname" in the shader paths



textures/yourmapname/redislands
{
	qer_editorimage textures/yourmapname/redsunset_rt.tga						//Which texture will Radiant use
	
	q3map_sun 1 1 1 180 40 20													//red green blue intensity degrees elevation
	q3map_surfacelight 10
	q3map_nofog																	//delete if u work with q3
	
	surfaceparm noimpact
	surfaceparm nolightmap
	surfaceparm sky																//Do not modify
		
	nopicmip
	nomipmaps
	
	skyparms textures/yourmapname/redsunset - -									//farbox cloudheight nearbox
}