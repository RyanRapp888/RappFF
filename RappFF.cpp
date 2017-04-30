#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>
#include "Display.h"
#include "GameMap.h"
#include "Character.h"
#include "TheGame.h"
#include <stdio.h>
#include <direct.h>
#include <fstream>
#include <sstream>

static void PrintGLParams();
static void error_callback(int error, const char* description);
static void PrintCurrentPath();
TheGame rappff;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	rappff.KeyHandler(key, scancode, action, mods);
}

int main(void)
{
	SetKeyCallbackFunc(KeyCallback);
	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit())
	{
		std::cout << "Error initializing glfw";
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	rappff.Play();

	return 0;
}

void PrintGLParams()
{
	GLenum params[] = {
	   GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
	   GL_MAX_CUBE_MAP_TEXTURE_SIZE,
	   GL_MAX_DRAW_BUFFERS,
	   GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
	   GL_MAX_TEXTURE_IMAGE_UNITS,
	   GL_MAX_TEXTURE_SIZE,
	   GL_MAX_VARYING_FLOATS,
	   GL_MAX_VERTEX_ATTRIBS,
	   GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
	   GL_MAX_VERTEX_UNIFORM_COMPONENTS,
	   GL_MAX_VIEWPORT_DIMS,
	   GL_STEREO,
	};

	const char *names[] = {
	   "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
	   "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
	   "GL_MAX_DRAW_BUFFERS",
	   "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
	   "GL_MAX_TEXTURE_IMAGE_UNITS",
	   "GL_MAX_TEXTURE_SIZE",
	   "GL_MAX_VARYING_FLOATS",
	   "GL_MAX_VERTEX_ATTRIBS",
	   "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
	   "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
	   "GL_MAX_VIEWPORT_DIMS",
	   "GL_STEREO",
	};

	std::cout << "GL Context Params:\n";
	// integers - only works if the order is 0-10 integer return types
	for (int i = 0; i < 10; i++)
	{
		int v = 0;
		glGetIntegerv(params[i], &v);
		std::cout << names[i] << " = " << v << std::endl;
	}

	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	printf("%s %i %i\n", names[10], v[0], v[1]);
	unsigned char s = 0;
	glGetBooleanv(params[11], &s);
	printf("%s %i\n", names[11], (unsigned int)s);
	printf("-----------------------------\n");
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}



void PrintCurrentPath()
{
   char cCurrentPath[FILENAME_MAX];

   if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
   {
      return;
   }

   cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; 
   printf("The current working directory is %s\n\n", cCurrentPath);
}

enum class gl_state_type
{
	BOOL, INT, FLOAT, DOUBLE, ISENAB
};

struct gl_state_var
{
	gl_state_var() :glcode(0), gl_datatype(gl_state_type::DOUBLE){}
	gl_state_var(GLenum ge, gl_state_type gst) :glcode(ge), gl_datatype(gst){}
	GLenum glcode;
	gl_state_type gl_datatype;
};

