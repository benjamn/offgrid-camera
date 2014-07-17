/*
Copyright (c) 2013, Broadcom Europe Ltd
Copyright (c) 2013, Tim Gover
All rights reserved.


Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "calibration.h"
#include "RaspiTex.h"
#include "RaspiTexUtil.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <stdio.h>

/* \file calibration.c
 * Example code for implementing Calibration filter as GLSL shaders.
 * The input image is a greyscale texture from the MMAL buffer Y plane.
 */

static GLfloat quad_varray[] = {
   -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
   -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
};

static GLuint quad_vbo;

static RASPITEXUTIL_SHADER_PROGRAM_T calibration_shader =
{
    .vertex_source = NULL,
    .fragment_source = NULL,
    .uniform_names = {"tex", "tex_unit"},
    .attribute_names = {"vertex"},
};

static const EGLint calibration_egl_config_attribs[] =
{
   EGL_RED_SIZE,   8,
   EGL_GREEN_SIZE, 8,
   EGL_BLUE_SIZE,  8,
   EGL_ALPHA_SIZE, 8,
   EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
   EGL_NONE
};


/**
 * Initialisation of shader uniforms.
 *
 * @param width Width of the EGL image.
 * @param width Height of the EGL image.
 */
static int shader_set_uniforms(RASPITEXUTIL_SHADER_PROGRAM_T *shader,
      int width, int height)
{
   GLCHK(glUseProgram(shader->program));
   GLCHK(glUniform1i(shader->uniform_locations[0], 0)); // Texture unit

   /* Dimensions of a single pixel in texture co-ordinates */
   GLCHK(glUniform2f(shader->uniform_locations[1],
            1.0 / (float) width, 1.0 / (float) height));

   /* Enable attrib 0 as vertex array */
   GLCHK(glEnableVertexAttribArray(shader->attribute_locations[0]));
   return 0;
}

static char* read_file(const char* relative_name)
{
  size_t base_offset = strlen(__FILE__) - strlen("calibration.c");
  size_t abs_length = base_offset + strlen(relative_name);
  char* abs_name = (char*) malloc(sizeof(char) * (abs_length + 1));

  strcpy(abs_name, __FILE__);
  abs_name[base_offset] = '\0';
  strcat(abs_name, relative_name);

  FILE* f = fopen(abs_name, "r");
  free(abs_name);

  if (f == NULL) {
    return NULL;
  }

  fseek(f, 0, SEEK_END);
  long len = ftell(f);
  rewind(f);

  char* buf = (char*) malloc(sizeof(char) * len);
  if (buf == NULL) {
    fclose(f);
    return NULL;
  }

  if (fread(buf, sizeof(char), len, f) != len) {
    fclose(f);
    free(buf);
    return NULL;
  }

  fclose(f);
  return buf;
}

/**
 * Creates the OpenGL ES 2.X context and builds the shaders.
 * @param raspitex_state A pointer to the GL preview state.
 * @return Zero if successful.
 */
static int calibration_init(RASPITEX_STATE *raspitex_state)
{
    int rc = 0;
    int width = raspitex_state->width;
    int height = raspitex_state->height;
    char* vsrc = read_file("calibration.vert");
    char* fsrc = read_file("calibration.frag");

    calibration_shader.vertex_source = vsrc;
    calibration_shader.fragment_source = fsrc;

    vcos_log_trace("%s", VCOS_FUNCTION);
    raspitex_state->egl_config_attribs = calibration_egl_config_attribs;
    rc = raspitexutil_gl_init_2_0(raspitex_state);
    if (rc != 0)
       goto end;

    rc = raspitexutil_build_shader_program(&calibration_shader);
    if (rc != 0)
       goto end;

    rc = shader_set_uniforms(&calibration_shader, width, height);
    if (rc != 0)
       goto end;

    GLCHK(glGenBuffers(1, &quad_vbo));
    GLCHK(glBindBuffer(GL_ARRAY_BUFFER, quad_vbo));
    GLCHK(glBufferData(GL_ARRAY_BUFFER, sizeof(quad_varray), quad_varray, GL_STATIC_DRAW));
    GLCHK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

end:
    free(vsrc);
    free(fsrc);
    return rc;
}

/* Redraws the scene with the latest luma buffer.
 *
 * @param raspitex_state A pointer to the GL preview state.
 * @return Zero if successful.
 */
static int calibration_redraw(RASPITEX_STATE* state)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   GLCHK(glUseProgram(calibration_shader.program));

   /* Bind the Y plane texture */
   GLCHK(glActiveTexture(GL_TEXTURE0));
   GLCHK(glBindTexture(GL_TEXTURE_EXTERNAL_OES, state->y_texture));
   GLCHK(glBindBuffer(GL_ARRAY_BUFFER, quad_vbo));
   GLCHK(glEnableVertexAttribArray(calibration_shader.attribute_locations[0]));
   GLCHK(glVertexAttribPointer(calibration_shader.attribute_locations[0], 2, GL_FLOAT, GL_FALSE, 0, 0));
   GLCHK(glDrawArrays(GL_TRIANGLES, 0, 6));

   return 0;
}

int calibration_open(RASPITEX_STATE *state)
{
   state->ops.gl_init = calibration_init;
   state->ops.redraw = calibration_redraw;
   state->ops.update_y_texture = raspitexutil_update_y_texture;
   return 0;
}