static void dumpGlState(const std::string &file)
{
	static std::map<std::string, gl_state_var> lookup;
	if (lookup.size() == 0)
	{
		lookup["GL_ACCUM_ALPHA_BITS"] = gl_state_var(GL_ACCUM_ALPHA_BITS, gl_state_type::INT);
		lookup["GL_ACCUM_BLUE_BITS"] = gl_state_var(GL_ACCUM_BLUE_BITS, gl_state_type::INT);
		lookup["GL_ACCUM_CLEAR_VALUE"] = gl_state_var(GL_ACCUM_CLEAR_VALUE, gl_state_type::DOUBLE);
		lookup["GL_ACCUM_GREEN_BITS"] = gl_state_var(GL_ACCUM_GREEN_BITS, gl_state_type::INT);
		lookup["GL_ACCUM_RED_BITS"] = gl_state_var(GL_ACCUM_RED_BITS, gl_state_type::INT);
		lookup["GL_ALPHA_BIAS"] = gl_state_var(GL_ALPHA_BIAS, gl_state_type::FLOAT);
		lookup["GL_ALPHA_BITS"] = gl_state_var(GL_ALPHA_BITS, gl_state_type::INT);
		lookup["GL_ALPHA_SCALE"] = gl_state_var(GL_ALPHA_SCALE, gl_state_type::FLOAT);
		lookup["GL_ALPHA_TEST"] = gl_state_var(GL_ALPHA_TEST, gl_state_type::ISENAB);
		lookup["GL_ALPHA_TEST_FUNC"] = gl_state_var(GL_ALPHA_TEST_FUNC, gl_state_type::DOUBLE);
		lookup["GL_ALPHA_TEST_REF"] = gl_state_var(GL_ALPHA_TEST_REF, gl_state_type::DOUBLE);
		lookup["GL_ATTRIB_STACK_DEPTH"] = gl_state_var(GL_ATTRIB_STACK_DEPTH, gl_state_type::DOUBLE);
		lookup["GL_AUTO_NORMAL"] = gl_state_var(GL_AUTO_NORMAL, gl_state_type::ISENAB);
		lookup["GL_AUX_BUFFERS"] = gl_state_var(GL_AUX_BUFFERS, gl_state_type::DOUBLE);
		lookup["GL_BLEND"] = gl_state_var(GL_BLEND, gl_state_type::ISENAB);
		lookup["GL_BLEND_DST"] = gl_state_var(GL_BLEND_DST, gl_state_type::DOUBLE);
		lookup["GL_BLEND_SRC"] = gl_state_var(GL_BLEND_SRC, gl_state_type::DOUBLE);
		lookup["GL_BLUE_BIAS"] = gl_state_var(0x0D1B, gl_state_type::FLOAT);
		lookup["GL_BLUE_BITS"] = gl_state_var(0x0D54, gl_state_type::INT);
		lookup["GL_BLUE_SCALE"] = gl_state_var(0x0D1A, gl_state_type::FLOAT);
		lookup["GL_CLIENT_ATTRIB_STACK_DEPTH"] = gl_state_var(0x0BB1, gl_state_type::INT);
		lookup["GL_CLIP_PLANE0"] = gl_state_var(GL_CLIP_PLANE0, gl_state_type::ISENAB);
		lookup["GL_CLIP_PLANE1"] = gl_state_var(GL_CLIP_PLANE1, gl_state_type::ISENAB);
		lookup["GL_CLIP_PLANE2"] = gl_state_var(GL_CLIP_PLANE2, gl_state_type::ISENAB);
		lookup["GL_CLIP_PLANE3"] = gl_state_var(GL_CLIP_PLANE3, gl_state_type::ISENAB);
		lookup["GL_CLIP_PLANE4"] = gl_state_var(GL_CLIP_PLANE4, gl_state_type::ISENAB);
		lookup["GL_CLIP_PLANE5"] = gl_state_var(GL_CLIP_PLANE5, gl_state_type::ISENAB);
		lookup["GL_COLOR_ARRAY"] = gl_state_var(GL_COLOR_ARRAY, gl_state_type::ISENAB);
		lookup["GL_COLOR_CLEAR_VALUE"] = gl_state_var(0x0C22, gl_state_type::DOUBLE);
		lookup["GL_COLOR_LOGIC_OP"] = gl_state_var(0x0BF2, gl_state_type::DOUBLE);
		lookup["GL_COLOR_MATERIAL"] = gl_state_var(GL_COLOR_MATERIAL, gl_state_type::ISENAB);
		lookup["GL_COLOR_MATERIAL_FACE"] = gl_state_var(0x0B55, gl_state_type::DOUBLE);
		lookup["GL_COLOR_MATERIAL_PARAMETER"] = gl_state_var(0x0B56, gl_state_type::DOUBLE);
		lookup["GL_COLOR_WRITEMASK"] = gl_state_var(0x0C23, gl_state_type::DOUBLE);
		lookup["GL_CULL_FACE"] = gl_state_var(GL_CULL_FACE, gl_state_type::ISENAB);
		lookup["GL_CULL_FACE_MODE"] = gl_state_var(0x0B45, gl_state_type::DOUBLE);
		lookup["GL_CURRENT_COLOR"] = gl_state_var(0x0B00, gl_state_type::DOUBLE);
		lookup["GL_CURRENT_INDEX"] = gl_state_var(0x0B01, gl_state_type::DOUBLE);
		lookup["GL_CURRENT_NORMAL"] = gl_state_var(0x0B02, gl_state_type::DOUBLE);
		lookup["GL_CURRENT_RASTER_COLOR"] = gl_state_var(0x0B04, gl_state_type::DOUBLE);
		lookup["GL_CURRENT_RASTER_DISTANCE"] = gl_state_var(0x0B09, gl_state_type::DOUBLE);
		lookup["GL_CURRENT_RASTER_INDEX"] = gl_state_var(0x0B05, gl_state_type::DOUBLE);
		lookup["GL_CURRENT_RASTER_POSITION"] = gl_state_var(0x0B07, gl_state_type::DOUBLE);
		lookup["GL_CURRENT_RASTER_POSITION_VALID"] = gl_state_var(0x0B08, gl_state_type::DOUBLE);
		lookup["GL_CURRENT_RASTER_TEXTURE_COORDS"] = gl_state_var(0x0B06, gl_state_type::DOUBLE);
		lookup["GL_CURRENT_TEXTURE_COORDS"] = gl_state_var(0x0B03, gl_state_type::DOUBLE);
		lookup["GL_DEPTH_BIAS"] = gl_state_var(0x0D1F, gl_state_type::FLOAT);
		lookup["GL_DEPTH_BITS"] = gl_state_var(0x0D56, gl_state_type::DOUBLE);
		lookup["GL_DEPTH_CLEAR_VALUE"] = gl_state_var(0x0B73, gl_state_type::DOUBLE);
		lookup["GL_DEPTH_FUNC"] = gl_state_var(0x0B74, gl_state_type::DOUBLE);
		lookup["GL_DEPTH_RANGE"] = gl_state_var(0x0B70, gl_state_type::DOUBLE);
		lookup["GL_DEPTH_SCALE"] = gl_state_var(0x0D1E, gl_state_type::FLOAT);
		lookup["GL_DEPTH_TEST"] = gl_state_var(GL_DEPTH_TEST, gl_state_type::ISENAB);
		lookup["GL_DEPTH_WRITEMASK"] = gl_state_var(0x0B72, gl_state_type::DOUBLE);
		lookup["GL_DITHER"] = gl_state_var(GL_DITHER, gl_state_type::ISENAB);
		lookup["GL_DOUBLEBUFFER"] = gl_state_var(0x0C32, gl_state_type::DOUBLE);
		lookup["GL_DRAW_BUFFER"] = gl_state_var(0x0C01, gl_state_type::DOUBLE);
		lookup["GL_EDGE_FLAG"] = gl_state_var(0x0B43, gl_state_type::DOUBLE);
		lookup["GL_EDGE_FLAG_ARRAY"] = gl_state_var(GL_EDGE_FLAG_ARRAY, gl_state_type::ISENAB);
		lookup["GL_FEEDBACK_BUFFER_POINTER"] = gl_state_var(0x0DF0, gl_state_type::DOUBLE);
		lookup["GL_FEEDBACK_BUFFER_SIZE"] = gl_state_var(0x0DF1, gl_state_type::DOUBLE);
		lookup["GL_FEEDBACK_BUFFER_TYPE"] = gl_state_var(0x0DF2, gl_state_type::DOUBLE);
		lookup["GL_FOG"] = gl_state_var(GL_FOG, gl_state_type::ISENAB);
		lookup["GL_FOG_COLOR"] = gl_state_var(0x0B66, gl_state_type::DOUBLE);
		lookup["GL_FOG_DENSITY"] = gl_state_var(0x0B62, gl_state_type::DOUBLE);
		lookup["GL_FOG_END"] = gl_state_var(0x0B64, gl_state_type::DOUBLE);
		lookup["GL_FOG_HINT"] = gl_state_var(0x0C54, gl_state_type::DOUBLE);
		lookup["GL_FOG_INDEX"] = gl_state_var(0x0B61, gl_state_type::DOUBLE);
		lookup["GL_FOG_MODE"] = gl_state_var(0x0B65, gl_state_type::DOUBLE);
		lookup["GL_FOG_START"] = gl_state_var(0x0B63, gl_state_type::DOUBLE);
		lookup["GL_FRONT_FACE"] = gl_state_var(0x0B46, gl_state_type::DOUBLE);
		lookup["GL_GREEN_BIAS"] = gl_state_var(0x0D19, gl_state_type::FLOAT);
		lookup["GL_GREEN_BITS"] = gl_state_var(0x0D53, gl_state_type::DOUBLE);
		lookup["GL_GREEN_SCALE"] = gl_state_var(0x0D18, gl_state_type::FLOAT);
		lookup["GL_INDEX_ARRAY"] = gl_state_var(GL_INDEX_ARRAY, gl_state_type::ISENAB);
		lookup["GL_INDEX_BITS"] = gl_state_var(0x0D51, gl_state_type::DOUBLE);
		lookup["GL_INDEX_CLEAR_VALUE"] = gl_state_var(0x0C20, gl_state_type::DOUBLE);
		lookup["GL_INDEX_LOGIC_OP"] = gl_state_var(GL_INDEX_LOGIC_OP, gl_state_type::ISENAB);
		lookup["GL_INDEX_MODE"] = gl_state_var(0x0C30, gl_state_type::DOUBLE);
		lookup["GL_INDEX_OFFSET"] = gl_state_var(0x0D13, gl_state_type::INT);
		lookup["GL_INDEX_SHIFT"] = gl_state_var(0x0D12, gl_state_type::INT);
		lookup["GL_INDEX_WRITEMASK"] = gl_state_var(0x0C21, gl_state_type::DOUBLE);
		lookup["GL_LIGHT_MODEL_AMBIENT"] = gl_state_var(0x0B53, gl_state_type::DOUBLE);
		lookup["GL_LIGHT_MODEL_LOCAL_VIEWER"] = gl_state_var(0x0B51, gl_state_type::DOUBLE);
		lookup["GL_LIGHT_MODEL_TWO_SIDE"] = gl_state_var(0x0B52, gl_state_type::DOUBLE);
		lookup["GL_LIGHTING"] = gl_state_var(GL_LIGHTING, gl_state_type::ISENAB);
		lookup["GL_LINE_SMOOTH"] = gl_state_var(0x0B20, gl_state_type::DOUBLE);
		lookup["GL_LINE_SMOOTH"] = gl_state_var(GL_LINE_SMOOTH, gl_state_type::ISENAB);
		lookup["GL_LINE_SMOOTH_HINT"] = gl_state_var(0x0C52, gl_state_type::DOUBLE);
		lookup["GL_LINE_STIPPLE"] = gl_state_var(GL_LINE_STIPPLE, gl_state_type::ISENAB);
		lookup["GL_LINE_STIPPLE_PATTERN"] = gl_state_var(0x0B25, gl_state_type::DOUBLE);
		lookup["GL_LINE_STIPPLE_REPEAT"] = gl_state_var(0x0B26, gl_state_type::DOUBLE);
		lookup["GL_LINE_WIDTH"] = gl_state_var(0x0B21, gl_state_type::DOUBLE);
		lookup["GL_LINE_WIDTH_GRANULARITY"] = gl_state_var(0x0B23, gl_state_type::DOUBLE);
		lookup["GL_LINE_WIDTH_RANGE"] = gl_state_var(0x0B22, gl_state_type::DOUBLE);
		lookup["GL_LIST_BASE"] = gl_state_var(0x0B32, gl_state_type::DOUBLE);
		lookup["GL_LIST_INDEX"] = gl_state_var(0x0B33, gl_state_type::DOUBLE);
		lookup["GL_LIST_MODE"] = gl_state_var(0x0B30, gl_state_type::DOUBLE);
		lookup["GL_LOGIC_OP_MODE"] = gl_state_var(0x0BF0, gl_state_type::DOUBLE);
		lookup["GL_MAP_COLOR"] = gl_state_var(0x0D10, gl_state_type::BOOL);
		lookup["GL_MAP_STENCIL"] = gl_state_var(0x0D11, gl_state_type::BOOL);
		lookup["GL_MAP1_COLOR_4"] = gl_state_var(GL_MAP1_COLOR_4, gl_state_type::ISENAB);
		lookup["GL_MAP1_GRID_DOMAIN"] = gl_state_var(0x0DD0, gl_state_type::DOUBLE);
		lookup["GL_MAP1_GRID_SEGMENTS"] = gl_state_var(0x0DD1, gl_state_type::DOUBLE);
		lookup["GL_MAP1_INDEX"] = gl_state_var(GL_MAP1_INDEX, gl_state_type::ISENAB);
		lookup["GL_MAP1_NORMAL"] = gl_state_var(GL_MAP1_NORMAL, gl_state_type::ISENAB);
		lookup["GL_MAP1_TEXTURE_COORD_1"] = gl_state_var(GL_MAP1_TEXTURE_COORD_1, gl_state_type::ISENAB);
		lookup["GL_MAP1_TEXTURE_COORD_2"] = gl_state_var(GL_MAP1_TEXTURE_COORD_2, gl_state_type::ISENAB);
		lookup["GL_MAP1_TEXTURE_COORD_3"] = gl_state_var(GL_MAP1_TEXTURE_COORD_3, gl_state_type::ISENAB);
		lookup["GL_MAP1_TEXTURE_COORD_4"] = gl_state_var(GL_MAP1_TEXTURE_COORD_4, gl_state_type::ISENAB);
		lookup["GL_MAP1_VERTEX_3"] = gl_state_var(0x0D97, gl_state_type::DOUBLE);
		lookup["GL_MAP1_VERTEX_4"] = gl_state_var(0x0D98, gl_state_type::DOUBLE);
		lookup["GL_MAP2_COLOR_4"] = gl_state_var(GL_MAP2_COLOR_4, gl_state_type::ISENAB);
		lookup["GL_MAP2_GRID_DOMAIN"] = gl_state_var(0x0DD2, gl_state_type::DOUBLE);
		lookup["GL_MAP2_GRID_SEGMENTS"] = gl_state_var(0x0DD3, gl_state_type::DOUBLE);
		lookup["GL_MAP2_INDEX"] = gl_state_var(GL_MAP2_INDEX, gl_state_type::ISENAB);
		lookup["GL_MAP2_NORMAL"] = gl_state_var(GL_MAP2_NORMAL, gl_state_type::ISENAB);
		lookup["GL_MAP2_TEXTURE_COORD_1"] = gl_state_var(GL_MAP2_TEXTURE_COORD_1, gl_state_type::ISENAB);
		lookup["GL_MAP2_TEXTURE_COORD_2"] = gl_state_var(GL_MAP2_TEXTURE_COORD_2, gl_state_type::ISENAB);
		lookup["GL_MAP2_TEXTURE_COORD_3"] = gl_state_var(GL_MAP2_TEXTURE_COORD_3, gl_state_type::ISENAB);
		lookup["GL_MAP2_TEXTURE_COORD_4"] = gl_state_var(GL_MAP2_TEXTURE_COORD_4, gl_state_type::ISENAB);
		lookup["GL_MAP2_VERTEX_3"] = gl_state_var(GL_MAP2_VERTEX_3, gl_state_type::ISENAB);
		lookup["GL_MAP2_VERTEX_4"] = gl_state_var(GL_MAP2_VERTEX_4, gl_state_type::ISENAB);
		lookup["GL_MATRIX_MODE"] = gl_state_var(0x0BA0, gl_state_type::DOUBLE);
		lookup["GL_MAX_ATTRIB_STACK_DEPTH"] = gl_state_var(0x0D35, gl_state_type::DOUBLE);
		lookup["GL_MAX_CLIENT_ATTRIB_STACK_DEPTH"] = gl_state_var(0x0D3B, gl_state_type::DOUBLE);
		lookup["GL_MAX_CLIP_PLANES"] = gl_state_var(0x0D32, gl_state_type::DOUBLE);
		lookup["GL_MAX_EVAL_ORDER"] = gl_state_var(0x0D30, gl_state_type::DOUBLE);
		lookup["GL_MAX_LIGHTS"] = gl_state_var(0x0D31, gl_state_type::DOUBLE);
		lookup["GL_MAX_LIST_NESTING"] = gl_state_var(0x0B31, gl_state_type::DOUBLE);
		lookup["GL_MAX_MODELVIEW_STACK_DEPTH"] = gl_state_var(0x0D36, gl_state_type::DOUBLE);
		lookup["GL_MAX_NAME_STACK_DEPTH"] = gl_state_var(0x0D37, gl_state_type::DOUBLE);
		lookup["GL_MAX_PIXEL_MAP_TABLE"] = gl_state_var(0x0D34, gl_state_type::DOUBLE);
		lookup["GL_MAX_PROJECTION_STACK_DEPTH"] = gl_state_var(0x0D38, gl_state_type::DOUBLE);
		lookup["GL_MAX_TEXTURE_SIZE"] = gl_state_var(0x0D33, gl_state_type::DOUBLE);
		lookup["GL_MAX_TEXTURE_STACK_DEPTH"] = gl_state_var(0x0D39, gl_state_type::DOUBLE);
		lookup["GL_MAX_VIEWPORT_DIMS"] = gl_state_var(0x0D3A, gl_state_type::DOUBLE);
		//lookup["GL_MODELVIEW_MATRIX"] = gl_state_var(0x0BA6, gl_state_type::DOUBLE);
		lookup["GL_MODELVIEW_STACK_DEPTH"] = gl_state_var(0x0BA3, gl_state_type::DOUBLE);
		lookup["GL_NAME_STACK_DEPTH"] = gl_state_var(0x0D70, gl_state_type::DOUBLE);
		lookup["GL_NORMAL_ARRAY"] = gl_state_var(GL_NORMAL_ARRAY, gl_state_type::ISENAB);
		lookup["GL_NORMALIZE"] = gl_state_var(GL_NORMALIZE, gl_state_type::ISENAB);
		lookup["GL_PACK_ALIGNMENT"] = gl_state_var(0x0D05, gl_state_type::DOUBLE);
		lookup["GL_PACK_LSB_FIRST"] = gl_state_var(0x0D01, gl_state_type::DOUBLE);
		lookup["GL_PACK_ROW_LENGTH"] = gl_state_var(0x0D02, gl_state_type::DOUBLE);
		lookup["GL_PACK_SKIP_PIXELS"] = gl_state_var(0x0D04, gl_state_type::DOUBLE);
		lookup["GL_PACK_SKIP_ROWS"] = gl_state_var(0x0D03, gl_state_type::DOUBLE);
		lookup["GL_PACK_SWAP_BYTES"] = gl_state_var(0x0D00, gl_state_type::DOUBLE);
		lookup["GL_PERSPECTIVE_CORRECTION_HINT"] = gl_state_var(0x0C50, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_A_TO_A"] = gl_state_var(0x0C79, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_A_TO_A_SIZE"] = gl_state_var(0x0CB9, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_B_TO_B"] = gl_state_var(0x0C78, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_B_TO_B_SIZE"] = gl_state_var(0x0CB8, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_G_TO_G"] = gl_state_var(0x0C77, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_G_TO_G_SIZE"] = gl_state_var(0x0CB7, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_I_TO_A"] = gl_state_var(0x0C75, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_I_TO_A_SIZE"] = gl_state_var(0x0CB5, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_I_TO_B"] = gl_state_var(0x0C74, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_I_TO_B_SIZE"] = gl_state_var(0x0CB4, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_I_TO_G"] = gl_state_var(0x0C73, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_I_TO_G_SIZE"] = gl_state_var(0x0CB3, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_I_TO_I"] = gl_state_var(0x0C70, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_I_TO_I_SIZE"] = gl_state_var(0x0CB0, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_I_TO_R"] = gl_state_var(0x0C72, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_I_TO_R_SIZE"] = gl_state_var(0x0CB2, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_R_TO_R"] = gl_state_var(0x0C76, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_R_TO_R_SIZE"] = gl_state_var(0x0CB6, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_S_TO_S"] = gl_state_var(0x0C71, gl_state_type::DOUBLE);
		lookup["GL_PIXEL_MAP_S_TO_S_SIZE"] = gl_state_var(0x0CB1, gl_state_type::DOUBLE);
		lookup["GL_POINT_SIZE"] = gl_state_var(0x0B11, gl_state_type::DOUBLE);
		lookup["GL_POINT_SIZE_GRANULARITY"] = gl_state_var(0x0B13, gl_state_type::DOUBLE);
		lookup["GL_POINT_SIZE_RANGE"] = gl_state_var(0x0B12, gl_state_type::DOUBLE);
		lookup["GL_POINT_SMOOTH"] = gl_state_var(GL_POINT_SMOOTH, gl_state_type::ISENAB);
		lookup["GL_POINT_SMOOTH_HINT"] = gl_state_var(0x0C51, gl_state_type::DOUBLE);
		lookup["GL_POLYGON_MODE"] = gl_state_var(0x0B40, gl_state_type::DOUBLE);
		lookup["GL_POLYGON_OFFSET_FILL"] = gl_state_var(GL_POLYGON_OFFSET_FILL, gl_state_type::ISENAB);
		lookup["GL_POLYGON_OFFSET_LINE"] = gl_state_var(GL_POLYGON_OFFSET_LINE, gl_state_type::ISENAB);
		lookup["GL_POLYGON_OFFSET_POINT"] = gl_state_var(GL_POLYGON_OFFSET_POINT, gl_state_type::ISENAB);
		lookup["GL_POLYGON_SMOOTH"] = gl_state_var(GL_POLYGON_SMOOTH, gl_state_type::ISENAB);
		lookup["GL_POLYGON_SMOOTH_HINT"] = gl_state_var(0x0C53, gl_state_type::DOUBLE);
		lookup["GL_POLYGON_STIPPLE"] = gl_state_var(GL_POLYGON_STIPPLE, gl_state_type::ISENAB);
		//lookup["GL_PROJECTION_MATRIX"] = gl_state_var(0x0BA7, gl_state_type::DOUBLE);
		lookup["GL_PROJECTION_STACK_DEPTH"] = gl_state_var(0x0BA4, gl_state_type::DOUBLE);
		lookup["GL_READ_BUFFER"] = gl_state_var(0x0C02, gl_state_type::DOUBLE);
		lookup["GL_RED_BIAS"] = gl_state_var(0x0D15, gl_state_type::FLOAT);
		lookup["GL_RED_BITS"] = gl_state_var(0x0D52, gl_state_type::DOUBLE);
		lookup["GL_RED_SCALE"] = gl_state_var(0x0D14, gl_state_type::FLOAT);
		lookup["GL_RENDER_MODE"] = gl_state_var(0x0C40, gl_state_type::DOUBLE);
		lookup["GL_RGBA_MODE"] = gl_state_var(0x0C31, gl_state_type::DOUBLE);
		lookup["GL_SCISSOR_BOX"] = gl_state_var(0x0C10, gl_state_type::DOUBLE);
		lookup["GL_SCISSOR_TEST"] = gl_state_var(GL_SCISSOR_TEST, gl_state_type::ISENAB);
		lookup["GL_SELECTION_BUFFER_POINTER"] = gl_state_var(0x0DF3, gl_state_type::DOUBLE);
		lookup["GL_SELECTION_BUFFER_SIZE"] = gl_state_var(0x0DF4, gl_state_type::DOUBLE);
		lookup["GL_SHADE_MODEL"] = gl_state_var(0x0B54, gl_state_type::DOUBLE);
		lookup["GL_STENCIL_BITS"] = gl_state_var(0x0D57, gl_state_type::DOUBLE);
		lookup["GL_STENCIL_CLEAR_VALUE"] = gl_state_var(0x0B91, gl_state_type::DOUBLE);
		lookup["GL_STENCIL_FAIL"] = gl_state_var(0x0B94, gl_state_type::DOUBLE);
		lookup["GL_STENCIL_FUNC"] = gl_state_var(0x0B92, gl_state_type::DOUBLE);
		lookup["GL_STENCIL_PASS_DEPTH_FAIL"] = gl_state_var(0x0B95, gl_state_type::DOUBLE);
		lookup["GL_STENCIL_PASS_DEPTH_PASS"] = gl_state_var(0x0B96, gl_state_type::DOUBLE);
		lookup["GL_STENCIL_REF"] = gl_state_var(0x0B97, gl_state_type::DOUBLE);
		lookup["GL_STENCIL_TEST"] = gl_state_var(GL_STENCIL_TEST, gl_state_type::ISENAB);
		lookup["GL_STENCIL_VALUE_MASK"] = gl_state_var(0x0B93, gl_state_type::DOUBLE);
		lookup["GL_STENCIL_WRITEMASK"] = gl_state_var(0x0B98, gl_state_type::DOUBLE);
		lookup["GL_STEREO"] = gl_state_var(0x0C33, gl_state_type::DOUBLE);
		lookup["GL_SUBPIXEL_BITS"] = gl_state_var(0x0D50, gl_state_type::DOUBLE);
		lookup["GL_TEXTURE_1D"] = gl_state_var(GL_TEXTURE_1D, gl_state_type::ISENAB);
		lookup["GL_TEXTURE_2D"] = gl_state_var(GL_TEXTURE_2D, gl_state_type::ISENAB);
		lookup["GL_TEXTURE_COORD_ARRAY"] = gl_state_var(GL_TEXTURE_COORD_ARRAY, gl_state_type::ISENAB);
		lookup["GL_TEXTURE_GEN_Q"] = gl_state_var(GL_TEXTURE_GEN_Q, gl_state_type::ISENAB);
		lookup["GL_TEXTURE_GEN_R"] = gl_state_var(GL_TEXTURE_GEN_R, gl_state_type::ISENAB);
		lookup["GL_TEXTURE_GEN_S"] = gl_state_var(GL_TEXTURE_GEN_S, gl_state_type::ISENAB);
		lookup["GL_TEXTURE_GEN_T"] = gl_state_var(GL_TEXTURE_GEN_T, gl_state_type::ISENAB);
		lookup["GL_TEXTURE_MATRIX"] = gl_state_var(0x0BA8, gl_state_type::DOUBLE);
		lookup["GL_TEXTURE_STACK_DEPTH"] = gl_state_var(0x0BA5, gl_state_type::DOUBLE);
		lookup["GL_UNPACK_ALIGNMENT"] = gl_state_var(0x0CF5, gl_state_type::DOUBLE);
		lookup["GL_UNPACK_LSB_FIRST"] = gl_state_var(0x0CF1, gl_state_type::DOUBLE);
		lookup["GL_UNPACK_ROW_LENGTH"] = gl_state_var(0x0CF2, gl_state_type::DOUBLE);
		lookup["GL_UNPACK_SKIP_PIXELS"] = gl_state_var(0x0CF4, gl_state_type::DOUBLE);
		lookup["GL_UNPACK_SKIP_ROWS"] = gl_state_var(0x0CF3, gl_state_type::DOUBLE);
		lookup["GL_UNPACK_SWAP_BYTES"] = gl_state_var(0x0CF0, gl_state_type::DOUBLE);
		lookup["GL_VERTEX_ARRAY"] = gl_state_var(GL_VERTEX_ARRAY, gl_state_type::ISENAB);
		lookup["GL_VIEWPORT"] = gl_state_var(0x0BA2, gl_state_type::DOUBLE);
		lookup["GL_ZOOM_X"] = gl_state_var(0x0D16, gl_state_type::DOUBLE);
		lookup["GL_ZOOM_Y"] = gl_state_var(0x0D17, gl_state_type::DOUBLE);
		lookup["GL_COLOR_LOGIC_OP"] = gl_state_var(GL_COLOR_LOGIC_OP, gl_state_type::ISENAB);
	}

	std::ofstream filestr(file, std::ios::app);
	int chumpsize(16);
	for (auto cur : lookup)
	{
		if (cur.second.gl_datatype == gl_state_type::ISENAB)
		{
			GLboolean bool_temp(0);
			bool_temp = glIsEnabled(cur.second.glcode);
			std::string output = (bool_temp == GL_TRUE) ? "TRUE" : "FALSE";
			filestr << cur.first << "=" << output << std::endl;
		}
		else if (cur.second.gl_datatype == gl_state_type::DOUBLE)
		{
			GLdouble *double_temp = new GLdouble[chumpsize];
			for (int aa = 0; aa < chumpsize; aa++) double_temp[aa] = -1;

			glGetDoublev(cur.second.glcode, double_temp);

			filestr << cur.first << "=" << double_temp[0] << std::endl;

			for (int aa = 1; aa < chumpsize; aa++)
			{
				if (fabs(double_temp[aa] + 1) > .001)
				{
					filestr << "   [" << aa << "] = " << double_temp[aa] << std::endl;
				}
			}

			delete[] double_temp;
		}
		else if (cur.second.gl_datatype == gl_state_type::INT)
		{
			GLint *int_temp = new GLint[chumpsize];
			for (int aa = 0; aa < chumpsize; aa++) int_temp[aa] = -1;

			glGetIntegerv(cur.second.glcode, int_temp);
			filestr << cur.first << "=" << int_temp[0] << std::endl;

			for (int aa = 1; aa < chumpsize; aa++)
			{
				if (abs(int_temp[aa] + 1) > .001)
				{
					filestr << "   [" << aa << "] = " << int_temp[aa] << std::endl;
				}
			}

			delete[] int_temp;
		}
		else if (cur.second.gl_datatype == gl_state_type::FLOAT)
		{
			GLfloat *float_temp = new GLfloat[chumpsize];
			for (int aa = 0; aa < chumpsize; aa++) float_temp[aa] = -1;
			glGetFloatv(cur.second.glcode, float_temp);
			filestr << cur.first << "=" << float_temp[0] << std::endl;

			for (int aa = 1; aa < chumpsize; aa++)
			{
				if (fabs(float_temp[aa] + 1) > .001)
				{
					filestr << "   [" << aa << "] = " << float_temp[aa] << std::endl;
				}
			}


			delete[] float_temp;
		}
		else if (cur.second.gl_datatype == gl_state_type::BOOL)
		{
			GLboolean bool_temp2;
			glGetBooleanv(cur.second.glcode, &bool_temp2);
			std::string output = (bool_temp2 == GL_TRUE) ? "TRUE" : "FALSE";
			filestr << cur.first << "=" << output << std::endl;
		}

	}

	filestr.close();
}


